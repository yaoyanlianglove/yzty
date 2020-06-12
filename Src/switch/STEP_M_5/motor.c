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
#include "delay.h"
#include "tmc262.h"
MotorCfgTypeDef g_motorCfg;
/*****************************************************************************
 Function    : Motor_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Init(uint8_t switchType)
{
    switch(switchType)
    {
        case 5:
            g_motorCfg.dangwei_total  = 5;
            g_motorCfg.pulse_tiaodang = 12000;
            g_motorCfg.pulse_zhaodang = 12608;
            g_motorCfg.current_half   = 5;
            g_motorCfg.current_all    = 22;
            g_motorCfg.pulse_chutou   = 36;
        break;
        case 9:
            g_motorCfg.dangwei_total  = 9;
            g_motorCfg.pulse_tiaodang = 4800;
            g_motorCfg.pulse_zhaodang = 5040;
            g_motorCfg.current_half   = 5;
            g_motorCfg.current_all    = 28;
            g_motorCfg.pulse_chutou   = 30;
        break;
        case 17:
            g_motorCfg.dangwei_total  = 17;
            g_motorCfg.pulse_tiaodang = 4800;
            g_motorCfg.pulse_zhaodang = 5040;
            g_motorCfg.current_half   = 10;
            g_motorCfg.current_all    = 28;
            g_motorCfg.pulse_chutou   = 30;
        break;
    }
}
/*****************************************************************************
 Function    : Motor_Dir
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Dir(uint8_t dir)
{
    if(dir == FORWARD)
        Motor_Config_Zhengzhuan();
    else if(dir == REVERSE)
        Motor_Config_Fanzhuan();
}
/*****************************************************************************
 Function    : Motor_Run
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Run(void)
{
    if(TMC262_READ_STEP == 0)
        TMC262_STEP_HIGH;
    else
        TMC262_STEP_LOW;
}
/*****************************************************************************
 Function    : Motor_Speed
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint32_t Motor_Speed(uint32_t step)
{
    if(step < 40)
        return 2000/7;  //500Hz
    else if(step < 80 && step >= 40)
        return 2000/20;   //1500Hz
    else if(step < 120 && step >= 80)
        return 2000/40;   //3200Hz
    else if(step < 160 && step >= 120)
        return 2000/60;   //5000Hz
    else if(step < (g_motorCfg.pulse_tiaodang - 160) && step >= 160)
        return 2000/90;   //7200Hz
    else if(step < (g_motorCfg.pulse_tiaodang - 120) && (step >= g_motorCfg.pulse_tiaodang - 160))
        return 2000/60; 
    else if(step < (g_motorCfg.pulse_tiaodang - 80) && (step >= g_motorCfg.pulse_tiaodang - 120))
        return 2000/40;   
    else if(step < (g_motorCfg.pulse_tiaodang - 40) && (step >= g_motorCfg.pulse_tiaodang - 80))
        return 2000/20;
    else if(step < g_motorCfg.pulse_tiaodang && step >= (g_motorCfg.pulse_tiaodang - 40))
        return 2000/7;
    return 2000/7;
}
/************************ZXDQ *****END OF FILE****/

