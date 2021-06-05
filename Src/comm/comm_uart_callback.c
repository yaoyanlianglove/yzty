/**
  ******************************************************************************
  * File Name          : comm_uart_callback.c
  * Description        : This file contains all functions about HAL UART callback.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */


#include "private_comm.h"
#include "modbus.h"
#include "usart.h"

extern ModbusTypeDef modbus;  
extern uint8_t modbusRxBuffer;

extern PrivateCommTypeDef  privateComm;
extern uint8_t privateCommRxBuffer;
/*****************************************************************************
 Function    : HAL_UART_RxCpltCallback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &MODBUS_UART) 
        RS485DIR_RX;
    else if(huart == &PRIVATE_COMM_UART)
    {
        privateComm.txFinishFlag = 1;
    }
}
/*****************************************************************************
 Function    : HAL_UART_RxCpltCallback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &MODBUS_UART)                                  // 判断是由哪个串口触发的中断
    {
        if(modbus.rxEndFlag == 0)
        {
            modbus.rxBuffer[modbus.rxCount] = modbusRxBuffer;
            if(modbus.rxBeginFlag == 0)
                modbus.rxBeginFlag = 1;
            modbus.rxSpaceCount = 0;
            if(modbus.rxCount < 128)
                modbus.rxCount++;
            else
                modbus.rxCount = 0;
        }
        else
            modbus.rxCount = 0;
        HAL_UART_Receive_IT(&MODBUS_UART, &modbusRxBuffer, 1); // 重新使能串口1接收中断
    }
    else if(huart == &PRIVATE_COMM_UART)
    {
        if(privateComm.rxFinishFlag == 0)
        {
            privateComm.rxBuffer[privateComm.rxCount] = privateCommRxBuffer;
            switch(privateComm.rxCount)
            {
                case 0:
                    if(privateCommRxBuffer == SEND_DATA_HEAD_H)
                        privateComm.rxCount++;
                    else
                        privateComm.rxCount = 0;
                break;
                case 1:
                    if(privateCommRxBuffer == SEND_DATA_HEAD_L)
                        privateComm.rxCount++;
                    else
                        privateComm.rxCount = 0;
                break;
                case 2:
                    if(privateCommRxBuffer == PRIVATE_COMM_DEVICE_ID)
                        privateComm.rxCount++;
                    else
                        privateComm.rxCount = 0;
                break;
                case 3:
                    privateComm.rxCount++;
                break;
                case 4:
                    privateComm.rxCount++;
                    privateComm.rxLength = privateCommRxBuffer << 8;
                break;
                case 5:
                    privateComm.rxCount++;
                    privateComm.rxLength = privateComm.rxLength + privateCommRxBuffer;
                break;
                default:
                    if(privateComm.rxLength > 64)
                    {
                        privateComm.rxCount  = 0;
                        privateComm.rxLength = 0;
                    }
                    if(privateComm.rxCount < privateComm.rxLength - 1)
                        privateComm.rxCount++;
                    else
                    {
                        if(privateComm.rxLength > 2 && privateComm.rxLength < 64)  //不完整帧处理
                            Private_Comm_Rev_Data_Check();
                        else
                        {
                            privateComm.rxCount = 0;
                            privateComm.rxLength = 0;
                        }

                    }
                break;
            }
        }
        HAL_UART_Receive_IT(&PRIVATE_COMM_UART, &privateCommRxBuffer, 1); // 重新使能串口1接收中断                     
    }
}


/************************ZXDQ *****END OF FILE****/

