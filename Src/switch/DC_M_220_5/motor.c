/**
  ******************************************************************************
  * File Name          : motor.c
  * Description        : This file contains all functions about motor.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "crc.h"
#include "fram_addr_map.h"
#include "fram.h"
#include "motor.h"
#include "switch_driver.h"

/*****************************************************************************
 Function    : Motor_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Init(MotorTypeDef *motor)
{
    uint8_t res = 0;
    motor->dutyCycle  = 700.0; 
    if(HAL_TIM_Base_Start(&htim2) != HAL_OK)
        res = 1;                //记录电机转动的圈数
    if(HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1) != HAL_OK)
        res = 1; 
    if(HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3) != HAL_OK) 
        res = 1;
    if(res != 0)
        Error_Handler();
    Motor_Standby();
}
/*****************************************************************************
 Function    : Motor_Get_Cycle
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Motor_Get_Number_Of_Turns(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}
/*****************************************************************************
 Function    : Motor_Clear_Cycle
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Clear_Number_Of_Turns(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}
/*****************************************************************************
 Function    : Motor_Standby
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Standby(void)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 1000);
    Motor_Clear_Number_Of_Turns();
}
/*****************************************************************************
 Function    : Motor_Speed_Up
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
float Motor_Speed_Up(float dutyCycle)
{
    if(dutyCycle > 30.0)
    {
        return dutyCycle = dutyCycle - R_STEP;
    }
    return dutyCycle;
}
/*****************************************************************************
 Function    : Motor_Slow_Down
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
float Motor_Slow_Down(float dutyCycle)
{
    if(dutyCycle < 800.0)
    {
        return dutyCycle = dutyCycle + R_STEP;
    }
    return dutyCycle;
}
/*****************************************************************************
 Function    : Motor_Forward_Set_Duty_Cycle
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Forward_Set_Duty_Cycle(uint16_t dutyCycle)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, dutyCycle);    
}
/*****************************************************************************
 Function    : Motor_Reverse_Set_Duty_Cycle
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Reverse_Set_Duty_Cycle(uint16_t dutyCycle)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, dutyCycle);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 1000);
}
/*****************************************************************************
 Function    : Motor_Run
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Run(uint8_t dir, uint16_t dutyCycle)
{
    switch(dir)
    {
        case FORWARD:
            Motor_Forward_Set_Duty_Cycle(dutyCycle);
            break;
        case REVERSE:
            Motor_Reverse_Set_Duty_Cycle(dutyCycle);
            break;
        default :
            ;
        break;
    }
}

/*****************************************************************************
 Function    : Motor_Set_Speed
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Set_Speed(uint16_t r, MotorTypeDef *motor)
{
    if(r < (R_SPEED_UP_TH))
    {
        motor->dutyCycle = Motor_Speed_Up(motor->dutyCycle);
    }
    else if(r > (R_SLOW_DOWN_TH))
    {
        motor->dutyCycle = Motor_Slow_Down(motor->dutyCycle);
    }
}
/************************ZXDQ *****END OF FILE****/

