/**
  ******************************************************************************
  * File Name          : switch.c
  * Description        : This file contains all functions about switch.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "crc.h"
#include "fram_addr_map.h"
#include "fram.h"
#include "switch.h"
#include "switch_driver.h"
#include "delay.h"
#include "config_para.h"
#include "tmc262.h"

#define   STEP_PWM_PERIOD     1 //us 开关控制过程中，PWM的周期加上代码的执行时间(读档位要5us)，周期总为STEP_PWM_PERIOD+5  us
volatile  uint16_t gear1Count    = 1;
volatile  uint16_t gear2Count    = 1;
volatile  uint16_t gear3Count    = 1;
volatile  uint16_t gear4Count    = 1;
volatile  uint16_t gear5Count    = 1;
volatile  uint16_t gear6Count    = 1;
volatile  uint16_t gear7Count    = 1;
volatile  uint16_t gear8Count    = 1;
volatile  uint16_t gear9Count    = 1;
volatile  uint16_t gear10Count    = 1;
extern MotorCfgTypeDef     g_motorCfg;
uint8_t   gearFaultArray[32] = {0};
/*****************************************************************************
 Function    : Gear_Signal_Time_Counter
 Description : Read gear signal data.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Gear_Signal_Time_Counter(GearSignalTypeDef *gearSignal)
{
    //档位
    if(gearSignal->gear1 == REMOTE_SIGNAL_GEAR1)
    {
        if(gear1Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear1Count++;
        else
        {
            gearSignal->gear1 = REMOTE_SIGNAL_GEAR1 ^ 1;
        }
    }
    else 
        gear1Count = 0;

    if(gearSignal->gear2 == REMOTE_SIGNAL_GEAR2)
    {
        if(gear2Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear2Count++;
        else
        {
            gearSignal->gear2 = REMOTE_SIGNAL_GEAR2 ^ 1;
        }
    }
    else 
        gear2Count = 0;

    if(gearSignal->gear3 == REMOTE_SIGNAL_GEAR3)
    {
        if(gear3Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear3Count++;
        else
        {
            gearSignal->gear3 = REMOTE_SIGNAL_GEAR3 ^ 1;
        }
    }
    else 
        gear3Count = 0;

    if(gearSignal->gear4 == REMOTE_SIGNAL_GEAR4)
    {
        if(gear4Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear4Count++;
        else
        {
            gearSignal->gear4 = REMOTE_SIGNAL_GEAR4 ^ 1;
        }
    }
    else 
        gear4Count = 0;

    if(gearSignal->gear5 == REMOTE_SIGNAL_GEAR5)
    {
        if(gear5Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear5Count++;
        else
        {
            gearSignal->gear5 = REMOTE_SIGNAL_GEAR5 ^ 1;
        }
    }
    else 
        gear5Count = 0;

    if(gearSignal->gear6 == REMOTE_SIGNAL_GEAR6)
    {
        if(gear6Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear6Count++;
        else
        {
            gearSignal->gear6 = REMOTE_SIGNAL_GEAR6 ^ 1;
        }
    }
    else 
        gear6Count = 0;

    if(gearSignal->gear7 == REMOTE_SIGNAL_GEAR7)
    {
        if(gear7Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear7Count++;
        else
        {
            gearSignal->gear7 = REMOTE_SIGNAL_GEAR7 ^ 1;
        }
    }
    else 
        gear7Count = 0;

    if(gearSignal->gear8 == REMOTE_SIGNAL_GEAR8)
    {
        if(gear8Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear8Count++;
        else
        {
            gearSignal->gear8 = REMOTE_SIGNAL_GEAR8 ^ 1;
        }
    }
    else 
        gear8Count = 0;

    if(gearSignal->gear9 == REMOTE_SIGNAL_GEAR9)
    {
        if(gear9Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear9Count++;
        else
        {
            gearSignal->gear9 = REMOTE_SIGNAL_GEAR9 ^ 1;
        }
    }
    else 
        gear9Count = 0;

    if(gearSignal->gear10 == REMOTE_SIGNAL_GEAR10)
    {
        if(gear10Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear10Count++;
        else
        {
            gearSignal->gear10 = REMOTE_SIGNAL_GEAR10 ^ 1;
        }
    }
    else 
        gear10Count = 0;

}
/*****************************************************************************
 Function    : Read_Gear_No_Delay
 Description : None
 Input       : None
 Output      : None
 Return      : 0-Read no gear,0xff-Read multi gear, 1~total-Read right gear.
 *****************************************************************************/
