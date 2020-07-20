/**
  ******************************************************************************
  * File Name          : switch_driver.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "switch_driver.h"
#include "tmc262.h"
#include "spi_tmc262.h"
#include "motor.h"
/*****************************************************************************
 Function    : Switch_Driver_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Switch_Driver_Init(void)
{

    /*Configure GPIO pins : PGPin PGPin PGPin PGPin 
                           PGPin PGPin */
    GPIO_InitStruct.Pin = LED5_Pin|LED6_Pin|DOUT13_Pin|DOUT12_Pin 
                            |DOUT11_Pin|DOUT10_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    SPI_TMC262_GPIO_Init();
    TMC262_GPIO_Init();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
