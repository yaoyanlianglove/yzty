/**
  ******************************************************************************
  * File Name          : private_comm.c
  * Description        : This file contains all functions about private 
                         communcation protocol.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */


#include "private_comm.h"
#include "usart.h"
#include "crc.h"
#include "config_para.h"
#include "storage.h"
#include "fram_addr_map.h"

PrivateCommTypeDef  privateComm;
uint8_t privateCommRxBuffer;

/*****************************************************************************
 Function    : Private_Comm_Start_Rec
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_Start_Rec(void)
{
    privateComm.rxCount      = 0;
    HAL_UART_Receive_IT(&PRIVATE_COMM_UART, &privateCommRxBuffer, 1);
}
/*****************************************************************************
 Function    : Private_Comm_Send_Data
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_Send_Data(uint8_t *pData, uint16_t size)
{
    HAL_UART_Transmit(&PRIVATE_COMM_UART, pData, size, 0xFFFF);
}
/*****************************************************************************
 Function    : Private_Comm_Send_Status_Code
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_Send_Status_Code(uint16_t errorCode)
{
    uint16_t crc;
    privateComm.txBuffer[0] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3] = FUNCTION_CODE_ERROR;
    privateComm.txBuffer[4] = SEND_STATUS_CODE_LEN >> 8;
    privateComm.txBuffer[5] = SEND_STATUS_CODE_LEN & 0xFF;
    privateComm.txBuffer[6] = errorCode >> 8;
    privateComm.txBuffer[7] = errorCode & 0xFF;
    crc = CRC_16(privateComm.txBuffer, SEND_STATUS_CODE_LEN - 2);
    privateComm.txBuffer[SEND_STATUS_CODE_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_STATUS_CODE_LEN - 1] = crc & 0xFF;
    privateComm.txLength = SEND_STATUS_CODE_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_Rev_Data_Check
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_Rev_Data_Check(void)
{
    uint16_t crc = 0;
    crc = (privateComm.rxBuffer[privateComm.rxLength - 2] << 8) + privateComm.rxBuffer[privateComm.rxLength - 1];
    if(crc == CRC_16(privateComm.rxBuffer, privateComm.rxLength - 2))
    {
        privateComm.rxFinishFlag = 1;
        privateComm.rxCount  = 0;
    }
    else
    {
        privateComm.rxCount  = 0;
        Private_Comm_Send_Status_Code(STATUS_CODE_CRC_ERR);
    }
}
/*****************************************************************************
 Function    : Private_Comm_Heart_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_Heart_Response(void)
{
    uint16_t crc;
    privateComm.txBuffer[0] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3] = FUNCTION_CODE_HEART;
    privateComm.txBuffer[4] = SEND_HEART_LEN >> 8;
    privateComm.txBuffer[5] = SEND_HEART_LEN & 0xFF;

    privateComm.txBuffer[6] = *privateComm.currentGear;

    if(*privateComm.coilReg[0 ] == 1)
        privateComm.txBuffer[7] = 0;
    else
        privateComm.txBuffer[7] = 1;
    Read_Switch_Motion_Counter(privateComm.txBuffer + 8);
    Read_Alarm_Counter(privateComm.txBuffer + 12);
    privateComm.txBuffer[14] = (*privateComm.deviceStatus) >> 8;
    privateComm.txBuffer[15] = (*privateComm.deviceStatus) & 0xFF;
    
    Read_Reboot_Counter(privateComm.txBuffer + 16);
    privateComm.txBuffer[18] = *privateComm.memoryGear;
    privateComm.txBuffer[19] = *privateComm.discreteInputReg[19];
    crc = CRC_16(privateComm.txBuffer, SEND_HEART_LEN - 2);
    privateComm.txBuffer[SEND_HEART_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_HEART_LEN - 1] = crc & 0xFF;

    privateComm.txLength = SEND_HEART_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}

/*****************************************************************************
 Function    : Private_Comm_Yk_Return_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_Yk_Return_Counter(void)
{
    if(privateComm.flagYkReady == 1)
    {
        if(privateComm.ykReturnTimeCounter < YK_RETURN_TIME)
        {
            privateComm.ykReturnTimeCounter++;
        }
        else
        {
            privateComm.ykReturnTimeCounter = 0;
            privateComm.flagYkReady = 0;
        }
    }
}
/*****************************************************************************
 Function    : Private_Comm_Control_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_Control_Response(uint8_t code)
{
    uint16_t crc;
    uint16_t res;
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = code;
    privateComm.txBuffer[4 ] = SEND_CONTROL_LEN >> 8;
    privateComm.txBuffer[5 ] = SEND_CONTROL_LEN & 0xFF;
    privateComm.txBuffer[6 ] = privateComm.rxBuffer[6 ];
    
    res = Private_Comm_Remote_CTR_Callback(code, privateComm.rxBuffer[6 ]);
    if(res != 0)
    {
        Private_Comm_Send_Status_Code(res);
        return;
    }
    crc = CRC_16(privateComm.txBuffer, SEND_CONTROL_LEN - 2);
    privateComm.txBuffer[SEND_CONTROL_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_CONTROL_LEN - 1] = crc & 0xFF;
    
    privateComm.txLength = SEND_CONTROL_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_SET_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_SET_Response(uint8_t code)
{
    uint16_t crc;
    uint16_t res;
    uint16_t data[64];
    uint8_t  i, regStart, regNum;
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = code;
    privateComm.txBuffer[4 ] = SEND_SET_CFG_LEN >> 8;
    privateComm.txBuffer[5 ] = SEND_SET_CFG_LEN & 0xFF;
    
    
    if(code == FUNCTION_CODE_SET_CFG)
    {
        regStart = 0;
        regNum   = 18;
    }
    else
    {
        regStart = 21;
        regNum   = 6;
    }
    for(i = 0; i < regNum; i++)
    {
        data[i + regStart] = (privateComm.rxBuffer[6 + i*2] << 8) + privateComm.rxBuffer[7 + i*2];
    }
    res = Private_Comm_Set_CFG_Callback(code, data, regNum);
    if(res != 0)
    {
        Private_Comm_Send_Status_Code(res);
        return;
    }
    crc = CRC_16(privateComm.txBuffer, SEND_SET_CFG_LEN - 2);
    privateComm.txBuffer[SEND_SET_CFG_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_SET_CFG_LEN - 1] = crc & 0xFF;
    
    privateComm.txLength = SEND_SET_CFG_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_GET_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_GET_Response(uint8_t code)
{
    uint16_t crc;
    uint8_t  i, regStart, regNum, length;
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = code;

    if(code == FUNCTION_CODE_GET_CFG)
    {
        length   = SEND_GET_CFG_LEN;
        regStart = 0;
        regNum   = 21*2;
    }
    else
    {
        length = SEND_GET_TIME_LEN;
        regStart = 21;
        regNum   = 6*2;
    }
    privateComm.txBuffer[4 ] = length >> 8;
    privateComm.txBuffer[5 ] = length & 0xFF;

    for(i = 0; i < regNum; i = i + 2)
    {
        privateComm.txBuffer[6 + i] = *(privateComm.holdingReg[regStart + i/2]) >> 8;
        privateComm.txBuffer[7 + i] = *(privateComm.holdingReg[regStart + i/2]) & 0xFF; 
    }
    
    crc = CRC_16(privateComm.txBuffer, length - 2);
    privateComm.txBuffer[length - 2] = crc >> 8;
    privateComm.txBuffer[length - 1] = crc & 0xFF;
    
    privateComm.txLength = length;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_YC_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_YC_Response(void)
{
    uint16_t crc;
    uint8_t i;
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = FUNCTION_CODE_YC;
    privateComm.txBuffer[4 ] = SEND_YC_LEN >> 8;
    privateComm.txBuffer[5 ] = SEND_YC_LEN & 0xFF;

    for(i = 0; i < SEND_YC_LEN - 8; i = i + 2)
    {
        privateComm.txBuffer[6 + i] = *(privateComm.inputReg[i/2]) >> 8;
        privateComm.txBuffer[7 + i] = *(privateComm.inputReg[i/2]) & 0xFF; 
    }
    
    crc = CRC_16(privateComm.txBuffer, SEND_YC_LEN - 2);
    privateComm.txBuffer[SEND_YC_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_YC_LEN - 1] = crc & 0xFF;
    privateComm.txLength = SEND_YC_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_Calibration_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Private_Comm_Calibration_Response(void)
{
    uint16_t crc;
    float u = ((privateComm.rxBuffer[6] << 24) + (privateComm.rxBuffer[7] << 16) + (privateComm.rxBuffer[8] << 8) + privateComm.rxBuffer[9])/10.0;
    float i = ((privateComm.rxBuffer[10] << 24) + (privateComm.rxBuffer[11] << 16) + (privateComm.rxBuffer[12] << 8) + privateComm.rxBuffer[13])/1000.0;

    if(i == 0)
        i = 0.0001;
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = FUNCTION_CODE_CALIBRATION;
    privateComm.txBuffer[4 ] = SEND_CALIBRATION_LEN >> 8;
    privateComm.txBuffer[5 ] = SEND_CALIBRATION_LEN & 0xFF;

    if(Private_Comm_Calibration_Callback(u, i) == 1)
    {
        Private_Comm_Send_Status_Code(STATUS_CODE_CALIBRATION_ERR);
        return;
    }
    crc = CRC_16(privateComm.txBuffer, SEND_CALIBRATION_LEN - 2);
    privateComm.txBuffer[SEND_CALIBRATION_LEN - 2] = crc >> 8;
    privateComm.txBuffer[SEND_CALIBRATION_LEN - 1] = crc & 0xFF;

    privateComm.txLength = SEND_CALIBRATION_LEN;
    Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
}
/*****************************************************************************
 Function    : Private_Comm_GET_REC_Response
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_GET_REC_Response(uint8_t code)
{
    uint16_t crc, length, addrNum, endAddrNum, i, recSum, recPos;
    uint8_t  readData[6];
    privateComm.txBuffer[0 ] = SEND_DATA_HEAD_H;
    privateComm.txBuffer[1 ] = SEND_DATA_HEAD_L;
    privateComm.txBuffer[2 ] = PRIVATE_COMM_DEVICE_ID;
    privateComm.txBuffer[3 ] = code;
    

    addrNum    = (privateComm.rxBuffer[6 ] << 8) + privateComm.rxBuffer[7 ];
    endAddrNum = (privateComm.rxBuffer[8 ] << 8) + privateComm.rxBuffer[9 ];
    if(code == FUNCTION_CODE_GET_T_REC)
    {
        length = SEND_T_REC_LEN;
        Read_Switch_Motion_Counter(readData);
        recSum = (readData[0 ] << 24) + (readData[1 ] << 16) + (readData[2 ] << 8) + readData[3 ];
        Read_Switch_Motion_Num(readData);
        recPos = (readData[0 ] << 8) + readData[1 ];
        if(recSum > SWITCH_MOTION_RECORD_SUM)
            recSum = SWITCH_MOTION_RECORD_SUM;
        if(endAddrNum > recSum)
        {
            Private_Comm_Send_Status_Code(STATUS_CODE_OVER_ERR);
            return;
        }  
    }
    else
    {
        length = SEND_A_REC_LEN;
        Read_Alarm_Counter(readData);
        recSum = (readData[0 ] << 8) + readData[1 ];
        Read_Alarm_Num(readData);
        recPos = (readData[0 ] << 8) + readData[1 ];
        if(recSum > ALARM_RECORD_SUM)
            recSum = ALARM_RECORD_SUM;
        if(endAddrNum > recSum)
        {
            Private_Comm_Send_Status_Code(STATUS_CODE_OVER_ERR);
            return;
        }  
    }
    privateComm.txBuffer[4 ] = length >> 8;
    privateComm.txBuffer[5 ] = length & 0xFF;

    privateComm.txBuffer[6 ] = privateComm.rxBuffer[8 ];
    privateComm.txBuffer[7 ] = privateComm.rxBuffer[9 ];
    
    for(i = addrNum; i < endAddrNum + 1; i++)
    {
        privateComm.txBuffer[8 ] = (i) >> 8;
        privateComm.txBuffer[9 ] = (i) & 0xff;
        if(i < recPos + 2)
            Private_Comm_Get_Rec_Callback(code, recPos + 1 - i, privateComm.txBuffer + 10);
        else
            Private_Comm_Get_Rec_Callback(code, recSum + recPos + 1 - i, privateComm.txBuffer + 10);
    
        crc = CRC_16(privateComm.txBuffer, length - 2);
        privateComm.txBuffer[length - 2] = crc >> 8;
        privateComm.txBuffer[length - 1] = crc & 0xFF;

        privateComm.txLength = length;
        Private_Comm_Send_Data(privateComm.txBuffer, privateComm.txLength);
    } 
}
/*****************************************************************************
 Function    : Private_Comm_Remote_CTR_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
__weak uint16_t Private_Comm_Remote_CTR_Callback(uint8_t codeType, uint8_t data)
{
    return 0;
}
/*****************************************************************************
 Function    : Private_Comm_Set_CFG_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
__weak uint16_t Private_Comm_Set_CFG_Callback(uint8_t codeType, uint16_t *data, uint8_t length)
{
    return 0;
}
/*****************************************************************************
 Function    : Private_Comm_Calibration_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
__weak uint16_t Private_Comm_Calibration_Callback(float u, float i)
{
    return 0;
}
/*****************************************************************************
 Function    : Private_Comm_Get_Rec_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
__weak uint16_t Private_Comm_Get_Rec_Callback(uint8_t codeType, uint16_t addrStart,  uint8_t *data)
{
    return 0;
}
/*****************************************************************************
 Function    : Private_Comm_Data_Analysis
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_Data_Analysis(volatile  uint16_t *remoteHeartTimeout)
{
    if(privateComm.rxFinishFlag == 1)
    {
        *remoteHeartTimeout = (uint16_t)HEART_TIMEOUT_PERIOD;
        switch(privateComm.rxBuffer[3])
        {
            case FUNCTION_CODE_HEART:
                Private_Comm_Heart_Response();
            break;
            case FUNCTION_CODE_GET_CFG:
                Private_Comm_GET_Response(FUNCTION_CODE_GET_CFG);
            break;
            case FUNCTION_CODE_GET_T_REC:
                Private_Comm_GET_REC_Response(FUNCTION_CODE_GET_T_REC);
            break;
            case FUNCTION_CODE_GET_A_REC:
                Private_Comm_GET_REC_Response(FUNCTION_CODE_GET_A_REC);
            break;
            case FUNCTION_CODE_CLR_REC:
                Private_Comm_Control_Response(FUNCTION_CODE_CLR_REC);
            break;
            case FUNCTION_CODE_YC:
                Private_Comm_YC_Response();
            break;
            case FUNCTION_CODE_MODE:
                Private_Comm_Control_Response(FUNCTION_CODE_MODE);
            break;
            case FUNCTION_CODE_SET_CFG:
                Private_Comm_SET_Response(FUNCTION_CODE_SET_CFG);
            break;
            case FUNCTION_CODE_GET_TIME:
                Private_Comm_GET_Response(FUNCTION_CODE_GET_TIME);
            break;
            case FUNCTION_CODE_SET_TIME:
                Private_Comm_SET_Response(FUNCTION_CODE_SET_TIME);
            break;
            case FUNCTION_CODE_YK:
                Private_Comm_Control_Response(FUNCTION_CODE_YK);
            break;
            case FUNCTION_CODE_YK_CONFIRM:
                Private_Comm_Control_Response(FUNCTION_CODE_YK_CONFIRM);
            break;
            case FUNCTION_CODE_CALIBRATION:
                Private_Comm_Calibration_Response();
            break;
            case FUNCTION_CODE_REBOOT:
                Private_Comm_Control_Response(FUNCTION_CODE_REBOOT);
            break;
            default:;
            break;
        }
        privateComm.rxFinishFlag = 0;
    }
}
/************************ZXDQ *****END OF FILE****/

