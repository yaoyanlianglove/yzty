/**
  ******************************************************************************
  * File Name          : spi_tmc262.h
  * Description        : This file contains all functions about tmc262.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_TMC262_H
#define __SPI_TMC262_H

#include "main.h"


#define  TMC262_CS_PORT    DOUT8_GPIO_Port
#define  TMC262_CS_PIN     DOUT8_Pin
 
#define  TMC262_SCK_PORT   DOUT11_GPIO_Port
#define  TMC262_SCK_PIN    DOUT11_Pin
 
#define  TMC262_MOSI_PORT  DOUT12_GPIO_Port
#define  TMC262_MOSI_PIN   DOUT12_Pin
 
#define  TMC262_MISO_PORT  DIN10_GPIO_Port
#define  TMC262_MISO_PIN   DIN10_Pin

#define  TMC262_CS_LOW     HAL_GPIO_WritePin(TMC262_CS_PORT, TMC262_CS_PIN, GPIO_PIN_RESET)   
#define  TMC262_CS_HIGH    HAL_GPIO_WritePin(TMC262_CS_PORT, TMC262_CS_PIN, GPIO_PIN_SET) 

#define  TMC262_SCK_LOW    HAL_GPIO_WritePin(TMC262_SCK_PORT, TMC262_SCK_PIN, GPIO_PIN_RESET) 
#define  TMC262_SCK_HIGH   HAL_GPIO_WritePin(TMC262_SCK_PORT, TMC262_SCK_PIN, GPIO_PIN_SET) 

#define  TMC262_MOSI_LOW   HAL_GPIO_WritePin(TMC262_MOSI_PORT, TMC262_MOSI_PIN, GPIO_PIN_RESET) 
#define  TMC262_MOSI_HIGH  HAL_GPIO_WritePin(TMC262_MOSI_PORT, TMC262_MOSI_PIN, GPIO_PIN_SET) 

#define  TMC262_MISO_READ_DATA HAL_GPIO_ReadPin(DIN10_GPIO_Port, DIN10_Pin)



void SPI_TMC262_GPIO_Init(void);
uint32_t TMC262_Read_Write(uint32_t data);

#endif


/************************ZXDQ *****END OF FILE****/
