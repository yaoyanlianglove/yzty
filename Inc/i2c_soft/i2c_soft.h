/**
  ******************************************************************************
  * File Name          : i2c_soft.h
  * Description        : This file provides functions to implement I2C using 
                         GPIO.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_soft_H
#define __i2c_soft_H
#ifdef __cplusplus
extern "C" {
#endif



#include "main.h"

#define I2C_Delay_Time 50   //周期

#define SDA_HIGH    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)       
#define SDA_LOW     HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)
#define SCL_HIGH    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)       
#define SCL_LOW     HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)
#define SDA_STATUS  HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin) 

void    I2C_Send_Start(void);
void    I2C_Send_Stop(void);
uint8_t I2C_Wirte_Byte(uint8_t ndata);
uint8_t I2C_Read_Byte(uint8_t ack);
void    Delay_I2C(uint16_t nDelay);



#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

