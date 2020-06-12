/**
  ******************************************************************************
  * File Name          : led.c
  * Description        : This file provides functions about led control.
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
 Function    : Led_Red_On
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Led_Red_On(void)
{
    LED_RED_ON;
    LED_GREEN_OFF;
}
/*****************************************************************************
 Function    : Led_Green_On
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Led_Green_On(void)
{
    LED_RED_OFF;
    LED_GREEN_ON;
}
/*****************************************************************************
 Function    : Led_Yellow_On
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Led_Yellow_On(void)
{
    LED_RED_ON;
    LED_GREEN_ON;
}
/*****************************************************************************
 Function    : Led_Off
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Led_Off(void)
{
    LED_RED_OFF;
    LED_GREEN_OFF;
}
/*****************************************************************************
 Function    : Led_Control_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Led_Control_Counter(uint8_t deviceStatus, uint8_t gear, uint8_t capa)
{
    uint8_t LED_ALARM_CONTROL_PERIOD = 0;
    if(deviceStatus == 0)
        LED_ALARM_CONTROL_PERIOD = 0;
    else
        LED_ALARM_CONTROL_PERIOD = 3;

    if(ledMSTimeCounter < 1000)
    {
        ledMSTimeCounter++;
        if(ledMSTimeCounter == 400)
        {
            
            if(ledSecTimeCounter < LED_ALARM_CONTROL_PERIOD)
                Led_Yellow_On();
            else if(ledSecTimeCounter >= LED_ALARM_CONTROL_PERIOD && 
                ledSecTimeCounter < (LED_ALARM_CONTROL_PERIOD + LED_CAPA_CONTROL_PERIOD))
            {
                Led_Red_On();
            }
            else if(ledSecTimeCounter < LED_ALARM_CONTROL_PERIOD + LED_CAPA_CONTROL_PERIOD + gear)
                Led_Green_On();
        }
        if(ledMSTimeCounter == 600)
        {
            if(ledSecTimeCounter < LED_ALARM_CONTROL_PERIOD)
            {
                if(deviceStatus == 0)
                    Led_Off();
            }
            else if(ledSecTimeCounter >= LED_ALARM_CONTROL_PERIOD && 
                ledSecTimeCounter < (LED_ALARM_CONTROL_PERIOD + LED_CAPA_CONTROL_PERIOD))
            {
                if(capa == 0)//大容量
#ifdef FUNCTION_TURN_CAPACITY
                    Led_Off();
#else
                    ;
#endif
            }
            else if(gear > 0 && gear <= SWITCH_TYPE)
                Led_Off();
        }
    }
    else
    {
        ledMSTimeCounter = 0;
        if(ledSecTimeCounter < LED_ALARM_CONTROL_PERIOD + LED_CAPA_CONTROL_PERIOD + gear)
            ledSecTimeCounter++;
        else
            ledSecTimeCounter = 0;
    }
}

/************************ZXDQ *****END OF FILE****/

