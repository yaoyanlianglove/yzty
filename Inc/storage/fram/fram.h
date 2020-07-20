/**
  ******************************************************************************
  * File Name          : fram.h
  * Description        : This file provides functions about fram operations.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fram_H
#define __fram_H
#ifdef __cplusplus
extern "C" {
#endif



#include "main.h"

#define WP_ENABLE   HAL_GPIO_WritePin(FRAM_WP_GPIO_Port, FRAM_WP_Pin, GPIO_PIN_RESET)
#define WP_DISABLE  HAL_GPIO_WritePin(FRAM_WP_GPIO_Port, FRAM_WP_Pin, GPIO_PIN_SET)  
#define CHECK_NUM   3      //数据有问题，需要重新读写的次数


/**
  * @brief  FRAM Status structures definition
  */
typedef enum
{
    FRAM_WRITE_READ_OK           = 0x00U,
    FRAM_WRITE_ERROR             = 0x01U,
    FRAM_READ_ERROR              = 0x02U,
    FRAM_OVERFLOW_ERROR          = 0x03U,
} FRAM_StatusTypeDef;


FRAM_StatusTypeDef FRAM_Write_With_CRC(uint16_t addr, uint8_t *data, uint16_t length);
FRAM_StatusTypeDef FRAM_Read_With_CRC(uint16_t addr, uint8_t *data, uint16_t length);



#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

