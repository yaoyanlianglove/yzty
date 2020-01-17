/**
  ******************************************************************************
  * File Name          : led.c
  * Description        : This file provides functions about led.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */


#include "led.h"
#include "config_para.h"
uint8_t  ledSecTimeCounter = 0;
uint16_t ledMSTimeCounter  = 0;
/*****************************************************************************
 Function    : Led_Control_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Led_Control_Counter(uint16_t deviceStatus, uint8_t gear)
{
    if(ledMSTimeCounter < 1000)
    {
        ledMSTimeCounter++;
        if(ledMSTimeCounter == 500)
        {
            if(deviceStatus == 0)
                LED_RED_OFF;  
            if(gear > 0 && gear < GEAR_TOTAL)
                LED_GREEN_OFF;
        }
    }
    else
    {
        ledMSTimeCounter = 0;
        if(gear == 0 || gear > GEAR_TOTAL - 1)
            gear = GEAR_TOTAL;

        if(ledSecTimeCounter < LED_CONTROL_PERIOD + gear - 1)
            ledSecTimeCounter++;
        else
            ledSecTimeCounter = 0;

        if(ledSecTimeCounter < LED_CONTROL_PERIOD)
        {
            LED_GREEN_OFF;
            LED_RED_ON;
        }
        else 
        {
            LED_RED_OFF;
            LED_GREEN_ON;
        }
    }
}

/************************ZXDQ *****END OF FILE****/

