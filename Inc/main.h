/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FRAM_WP_Pin GPIO_PIN_13
#define FRAM_WP_GPIO_Port GPIOC
#define I2C_SCL_Pin GPIO_PIN_14
#define I2C_SCL_GPIO_Port GPIOC
#define I2C_SDA_Pin GPIO_PIN_15
#define I2C_SDA_GPIO_Port GPIOC
#define HADN_EN_Pin GPIO_PIN_0
#define HADN_EN_GPIO_Port GPIOC
#define HAND_UP_Pin GPIO_PIN_1
#define HAND_UP_GPIO_Port GPIOC
#define HAND_DOWN_Pin GPIO_PIN_2
#define HAND_DOWN_GPIO_Port GPIOC
#define HAND_CAP_Pin GPIO_PIN_3
#define HAND_CAP_GPIO_Port GPIOC
#define MOTOR_COUNTER_Pin GPIO_PIN_0
#define MOTOR_COUNTER_GPIO_Port GPIOA
#define OIL_POS_Pin GPIO_PIN_1
#define OIL_POS_GPIO_Port GPIOA
#define DIN10_Pin GPIO_PIN_2
#define DIN10_GPIO_Port GPIOA
#define DIN9_Pin GPIO_PIN_3
#define DIN9_GPIO_Port GPIOA
#define DIN8_Pin GPIO_PIN_4
#define DIN8_GPIO_Port GPIOA
#define DIN7_Pin GPIO_PIN_5
#define DIN7_GPIO_Port GPIOA
#define DIN6_Pin GPIO_PIN_6
#define DIN6_GPIO_Port GPIOA
#define DIN5_Pin GPIO_PIN_7
#define DIN5_GPIO_Port GPIOA
#define DIN4_Pin GPIO_PIN_4
#define DIN4_GPIO_Port GPIOC
#define DIN3_Pin GPIO_PIN_5
#define DIN3_GPIO_Port GPIOC
#define DIN2_Pin GPIO_PIN_0
#define DIN2_GPIO_Port GPIOB
#define DIN1_Pin GPIO_PIN_1
#define DIN1_GPIO_Port GPIOB
#define POWER_CHECK_Pin GPIO_PIN_11
#define POWER_CHECK_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOF
#define LED5_Pin GPIO_PIN_0
#define LED5_GPIO_Port GPIOG
#define LED6_Pin GPIO_PIN_1
#define LED6_GPIO_Port GPIOG
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_8
#define KEY2_GPIO_Port GPIOE
#define KEY3_Pin GPIO_PIN_9
#define KEY3_GPIO_Port GPIOE
#define KEY4_Pin GPIO_PIN_10
#define KEY4_GPIO_Port GPIOE
#define KEY5_Pin GPIO_PIN_11
#define KEY5_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_13
#define LCD_RST_GPIO_Port GPIOE
#define LCD_A0_Pin GPIO_PIN_14
#define LCD_A0_GPIO_Port GPIOE
#define LCD_BKL_Pin GPIO_PIN_15
#define LCD_BKL_GPIO_Port GPIOE
#define LCD_SCL_Pin GPIO_PIN_10
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_11
#define LCD_SDA_GPIO_Port GPIOB
#define ATT7022_CS_Pin GPIO_PIN_8
#define ATT7022_CS_GPIO_Port GPIOD
#define ATT7022_RESET_Pin GPIO_PIN_9
#define ATT7022_RESET_GPIO_Port GPIOD
#define ATT7022_IRQ_Pin GPIO_PIN_10
#define ATT7022_IRQ_GPIO_Port GPIOD
#define _4G_RESET_Pin GPIO_PIN_11
#define _4G_RESET_GPIO_Port GPIOA
#define OIL_TEMP_Pin GPIO_PIN_12
#define OIL_TEMP_GPIO_Port GPIOA
#define WIFI_RESET_Pin GPIO_PIN_15
#define WIFI_RESET_GPIO_Port GPIOA
#define RS485_DIR_Pin GPIO_PIN_1
#define RS485_DIR_GPIO_Port GPIOD
#define POWER_CTR_Pin GPIO_PIN_11
#define POWER_CTR_GPIO_Port GPIOG
#define DOUT13_Pin GPIO_PIN_12
#define DOUT13_GPIO_Port GPIOG
#define DOUT12_Pin GPIO_PIN_13
#define DOUT12_GPIO_Port GPIOG
#define DOUT11_Pin GPIO_PIN_14
#define DOUT11_GPIO_Port GPIOG
#define DOUT10_Pin GPIO_PIN_15
#define DOUT10_GPIO_Port GPIOG
#define DOUT9_Pin GPIO_PIN_3
#define DOUT9_GPIO_Port GPIOB
#define DOUT8_Pin GPIO_PIN_4
#define DOUT8_GPIO_Port GPIOB
#define DOUT7_Pin GPIO_PIN_5
#define DOUT7_GPIO_Port GPIOB
#define DOUT6_Pin GPIO_PIN_6
#define DOUT6_GPIO_Port GPIOB
#define DOUT5_Pin GPIO_PIN_7
#define DOUT5_GPIO_Port GPIOB
#define DOUT4_Pin GPIO_PIN_8
#define DOUT4_GPIO_Port GPIOB
#define DOUT3_Pin GPIO_PIN_9
#define DOUT3_GPIO_Port GPIOB
#define DOUT2_Pin GPIO_PIN_0
#define DOUT2_GPIO_Port GPIOE
#define DOUT1_Pin GPIO_PIN_1
#define DOUT1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
