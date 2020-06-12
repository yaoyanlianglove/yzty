/**
  ******************************************************************************
  * File Name          : led.h
  * Description        : This file contains all functions about led control.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __led_H
#define __led_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define   LED_CAPA_CONTROL_PERIOD   3

#define  LED_RED_OFF        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define  LED_RED_ON         HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)

#define  LED_GREEN_OFF      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define  LED_GREEN_ON       HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)


void Led_Control_Counter(uint8_t deviceStatus, uint8_t gear, uint8_t capa);

#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  
