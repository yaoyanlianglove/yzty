/**
  ******************************************************************************
  * File Name          : fram.c
  * Description        : This file provides functions about fram operations.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "fram.h"
#include "i2c_soft.h"
#include "crc.h"
/*****************************************************************************
 Function    : FRAM_Read
 Description : Read from FRAM.
 Input       : addr-fram address
               buff-the pointer of data 
               length-the length of data
 Output      : None
 Return      : None
 *****************************************************************************/
void FRAM_Read(uint16_t addr, uint8_t *buff, uint16_t length)
{
    uint16_t i = 0;
    __disable_irq();
    WP_ENABLE;                               //写使能
    Delay_I2C(I2C_Delay_Time);
    I2C_Send_Start();
    I2C_Wirte_Byte(0xA0);                    //写从机地址
    I2C_Wirte_Byte(((addr) >> 8) & 0xff);    //写高地址
    I2C_Wirte_Byte((addr) & 0xff);           //写低地址
    WP_DISABLE;                              //写关闭
    I2C_Send_Start();
    I2C_Wirte_Byte(0xA1);                    //读从机地址
    for(i = 0; i < length-1; i++)
    {
        buff[i] = I2C_Read_Byte(1); 
    }
    buff[i] = I2C_Read_Byte(0);   
    I2C_Send_Stop();
    Delay_I2C(I2C_Delay_Time);
    __enable_irq();
}
/*****************************************************************************
 Function    : FRAM_Write
 Description : Write to FRAM.
 Input       : addr-fram address
               buff-the pointer of data 
               length-the length of data
 Output      : None
 Return      : None
 *****************************************************************************/
void FRAM_Write(uint16_t addr, uint8_t *buff, uint16_t length)
{
    uint16_t i = 0;
    __disable_irq();
    WP_ENABLE;                               //写使能
    Delay_I2C(I2C_Delay_Time);
    I2C_Send_Start();
    I2C_Wirte_Byte(0xA0);                    //写从机地址
    I2C_Wirte_Byte(((addr) >> 8) & 0xff);    //写高地址
    I2C_Wirte_Byte((addr) & 0xff);           //写低地址
    for(i = 0; i < length; i++)
    {
        I2C_Wirte_Byte(buff[i]); 
    }  
    I2C_Send_Stop();
    Delay_I2C(I2C_Delay_Time);
    WP_DISABLE;
    __enable_irq();
}
/*****************************************************************************
 Function    : FRAM_Write_With_CRC
 Description : Write to FRAM.
 Input       : addr-fram address
               data-the pointer of data 
               length-the length of data
 Output      : None
 Return      : write status.
 *****************************************************************************/
FRAM_StatusTypeDef FRAM_Write_With_CRC(uint16_t addr, uint8_t *data, uint16_t length)
{
    uint8_t readData[64] = {0};
    uint16_t crc = 0;
    uint8_t i = 0;
    if(length > 64)                         //fram的一次最大写操作64字节
        return FRAM_OVERFLOW_ERROR;
    while(i < CHECK_NUM)
    {
        FRAM_Write(addr, data, length);
        FRAM_Read(addr, readData, length);
        crc = (readData[length-2] << 8) + readData[length-1];
        if(crc == CRC_16(readData, length - 2))
        {
            return FRAM_WRITE_READ_OK;
        }
        i++;
    }
    if(i == CHECK_NUM)
    {
        return FRAM_WRITE_ERROR;
    }
    return FRAM_WRITE_READ_OK;
}
/*****************************************************************************
 Function    : FRAM_Read_With_CRC
 Description : Read from FRAM.
 Input       : addr-fram address
               data-the pointer of data 
               length-the length of data
 Output      : None
 Return      : read status.
 *****************************************************************************/
FRAM_StatusTypeDef FRAM_Read_With_CRC(uint16_t addr, uint8_t *data, uint16_t length)
{
    uint16_t crc = 0;
    uint8_t i = 0;
    while(i < CHECK_NUM)
    {
        FRAM_Read(addr, data, length);
        crc = (data[length-2] << 8) + data[length-1];
        if(crc == CRC_16(data, length - 2))
        {
            return FRAM_WRITE_READ_OK;
        }
        i++;
    }
    if(i == CHECK_NUM)
    {
        return FRAM_READ_ERROR;
    }
    return FRAM_WRITE_READ_OK;
}
/************************ZXDQ *****END OF FILE****/

