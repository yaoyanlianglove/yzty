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

volatile  uint16_t gear1Count    = 1;
volatile  uint16_t gear2Count    = 1;
volatile  uint16_t gear3Count    = 1;
volatile  uint16_t gear4Count    = 1;
volatile  uint16_t gear5Count    = 1;
MotorTypeDef        g_motor;
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
    if(temp == 1)
        gear = gear1 + gear2 + gear3 + gear4 + gear5;
        
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
            return GEAR_ERROR;
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
    uint8_t gear1 = 0; 
    uint8_t gear2 = 0; 
    uint8_t gear3 = 0; 
    uint8_t gear4 = 0; 
    uint8_t gear5 = 0; 
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
    if(temp == 1)
    {
        sw->currentGear = gear1 + gear2 + gear3 + gear4 + gear5;
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
SwitchStatusTypeDef Back_Gear(uint8_t r, uint8_t dir, MotorTypeDef* motor)
{
    uint16_t num   = 0;
    uint32_t count = 0;
    float speed   = 0.0;
    Motor_Clear_Number_Of_Turns();
    motor->dutyCycle = 700;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            Motor_Standby();
            return SWITCH_MOTOR_ERROR;
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    return Back_Gear(num, dir^1, motor);
                }
                else if(num > r)   //按转过的圈数返回
                {
                    Motor_Standby();
                    break;
                }
            }
        }
        Motor_Set_Speed(num, motor);
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Find_Middle_Of_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Find_Middle_Of_Gear(uint8_t dir, MotorTypeDef *motor, SwitchTypeDef *sw)
{
    uint16_t r;
    uint16_t num      = 0;
    uint16_t num1     = 0;
    uint16_t num2     = 0;
    uint16_t num3     = 0;
    uint16_t totalNum = 0;
    uint16_t delay    = 0;
    uint32_t count    = 0;
    float    speed    = 0.0;
    motor->dutyCycle  = 700;      //找中电机速度不变
    /************预期档位是0***************************/
    sw->expectGear = 0;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            Motor_Standby();
            totalNum = num;
            return Back_Gear(totalNum, dir^1, motor);
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    totalNum = num;
                    return Back_Gear(totalNum, dir^1, motor);
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    Motor_Standby();
                    totalNum = num;
                    return Back_Gear(totalNum, dir^1, motor);
                }
            }
        }
        if(Read_Gear_No_Delay() == sw->expectGear)
        {
            delay++;
            if(delay > DELAY_GEAR_REMOTE_SIGNAL)
            {
                Motor_Standby();
                break;
            } 
        }
        else
            delay = 0;
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num1 = num;
    /************预期档位是当前档位，与dir反向***************************/
    Motor_Clear_Number_Of_Turns();
    sw->expectGear = sw->currentGear;
    count = 0;
    num   = 0;
    delay = 0;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num > num1)
            {
                totalNum = num - num1;
                return Back_Gear(totalNum, dir, motor);
            }
            else if(num < num1)
            {
                totalNum = num1 - num;
                return Back_Gear(totalNum, dir^1, motor);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    if(num > num1)
                    {
                        totalNum = num - num1;
                        return Back_Gear(totalNum, dir, motor);
                    }
                    else if(num < num1)
                    {
                        totalNum = num1 - num;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    Motor_Standby();
                    if(num > num1)
                    {
                        totalNum = num - num1;
                        return Back_Gear(totalNum, dir, motor);
                    }
                    else if(num < num1)
                    {
                        totalNum = num1 - num;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                }
            }
        }
        if(Read_Gear_No_Delay() == sw->expectGear)
        {
            delay++;
            if(delay > DELAY_GEAR_REMOTE_SIGNAL)
            {
                Motor_Standby();
                break;
            } 
        }
        else
            delay = 0;
        Motor_Run(dir^1, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num2 = num;
    /************预期档位是0，继续与dir反向***************************/
    Motor_Clear_Number_Of_Turns();
    sw->expectGear = 0;
    count = 0;
    num   = 0;
    delay = 0;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num2 + num > num1)
            {
                totalNum = num2 + num - num1;
                return Back_Gear(totalNum, dir, motor);
            }
            else if(num2 + num < num1)
            {
                totalNum = num1 - num - num1;
                return Back_Gear(totalNum, dir^1, motor);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    if(num2 + num > num1)
                    {
                        totalNum = num2 + num - num1;
                        return Back_Gear(totalNum, dir, motor);
                    }
                    else if(num2 + num < num1)
                    {
                        totalNum = num1 - num - num1;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    Motor_Standby();
                    if(num2 + num > num1)
                    {
                        totalNum = num2 + num - num1;
                        return Back_Gear(totalNum, dir, motor);
                    }
                    else if(num2 + num < num1)
                    {
                        totalNum = num1 - num - num1;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                }
            }
        }
        if(Read_Gear_No_Delay() == sw->expectGear)
        {
            delay++;
            if(delay > DELAY_GEAR_REMOTE_SIGNAL)
            {
                Motor_Standby();
                break;
            } 
        }
        else
            delay = 0;
        Motor_Run(dir^1, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num3 = num;
    r = num/2;
    /************按dir方向走num/2圈，找到中点***************************/
    Motor_Clear_Number_Of_Turns();
    count = 0;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num1 + num > num2 + num3)
            {
                totalNum = num1 + num - num2 - num3;
                return Back_Gear(totalNum, dir^1, motor);
            }
            else if(num1 + num < num2 + num3)
            {
                totalNum = num2 + num3 - num2 - num;
                return Back_Gear(totalNum, dir, motor);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    if(num1 + num > num2 + num3)
                    {
                        totalNum = num1 + num - num2 - num3;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                    else if(num1 + num < num2 + num3)
                    {
                        totalNum = num2 + num3 - num2 - num;
                        return Back_Gear(totalNum, dir, motor);
                    }
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    Motor_Standby();
                    if(num1 + num > num2 + num3)
                    {
                        totalNum = num1 + num - num2 - num3;
                        return Back_Gear(totalNum, dir^1, motor);
                    }
                    else if(num1 + num < num2 + num3)
                    {
                        totalNum = num2 + num3 - num2 - num;
                        return Back_Gear(totalNum, dir, motor);
                    }
                }
            }
        }
        if(num < r)
        {               
            Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        }
        else
        {
            Motor_Standby();
            break;
        }
        delay_us(TIME_OF_ONE_CYCLE);
    }
    if(r*2 > R_OF_ON_GEAR + 5 || r*2 < R_OF_ON_GEAR - 5)
        return SWITCH_MOTOR_ERROR;
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Find_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Find_Gear(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw)
{
    uint16_t num   = 0;
    uint32_t count = 0;
    uint8_t  gear, gear1;
    uint16_t delay = 0;
    float    speed = 0.0;
    motor->dutyCycle = 700;      //找档电机速度要设置，初始为700
    /************无法预期档位，读多次相同，判断找到档位***************************/
    Motor_Clear_Number_Of_Turns();
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            gearFaultArray[sw->expectGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor);
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    return Back_Gear(num , dir^1, motor);
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    gearFaultArray[sw->expectGear] = 1;
                    Motor_Standby();
                    return Back_Gear(num , dir^1, motor);
                }
            }
        }
        gear = Read_Gear_No_Delay();
        if(gear != 0)
        {
            if(delay == 0)
                gear1 = gear;
            if(gear == gear1)
                delay++;
            else
                delay = 0;
            if(delay > DELAY_GEAR_REMOTE_SIGNAL)
            {
                Motor_Standby();
                sw->currentGear = gear;
                sw->memoryGear  = gear;
                break;
            } 
        }
        else
            delay = 0;
        Motor_Set_Speed(num, motor);
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Switch_Calibration
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Switch_Calibration(SwitchTypeDef *sw, MotorTypeDef *motor)
{
    SwitchStatusTypeDef res = SWITCH_OK;
    if(sw->currentGear == 0)
    {
        if(sw->memoryGear > 0 && sw->memoryGear < sw->totalGear/2 + 1)
            res = Find_Gear(FORWARD, motor, sw);
        else
            res = Find_Gear(REVERSE, motor, sw);
    }
    //如果档位存在，直接找中，如果档位不存在，找档成功以后执行找中
    if(res != SWITCH_OK)
        return res;
    if(sw->currentGear > 0 && sw->currentGear < 4)
    {
        return Find_Middle_Of_Gear(FORWARD, motor, sw);
    }
    else if(sw->currentGear > 3 && sw->currentGear < 6)
    {
        return Find_Middle_Of_Gear(REVERSE, motor, sw);
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
SwitchStatusTypeDef Turn_Gear(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw)
{
    uint16_t num         = 0;
    uint16_t delay       = 0;
    uint32_t count       = 0;
    uint8_t  flagGetGear = 0;
    float    speed       = 0.0;
    motor->dutyCycle     = 700;      
    /************预期档位***************************/
    if(dir == FORWARD)
    {
        sw->expectGear = sw->currentGear + 1;
    }
    else
        sw->expectGear = sw->currentGear - 1;
    Motor_Clear_Number_Of_Turns();
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            gearFaultArray[sw->expectGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor);
        }
        num = Motor_Get_Number_Of_Turns();
        if(count > MOTOR_TURN_TIME_ONE_CYCLE)
        {
            if(num > 0)
            {
                speed = count/num/1000;
                if(speed > MOTOR_TURN_OVER_SPEED)                   //超速
                {
                    motor->motorFault = 1;
                    Motor_Standby();
                    return Back_Gear(num , dir^1, motor);
                }
                else if(num > R_OF_ONE_GEAR + 5)  //超转数
                {
                    sw->gearFault = 1;
                    gearFaultArray[sw->expectGear] = 1;
                    Motor_Standby();
                    return Back_Gear(num , dir^1, motor);
                }
            }
        }
        if(flagGetGear == 0)
        {
            if(Read_Gear_No_Delay() == sw->expectGear)
            {
                delay++;
                if(delay > DELAY_GEAR_REMOTE_SIGNAL)
                {
                    flagGetGear = 1;
                    sw->currentGear = sw->expectGear;
                    sw->memoryGear  = sw->expectGear;
                    Motor_Standby();
                    return Find_Middle_Of_Gear(dir^1, motor, sw);
                }
            }
            else
                delay = 0;
        }
        Motor_Set_Speed(num, motor);
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
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
    Motor_Init(&g_motor);
    return Switch_Calibration(sw, &g_motor); 
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
        return Turn_Gear(FORWARD, &g_motor, sw);
    }
    else if(sw->motion == 2)
    {
        return Turn_Gear(REVERSE, &g_motor, sw);
    }
    return SWITCH_OK; 
}
/************************ZXDQ *****END OF FILE****/

