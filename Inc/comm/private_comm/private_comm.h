/**
  ******************************************************************************
  * File Name          : private_comm.h
  * Description        : This file contains all functions about private 
                         communcation protocol.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __private_comm_H
#define __private_comm_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define YK_RETURN_TIME          30
#define PRIVATE_COMM_UART       huart4
#define PRIVATE_COMM_DEVICE_ID        0x0F
#define SEND_DATA_HEAD_H              0x55
#define SEND_DATA_HEAD_L              0xAA
#define PRIVATE_COMM_INPUT_REG_LIMIT  16

#define FUNCTION_CODE_HEART        0x10          
#define FUNCTION_CODE_GET_CFG      0x11
#define FUNCTION_CODE_GET_T_REC    0x12
#define FUNCTION_CODE_GET_A_REC    0x13
#define FUNCTION_CODE_END_REC      0x14
#define FUNCTION_CODE_CLR_REC      0x15
#define FUNCTION_CODE_YC           0x16
#define FUNCTION_CODE_MODE         0x20
#define FUNCTION_CODE_SET_CFG      0x21
#define FUNCTION_CODE_GET_TIME     0x22
#define FUNCTION_CODE_SET_TIME     0x23
#define FUNCTION_CODE_YK           0x30
#define FUNCTION_CODE_YK_CONFIRM   0x31
#define FUNCTION_CODE_ERROR        0x40
#define FUNCTION_CODE_CALIBRATION  0x50
#define FUNCTION_CODE_REBOOT       0x60


#define SEND_STATUS_CODE_LEN    10
#define SEND_CALIBRATION_LEN    8
#define SEND_HEART_LEN          22
#define SEND_GET_CFG_LEN        50  
#define SEND_YC_LEN             40 
#define SEND_GET_TIME_LEN       20       
#define SEND_T_REC_LEN          47
#define SEND_A_REC_LEN          36
#define SEND_CONTROL_LEN         9
#define SEND_SET_CFG_LEN         8


#define STATUS_CODE_CRC_ERR             401
#define STATUS_CODE_NO_YK_ERR           402            //未收到遥控命令
#define STATUS_CODE_OVER_ERR            403            //参数错误
#define STATUS_CODE_STORE_ERR           404            //参数保存失败
#define STATUS_CODE_LOCK_ERR            405            //开关闭锁，拒绝遥控
#define STATUS_CODE_DW_LOW_ERR          406            //档位已最小，禁止降档
#define STATUS_CODE_DW_HIGH_ERR         407            //档位已最大，禁止升档
#define STATUS_CODE_MODE_ERR            408            //模式错误
#define STATUS_CODE_CALIBRATION_ERR     409            //校表失败

/**
  * @brief  Private communication protocol structures definition
  */
typedef struct 
{
    uint8_t  rxBuffer[128];               //接收数据
    uint8_t  txBuffer[128];               //发送数据 
    uint8_t  txLength;                    //发送数据长度
    uint8_t  rxLength;                    //接收数据长度
    uint8_t  rxFinishFlag;                //接收完成标志
    uint8_t  rxCount;                     //接收计数

    uint8_t  flagYkReady;                 //遥控准备标志
    uint8_t  switchMotion;                //遥控动作
    uint16_t ykReturnTimeCounter;

    uint16_t *deviceStatus;               //设备状态
    uint8_t  *currentGear;                //当前档位
    uint8_t  *memoryGear;                 //记忆档位
    uint8_t  *coilReg[32];                //线圈寄存器
    uint8_t  *discreteInputReg[32];       //离散输入寄存器
    int16_t  *inputReg[32];               //输入寄存器
    uint16_t *holdingReg[32];             //保存寄存器
}PrivateCommTypeDef;

void Private_Comm_Start_Rec(void);
void Private_Comm_Rev_Data_Check(void);
void Private_Comm_Yk_Return_Counter(void);
void Private_Comm_Data_Analysis(volatile  uint16_t *remoteHeartTimeout);


uint16_t Private_Comm_Remote_CTR_Callback(uint8_t codeType, uint8_t data);
uint16_t Private_Comm_Set_CFG_Callback(uint8_t codeType, uint16_t *data, uint8_t length);
uint16_t Private_Comm_Calibration_Callback(float u, float i);
uint16_t Private_Comm_Get_Rec_Callback(uint8_t codeType, uint16_t addrNum,  uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  