uint8_t Read_Gear_No_Delay(void)
{
    uint8_t temp = 0;
    uint8_t gear;
    uint8_t gear1 = 0; 
    uint8_t gear2 = 0; 
    uint8_t gear3 = 0; 
    uint8_t gear4 = 0; 
    uint8_t gear5 = 0;
    uint8_t gear6 = 0;
    uint8_t gear7 = 0;
    uint8_t gear8 = 0;
    uint8_t gear9 = 0;
    uint8_t gear10 = 0;

    if(REMOTE_SIGNAL_GEAR1 == 0)
    {
        temp++;
        gear1 = 1;
    }
    if(REMOTE_SIGNAL_GEAR2 == 0)
    {
        temp++;
        gear2 = 2;
    }
    if(REMOTE_SIGNAL_GEAR3 == 0)
    {
        temp++;
        gear3 = 3;
    }
    if(REMOTE_SIGNAL_GEAR4 == 0)
    {
        temp++;
        gear4 = 4;
    }
    if(REMOTE_SIGNAL_GEAR5 == 0)
    {
        temp++;
        gear5 = 5;
    }
    if(REMOTE_SIGNAL_GEAR6 == 0)
    {
        temp++;
        gear6 = 6;
    }
    if(REMOTE_SIGNAL_GEAR7 == 0)
    {
        temp++;
        gear7 = 7;
    }
    if(REMOTE_SIGNAL_GEAR8 == 0)
    {
        temp++;
        gear8 = 8;
    }
    if(REMOTE_SIGNAL_GEAR9 == 0)
    {
        temp++;
        gear9 = 9;
    }
    //if(REMOTE_SIGNAL_GEAR10 == 0)
    //{
    //    temp++;
    //    gear10 = 10;
    //}
    
    if(temp == 1)
    {
        if(GEAR_TOTAL == 5)
        {
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9;
            if(gear == 1 || gear == 3 || gear == 5 || gear == 7 || gear == 9)
                gear = gear/2 + 1;
            else
                gear = 0;
        }
        else if(GEAR_TOTAL == 9)
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9;
        else if(GEAR_TOTAL == 17)
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9 + gear10;
    }
        
    else if(temp == 0)
        gear = 0;         //无档位
    else
        gear = 0xff;      //多档位
    return gear;
}
/*****************************************************************************
 Function    : Gear_Check
 Description : None
 Input       : sw-Switch structer, gearSignal-Gear signal structer.
 Output      : None
 Return      : Gear status.
 *****************************************************************************/
GearStatusTypeDef Gear_Check(SwitchTypeDef *sw, GearSignalTypeDef *gearSignal)
{
    uint8_t i;
    if(Read_Gear(sw, gearSignal) == GEAR_OK)
        gearFaultArray[sw->memoryGear]  = 0;
    else
        gearFaultArray[sw->memoryGear]  = 1;

    for(i = 1; i < GEAR_TOTAL + 1; i++)
    {
        if(gearFaultArray[i] == 1)
        {
            if(gearFaultArray[i - 1] == 1 || gearFaultArray[i + 1] == 1)
                return GEAR_ERROR;
        }
    }
    return GEAR_OK;
}
/*****************************************************************************
 Function    : Read_Gear
 Description : None
 Input       : sw-Switch structer, gearSignal-Gear signal structer.
 Output      : None
 Return      : Gear status.
 *****************************************************************************/
