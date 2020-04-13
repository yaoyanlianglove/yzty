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


#define     DELAY_GEAR_REMOTE_SIGNAL  1

/**
  * @brief  Motor structures definition
  */
typedef struct
{                                  
    uint8_t  pulse_chutou;            //能够正确读到挡位的脉冲宽度
    uint8_t  current_half;            //半流配置
    uint8_t  current_all;             //全流配置
    uint32_t pulse_tiaodang;          //一档对应的脉冲数
    uint32_t pulse_zhaodang;          //找挡最大脉冲数
    uint8_t  dangwei_total;           //挡位总
}MotorCfgTypeDef;


void Motor_Init(uint8_t switchType);
void Motor_Run(void);
void Motor_Dir(uint8_t dir);
uint32_t Motor_Speed(uint32_t step);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

