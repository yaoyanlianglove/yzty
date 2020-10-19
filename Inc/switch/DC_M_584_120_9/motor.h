/**
  ******************************************************************************
  * File Name          : motor.h
  * Description        : This file contains all functions about motor.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _motor_H
#define _motor_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
    
#define     FORWARD   0
#define     REVERSE   1  

#define     MOTOR_ENABLE  HAL_GPIO_WritePin(DOUT8_GPIO_Port, DOUT8_Pin, GPIO_PIN_SET)
#define     MOTOR_DISABLE HAL_GPIO_WritePin(DOUT8_GPIO_Port, DOUT8_Pin, GPIO_PIN_RESET)
#define     MOTOR_X    0
#define     MOTOR_Q    1
//选择电机参数
#define     X_R_STEP               10.0f
#define     X_R_SPEED_UP_TH        34
#define     X_R_SLOW_DOWN_TH       550
#define     X_R_OF_ONE_GEAR        584      //一档的转数
#define     X_R_OF_ON_GEAR         22       //在档位上的转数
#define     X_OVER_ON_GEAR         5        //比22大5说明超转数未找到挡位
#define     X_TIME_OF_ONE_CYCLE    1        //电机运行的周期延时1us  代码固有周期3.5us：电机控制while(1)的周期 

#define     X_MOTOR_TURN_TIMEOUT        4000
#define     X_MOTOR_TURN_OVER_SPEED     5.0 //
//切换电机参数
#define     Q_R_STEP               8.0f
#define     Q_R_SPEED_UP_TH        24
#define     Q_R_SLOW_DOWN_TH       100
#define     Q_R_OF_ONE_GEAR        120      //一档的转数
#define     Q_R_OF_ON_GEAR         10       //在档位上的转数
#define     Q_OVER_ON_GEAR         3        //比10大3说明超转数未找到挡位
#define     Q_TIME_OF_ONE_CYCLE    1        //电机运行的周期延时1us  代码固有周期3.5us：电机控制while(1)的周期 

#define     Q_MOTOR_TURN_TIMEOUT        2000
#define     Q_MOTOR_TURN_OVER_SPEED     5.0 //


#define     DELAY_GEAR_REMOTE_SIGNAL  1

/**
  * @brief  Motor structures definition
  */
typedef struct 
{
    float    dutyCycle;            //当前档位
    uint8_t  motorFault;           //记忆档位
}MotorTypeDef;

void     Motor_Init(MotorTypeDef* motor);
void     Motor_Clear_Number_Of_Turns(void);
void     Motor_Standby(void);
uint16_t Motor_Get_Number_Of_Turns(void);
void     Motor_Set_Speed(uint16_t r, MotorTypeDef* motor, uint8_t motorType);
void     Motor_Run(uint8_t dir, uint16_t dutyCycle);


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