GearStatusTypeDef Read_Gear(SwitchTypeDef *sw, GearSignalTypeDef *gearSignal)
{
    uint8_t temp = 0;
    uint8_t gear = 0;
    uint8_t gear1 = 0; 
    uint8_t gear2 = 0; 
    uint8_t gear3 = 0; 
    uint8_t gear4 = 0; 
    uint8_t gear5 = 0;
    uint8_t gear6 = 0; 
    uint8_t gear7 = 0; 
    uint8_t gear8 = 0; 
    uint8_t gear9 = 0; 
    uint8_t gear10 = 0; 
    temp = 0;  
    
    if(gearSignal->gear1 == 1)
    {
        temp++;
        gear1 = 1;
    }
    if(gearSignal->gear2 == 1)
    {
        temp++;
        gear2 = 2;
    }
    if(gearSignal->gear3 == 1)
    {
        temp++;
        gear3 = 3;
    }
    if(gearSignal->gear4 == 1)
    {
        temp++;
        gear4 = 4;
    }
    if(gearSignal->gear5 == 1)
    {
        temp++;
        gear5 = 5;
    }
    if(gearSignal->gear6 == 1)
    {
        temp++;
        gear6 = 6;
    }
    if(gearSignal->gear7 == 1)
    {
        temp++;
        gear7 = 7;
    }
    if(gearSignal->gear8 == 1)
    {
        temp++;
        gear8 = 8;
    }
    if(gearSignal->gear9 == 1)
    {
        temp++;
        gear9 = 9;
    }
    //if(gearSignal->gear10 == 1)
    //{
    //    temp++;
    //    gear10 = 10;
    //}
     
    if(temp == 1)
    {
        if(GEAR_TOTAL == 5)
        {
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9;
            if(gear == 1 || gear == 3 || gear == 5 || gear == 7 || gear == 9)
                gear = gear/2 + 1;
            else
            {
                gear = 0;
                return GEAR_ERROR;
            }
        }
        else if(GEAR_TOTAL == 9)
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9;
        else if(GEAR_TOTAL == 17)
            gear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8 + gear9 + gear10;
        sw->currentGear = gear;
        return GEAR_OK;
    }
    else if(temp == 0)
    {
        sw->currentGear = 0;         //无档位
        return GEAR_ERROR;
    }
    else
    {
        sw->currentGear = 0xff;      //多档位
        return GEAR_ERROR;
    }
}
/*****************************************************************************
 Function    : Back_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Back_Gear(uint8_t dir, uint32_t backStep, SwitchStatusTypeDef stat, MotorCfgTypeDef* motorCfg)
{
    uint32_t step = 0;
    uint32_t count = 0;
    uint32_t speed = 0;
    Motor_Dir(dir);
    Motor_Config_Current_Scale(motorCfg->current_all);
    while(step < backStep)
    {
        delay_us(STEP_PWM_PERIOD + 5);     //等价读档位的时间
        speed = Motor_Speed(step);
        if(count < speed)
            count++;
        else
        {
            count = 0;
            Motor_Run();
            step++;
        } 
    }
    Motor_Config_Current_Scale(motorCfg->current_half);
    return stat;
}
/*****************************************************************************
 Function    : Go_To_Middle
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Go_To_Middle(uint8_t dir, MotorCfgTypeDef* motorCfg, SwitchTypeDef* sw)
{
    uint32_t step = 0;
    uint32_t count = 0;
    uint32_t speed = 0;
    Motor_Dir(dir);
    Motor_Config_Current_Scale(motorCfg->current_all);
    while(step < motorCfg->pulse_chutou * 2)
    {
        delay_us(STEP_PWM_PERIOD + 5);     //等价读档位的时间
        speed = Motor_Speed(10);
        if(count < speed)
            count++;
        else
        {
            count = 0;
            Motor_Run();
            step++;
        }
    }
    Motor_Config_Current_Scale(motorCfg->current_half);
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Find_Middle_Of_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Find_Middle_Of_Gear(uint8_t dir, MotorCfgTypeDef *motorCfg, SwitchTypeDef *sw)
{
    uint32_t step = 0;
    uint32_t count = 0;
    uint32_t speed = 0;
    uint8_t  rightTimes = 0;
    //目标无档位,与dir同相
    Motor_Dir(dir);
    Motor_Config_Current_Scale(motorCfg->current_all);
    sw->expectGear = 0;
    while(1)
    {
        if(step < (motorCfg->pulse_chutou * 2 * 2 + 10))
        {
            if(Read_Gear_No_Delay() == sw->expectGear)
            {
                rightTimes++;
                if(rightTimes == 3)
                    break;
            }
            else
            {
                delay_us(STEP_PWM_PERIOD);
                speed = Motor_Speed(10);
                if(count < speed)
                    count++;
                else
                {
                    count = 0;
                    Motor_Run();
                    step++;
                }   
            }
        }
        else
            return Back_Gear(dir^1, step, SWITCH_MOTOR_ERROR, motorCfg);
    }
    return Go_To_Middle(dir^1, motorCfg, sw);
}

/*****************************************************************************
 Function    : Find_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Find_Gear(uint8_t dir, MotorCfgTypeDef* motorCfg, SwitchTypeDef* sw)
{
    uint32_t step = 0;
    uint8_t gear = 0;
    uint32_t count = 0;
    uint32_t speed = 0;
    uint8_t  rightTimes = 0;
    if(dir == FORWARD)
        sw->expectGear = sw->memoryGear + 1;
    else
        sw->expectGear = sw->memoryGear - 1;
    Motor_Dir(dir);
    Motor_Config_Current_Scale(motorCfg->current_all);
    while(1)
    {
        if(step < motorCfg->pulse_zhaodang)
        {
            gear = Read_Gear_No_Delay();
            if( gear > 0 && gear < sw->totalGear + 1)
            {
                rightTimes++;
                if(rightTimes == 3)
                {
                    //找档成功
                    //故障清零,没法确定故障档位，只有在调档过程中再发现故障档位
                    gearFaultArray[sw->memoryGear] = 0;
                    gearFaultArray[sw->memoryGear + 1] = 0;
                    gearFaultArray[sw->memoryGear - 1] = 0;
                    sw->currentGear = gear;
                    sw->memoryGear  = gear;
                    break;
                }
            }
            else
            {
                delay_us(STEP_PWM_PERIOD);
                speed = Motor_Speed(step);
                if(count < speed)
                    count++;
                else
                {
                    count = 0;
                    Motor_Run();
                    step++;
                }   
            }
        }
        else
        {
            gearFaultArray[sw->expectGear] = 1;
            return Back_Gear(dir^1, step, SWITCH_GEAR_ERROR, motorCfg);
        }
    }
    return Go_To_Middle(dir, motorCfg, sw);
}
/*****************************************************************************
 Function    : Switch_Calibration
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Switch_Calibration(SwitchTypeDef *sw, MotorCfgTypeDef *motorCfg)
{
    SwitchStatusTypeDef res = SWITCH_OK;
    if(sw->currentGear == 0)
    {
        gearFaultArray[sw->memoryGear] = 1;
        if(sw->memoryGear == 1)
        {
            res = Find_Gear(FORWARD, motorCfg, sw);
        }
        else if(sw->memoryGear > 1 && sw->memoryGear < sw->totalGear)
        {
            res = Find_Gear(REVERSE, motorCfg, sw);
            if(res != SWITCH_OK)
            {
                res = Find_Gear(FORWARD, motorCfg, sw);
            }
        }
        else if(sw->memoryGear == sw->totalGear)
            res = Find_Gear(REVERSE, motorCfg, sw);
    }
    else if(sw->currentGear > 0 && sw->currentGear < sw->totalGear/2 + 2)
    {
        return Find_Middle_Of_Gear(FORWARD, motorCfg, sw);
    }
    else if(sw->currentGear > sw->totalGear/2 + 1 && sw->currentGear < sw->totalGear + 1)
    {
        return Find_Middle_Of_Gear(REVERSE, motorCfg, sw);
    }
    return res;
}
/*****************************************************************************
 Function    : Turn_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Turn_Gear(uint8_t dir, MotorCfgTypeDef* motorCfg, SwitchTypeDef* sw)
{
    uint32_t step = 0;
    uint8_t gear = 0;
    uint32_t count = 0;
    uint32_t speed = 0;
    if(dir == FORWARD)
        sw->expectGear = sw->memoryGear + 1;
    else
        sw->expectGear = sw->memoryGear - 1;
    Motor_Dir(dir);
    Motor_Config_Current_Scale(motorCfg->current_all);
    while(step < motorCfg->pulse_tiaodang)
    {
        delay_us(STEP_PWM_PERIOD + 5);     
        speed = Motor_Speed(step);
        if(count < speed)
            count++;
        else
        {
            count = 0;
            Motor_Run();
            step++;
        }
    }
    sw->memoryGear = sw->expectGear;
    gear = Read_Gear_No_Delay();
    if(gear > 0 && gear < sw->totalGear/2 + 2)
    {
        return Find_Middle_Of_Gear(FORWARD, motorCfg, sw);
    }
    else if(gear > sw->totalGear/2 + 1 && gear < sw->totalGear + 1)
    {
        return Find_Middle_Of_Gear(REVERSE, motorCfg, sw);
    }
    Motor_Config_Current_Scale(motorCfg->current_half);
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Switch_Init
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Switch_Init(SwitchTypeDef *sw, uint8_t toatlGear)
{
    sw->totalGear     = toatlGear;
    sw->motion        = 0;
    sw->lastMotion    = 0;
    Motor_Init(toatlGear);
    TMC262_Init();
    return Switch_Calibration(sw, &g_motorCfg); 
}
 /*****************************************************************************
 Function    : Switch_Control
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Switch_Control(SwitchTypeDef* sw)
{
    if(sw->motion == 1)
    {
        return Turn_Gear(FORWARD, &g_motorCfg, sw);
    }
    else if(sw->motion == 2)
    {
        return Turn_Gear(REVERSE, &g_motorCfg, sw);
    }
    return SWITCH_OK;
}
/************************ZXDQ *****END OF FILE****/

