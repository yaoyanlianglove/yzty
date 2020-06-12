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
    SPI_TMC262_GPIO_Init();
    TMC262_GPIO_Init();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
