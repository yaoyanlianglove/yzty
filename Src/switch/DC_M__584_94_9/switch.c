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
volatile  uint16_t gear6Count    = 1;
volatile  uint16_t gear7Count    = 1;
volatile  uint16_t gear8Count    = 1;
volatile  uint16_t gearACount    = 1;
volatile  uint16_t gearBCount    = 1;
volatile  uint16_t qieMotorCount    = 1;
MotorTypeDef        g_motor;
uint8_t   gearFaultArray[32] = {0};
typedef struct 
{             
    uint8_t  gear1;                    //档位：1
    uint8_t  gear2;                    //档位：2
    uint8_t  gear3;                    //档位：3
    uint8_t  gear4;                    //档位：4
    uint8_t  gear5;                    //档位：5  
    uint8_t  gear6;                    //档位：6
    uint8_t  gear7;                    //档位：7
    uint8_t  gear8;                    //档位：8
    uint8_t  gearA;                    //档位：切换
    uint8_t  gearB;                    //档位：切换
}RealGearTypeDef;
RealGearTypeDef realGear;

uint8_t XGear;//1-8的挡位不算A,B
uint8_t QieMotorFlag = 0;
/*****************************************************************************
 Function    : Gear_Signal_Time_Counter
 Description : Read gear signal data.
               这里读的是真实的挡位遥信，在Modbus通信中需要转成虚拟的遥信，因为这里
               的遥信有一个挡位切换不是所有控制器通用的
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Gear_Signal_Time_Counter(GearSignalTypeDef *gearSignal)
{
    //档位
    if(realGear.gear1 == REMOTE_SIGNAL_GEAR1)
    {
        if(gear1Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear1Count++;
        else
        {
            realGear.gear1 = REMOTE_SIGNAL_GEAR1 ^ 1;
        }
    }
    else 
        gear1Count = 0;

    if(realGear.gear2 == REMOTE_SIGNAL_GEAR2)
    {
        if(gear2Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear2Count++;
        else
        {
            realGear.gear2 = REMOTE_SIGNAL_GEAR2 ^ 1;
        }
    }
    else 
        gear2Count = 0;

    if(realGear.gear3 == REMOTE_SIGNAL_GEAR3)
    {
        if(gear3Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear3Count++;
        else
        {
            realGear.gear3 = REMOTE_SIGNAL_GEAR3 ^ 1;
        }
    }
    else 
        gear3Count = 0;

    if(realGear.gear4 == REMOTE_SIGNAL_GEAR4)
    {
        if(gear4Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear4Count++;
        else
        {
            realGear.gear4 = REMOTE_SIGNAL_GEAR4 ^ 1;
        }
    }
    else 
        gear4Count = 0;

    if(realGear.gear5 == REMOTE_SIGNAL_GEAR5)
    {
        if(gear5Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear5Count++;
        else
        {
            realGear.gear5 = REMOTE_SIGNAL_GEAR5 ^ 1;
        }
    }
    else 
        gear5Count = 0;

    if(realGear.gear6 == REMOTE_SIGNAL_GEAR6)
    {
        if(gear6Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear6Count++;
        else
        {
            realGear.gear6 = REMOTE_SIGNAL_GEAR6 ^ 1;
        }
    }
    else 
        gear6Count = 0;

    if(realGear.gear7 == REMOTE_SIGNAL_GEAR7)
    {
        if(gear7Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear7Count++;
        else
        {
            realGear.gear7 = REMOTE_SIGNAL_GEAR7 ^ 1;
        }
    }
    else 
        gear7Count = 0;

    if(realGear.gear8 == REMOTE_SIGNAL_GEAR8)
    {
        if(gear8Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear8Count++;
        else
        {
            realGear.gear8 = REMOTE_SIGNAL_GEAR8 ^ 1;
        }
    }
    else 
        gear8Count = 0;

    if(realGear.gearA == REMOTE_SIGNAL_GEARA)
    {
        if(gearACount < REMOTE_SIGNAL_SUB_DITH_TIME)
            gearACount++;
        else
        {
            realGear.gearA = REMOTE_SIGNAL_GEARA ^ 1;
        }
    }
    else 
        gearACount = 0;

    if(realGear.gearB == REMOTE_SIGNAL_GEARB)
    {
        if(gearBCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            gearBCount++;
        else
        {
            realGear.gearB = REMOTE_SIGNAL_GEARB ^ 1;
        }
    }
    else 
        gearBCount = 0;

    //电机切换成功反馈
    if(QieMotorFlag == REMOTE_SIGNAL_QIE_MOTOR)
    {
        if(qieMotorCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            qieMotorCount++;
        else
        {
            QieMotorFlag = REMOTE_SIGNAL_QIE_MOTOR ^ 1;
        }
    }
    else 
        qieMotorCount = 0; 
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
    uint8_t curGear = 0;
    uint8_t gear1 = 0; 
    uint8_t gear2 = 0; 
    uint8_t gear3 = 0; 
    uint8_t gear4 = 0; 
    uint8_t gear5 = 0;
    uint8_t gear6 = 0;
    uint8_t gear7 = 0;
    uint8_t gear8 = 0;
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
    if(temp == 1)
    {
        if(REMOTE_SIGNAL_GEARA == 1 && REMOTE_SIGNAL_GEARB == 0)
        { 
            curGear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8;
            XGear = curGear;
            if((curGear % 2) != 0)
                curGear =  curGear + 1;
        }
        else if(REMOTE_SIGNAL_GEARA == 0 && REMOTE_SIGNAL_GEARB == 1)
        { 
            curGear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8;
            XGear = curGear;
            if((curGear % 2) == 0)
                curGear =  curGear + 1;
        }
        else if(REMOTE_SIGNAL_GEARA == 1 && REMOTE_SIGNAL_GEARB == 1)
        {
            curGear =  0xff;
        }
        else if(REMOTE_SIGNAL_GEARA == 0 && REMOTE_SIGNAL_GEARB == 0)
        {
            curGear =  0;
        }
    }
    else if(temp == 0)
    {
        curGear =  0;
    }
    else
    {
        curGear =  0xff;
    }
    return curGear;
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
 Function    : Real_Gear_To_Virtual_Gear
 Description : 虚拟挡位转换为遥信挡位
 Input       : sw-Switch structer, gearSignal-Gear signal structer, stat 挡位状态
 Output      : None
 Return      : None
 *****************************************************************************/
static void Real_Gear_To_Virtual_Gear(SwitchTypeDef *sw, GearSignalTypeDef *gearSignal, uint8_t stat)
{
    uint8_t i = 0;
    uint8_t *p = (uint8_t *)gearSignal;
    //挡位故障时侯，就是实际遥信
    if(stat == 1)
    {
        gearSignal->gear1 = realGear.gear1;
        gearSignal->gear2 = realGear.gear2;
        gearSignal->gear3 = realGear.gear3;
        gearSignal->gear4 = realGear.gear4;
        gearSignal->gear5 = realGear.gear5;
        gearSignal->gear6 = realGear.gear6;
        gearSignal->gear7 = realGear.gear7;
        gearSignal->gear8 = realGear.gear8;
        gearSignal->gear9 = realGear.gearA;
        gearSignal->gear10 = realGear.gearB;
    }
    else if(stat == 0) //挡位正常时，实际挡位遥信转换为虚遥信
    {
        for(i = 0; i < GEAR_TOTAL; i++)
        {
            if(i == sw->currentGear - 1)
                *(p + i) = 1;
            else
                *(p + i) = 0;
        }
    }
}
/*****************************************************************************
 Function    : Read_Gear
 Description : None
               在读挡位后，将实际的挡位遥信转换为虚拟的挡位遥信
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
    uint8_t gear6 = 0; 
    uint8_t gear7 = 0; 
    uint8_t gear8 = 0;
    uint8_t curGear = 0;
    if(realGear.gear1 == 1)
    {
        temp++;
        gear1 = 1;
    }
    if(realGear.gear2 == 1)
    {
        temp++;
        gear2 = 2;
    }
    if(realGear.gear3 == 1)
    {
        temp++;
        gear3 = 3;
    }
    if(realGear.gear4 == 1)
    {
        temp++;
        gear4 = 4;
    }
    if(realGear.gear5 == 1)
    {
        temp++;
        gear5 = 5;
    }
     if(realGear.gear6 == 1)
    {
        temp++;
        gear6 = 6;
    }
     if(realGear.gear7 == 1)
    {
        temp++;
        gear7 = 7;
    }
     if(realGear.gear8 == 1)
    {
        temp++;
        gear8 = 8;
    }
    if(temp == 1)
    {
        if(realGear.gearA == 1 && realGear.gearB == 0)
        { 
            curGear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8;
            XGear = curGear;
            if((curGear % 2) == 0)
                sw->currentGear =  curGear + 1;
            else
                sw->currentGear =  curGear;
            Real_Gear_To_Virtual_Gear(sw, gearSignal, 0);
            return GEAR_OK;
        }
        else if(realGear.gearA == 0 && realGear.gearB == 1)
        { 
            curGear = gear1 + gear2 + gear3 + gear4 + gear5 + gear6 + gear7 + gear8;
            XGear = curGear;
            if((curGear % 2) == 0)
                sw->currentGear =  curGear;
            else
                sw->currentGear =  curGear + 1;
            Real_Gear_To_Virtual_Gear(sw, gearSignal, 0);
            return GEAR_OK;
        }
        else if(realGear.gearA == 1 && realGear.gearB == 1)
        {
            sw->currentGear = 0xff;      //多档位
            Real_Gear_To_Virtual_Gear(sw, gearSignal, 1);
            return GEAR_ERROR;
        }
        else if(realGear.gearA == 0 && realGear.gearB == 0)
        {
            sw->currentGear = 0;      //无档位
            Real_Gear_To_Virtual_Gear(sw, gearSignal, 1);
            return GEAR_ERROR;
        }
        
    }
    else if(temp == 0)
    {
        sw->currentGear = 0;         //无档位
        Real_Gear_To_Virtual_Gear(sw, gearSignal, 1);
        return GEAR_ERROR;
    }
    else
    {
        sw->currentGear = 0xff;      //多档位
        Real_Gear_To_Virtual_Gear(sw, gearSignal, 1);
        return GEAR_ERROR;
    }
		return GEAR_OK;
}
/*****************************************************************************
 Function    : Back_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Back_Gear(uint8_t r, uint8_t dir, MotorTypeDef* motor, uint8_t motorType)
{
    uint16_t num      = 0;
    uint32_t count    = 0;
    float    speed    = 0.0;

    uint16_t TIME_OF_ONE_CYCLE;
    uint16_t MOTOR_TURN_TIMEOUT;   
    float    MOTOR_TURN_OVER_SPEED;   
    if(motorType == MOTOR_X)
    {
        TIME_OF_ONE_CYCLE     = X_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = X_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = X_MOTOR_TURN_OVER_SPEED;
    }
    else
    {
        TIME_OF_ONE_CYCLE     = Q_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = Q_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = Q_MOTOR_TURN_OVER_SPEED;
    }
    Motor_Clear_Number_Of_Turns();
    motor->dutyCycle = 700;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else                              //超时
        {
            Motor_Standby();
            return SWITCH_ERROR;
        }
        num = Motor_Get_Number_Of_Turns();

        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED) //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            return SWITCH_ERROR;
        }
        else if(num > r)                 //按转过的圈数返回
        {
            Motor_Standby();
            break;
        }
        
        if(count % 200 == 0)
        {
            Motor_Set_Speed(num, motor, motorType);
        }
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    if(motor->motorFault == 1)
        return SWITCH_ERROR;
    else
        return SWITCH_GEAR_ERROR;
}
/*****************************************************************************
 Function    : Find_Middle_Of_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Find_Middle_Of_Gear(uint8_t dir, MotorTypeDef *motor, SwitchTypeDef *sw, uint8_t motorType)
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
    uint16_t TIME_OF_ONE_CYCLE;
    uint16_t MOTOR_TURN_TIMEOUT;   
    float    MOTOR_TURN_OVER_SPEED;
    uint16_t R_OF_ON_GEAR;         
    uint16_t OVER_ON_GEAR;   


    if(motorType == MOTOR_X)
    {
        TIME_OF_ONE_CYCLE     = X_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = X_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = X_MOTOR_TURN_OVER_SPEED;
        R_OF_ON_GEAR          = X_R_OF_ON_GEAR;
        OVER_ON_GEAR          = X_OVER_ON_GEAR;
    }
    else
    {
        TIME_OF_ONE_CYCLE     = Q_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = Q_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = Q_MOTOR_TURN_OVER_SPEED;
        R_OF_ON_GEAR          = Q_R_OF_ON_GEAR;
        OVER_ON_GEAR          = Q_OVER_ON_GEAR;
    }
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
            return Back_Gear(totalNum, dir^1, motor, motorType);
        }
        num = Motor_Get_Number_Of_Turns();
        
        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED)  //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            totalNum = num;
            return Back_Gear(totalNum, dir^1, motor, motorType);
        }
        else if(num > R_OF_ON_GEAR + OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            Motor_Standby();
            totalNum = num;
            return Back_Gear(totalNum, dir^1, motor, motorType);
        }
        if(motorType == MOTOR_X)
        {
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
        }
        else
        {
            if(REMOTE_SIGNAL_GEARA == 1 && REMOTE_SIGNAL_GEARB == 1)
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
        }
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num1 = num;
    delay_ms(30);
    /************预期档位是当前档位，与dir反向***************************/
    sw->expectGear = sw->currentGear;
    Motor_Clear_Number_Of_Turns();
    count = 0;
    num   = 0;
    delay = 0;
    while(1)
    {
        if(count < (1000/X_TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num > num1)
            {
                totalNum = num - num1;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num < num1)
            {
                totalNum = num1 - num;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            if(num > num1)
            {
                totalNum = num - num1;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num < num1)
            {
                totalNum = num1 - num;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        else if(num > R_OF_ON_GEAR + OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num > num1)
            {
                totalNum = num - num1;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num < num1)
            {
                totalNum = num1 - num;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        if(motorType == MOTOR_X)
        {
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
        }
        else
        {
            if(REMOTE_SIGNAL_GEARA ^ REMOTE_SIGNAL_GEARB)
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
        }
        Motor_Run(dir^1, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num2 = num;
    delay_ms(30);
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
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num2 + num < num1)
            {
                totalNum = num1 - num - num2;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            if(num2 + num > num1)
            {
                totalNum = num2 + num - num1;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num2 + num < num1)
            {
                totalNum = num1 - num - num2;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        else if(num > R_OF_ON_GEAR + OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num2 + num > num1)
            {
                totalNum = num2 + num - num1;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
            else if(num2 + num < num1)
            {
                totalNum = num1 - num - num2;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
        }
        if(motorType == MOTOR_X)
        {
            if(Read_Gear_No_Delay() == sw->expectGear && num > R_OF_ON_GEAR/2)
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
        }
        else
        {
            if(REMOTE_SIGNAL_GEARA == 1 && REMOTE_SIGNAL_GEARB == 1 && num > R_OF_ON_GEAR/2)
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
        }
        Motor_Run(dir^1, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    num3 = num;
    r = num/2;
    delay_ms(30);
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
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
            else if(num1 + num < num2 + num3)
            {
                totalNum = num2 + num3 - num1 - num;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            if(num1 + num > num2 + num3)
            {
                totalNum = num1 + num - num2 - num3;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
            else if(num1 + num < num2 + num3)
            {
                totalNum = num2 + num3 - num1 - num;
                return Back_Gear(totalNum, dir, motor, motorType);
            }
        }
        else if(num > R_OF_ON_GEAR + OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            Motor_Standby();
            if(num1 + num > num2 + num3)
            {
                totalNum = num1 + num - num2 - num3;
                return Back_Gear(totalNum, dir^1, motor, motorType);
            }
            else if(num1 + num < num2 + num3)
            {
                totalNum = num2 + num3 - num1 - num;
                return Back_Gear(totalNum, dir, motor, motorType);
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
    if(r*2 > R_OF_ON_GEAR + OVER_ON_GEAR || r*2 < R_OF_ON_GEAR - OVER_ON_GEAR)
        return SWITCH_ERROR;
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Go_To_Middle
 Description : None
 Input       : None
 Output      : None
 Return      : 0-sucess,1-failed;
 *****************************************************************************/
SwitchStatusTypeDef Go_To_Middle(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw, uint8_t motorType)
{
    uint16_t num   = 0;
    uint32_t count = 0;
    float    speed = 0.0;

    uint16_t TIME_OF_ONE_CYCLE;
    uint16_t MOTOR_TURN_TIMEOUT;   
    float    MOTOR_TURN_OVER_SPEED; 
    uint16_t R_OF_ON_GEAR;       

    if(motorType == MOTOR_X)
    {
        TIME_OF_ONE_CYCLE     = X_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = X_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = X_MOTOR_TURN_OVER_SPEED;
        R_OF_ON_GEAR          = X_R_OF_ON_GEAR;
    }
    else
    {
        TIME_OF_ONE_CYCLE     = Q_TIME_OF_ONE_CYCLE;
        MOTOR_TURN_TIMEOUT    = Q_MOTOR_TURN_TIMEOUT;
        MOTOR_TURN_OVER_SPEED = Q_MOTOR_TURN_OVER_SPEED;
        R_OF_ON_GEAR          = Q_R_OF_ON_GEAR;
    }
    Motor_Clear_Number_Of_Turns();
    motor->dutyCycle = 700;
    while(1)
    {
        if(count < (1000/TIME_OF_ONE_CYCLE)*MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            Motor_Standby();
            return SWITCH_ERROR;
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            return Back_Gear(num, dir^1, motor, motorType);
        }
        else if(num >= (R_OF_ON_GEAR/2 - 1))  //按转过的圈数返回
        {
            Motor_Standby();
            break;
        }
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(TIME_OF_ONE_CYCLE);
    }
    return SWITCH_OK;
}
/*****************************************************************************
 Function    : Turn_X_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Turn_X_Gear(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw)
{
    uint16_t num         = 0;
    uint16_t delay       = 0;
    uint32_t count       = 0;
    uint8_t  flagGetGear = 0;
    float    speed       = 0.0;
    motor->dutyCycle     = 700;   
    uint8_t expectXGear;
    /************预期档位***************************/
    if(dir == FORWARD)
    {
        expectXGear = XGear + 1;
    }
    else
        expectXGear = XGear - 1;

    Motor_Clear_Number_Of_Turns();
    while(1)
    {
        if(count < (1000/X_TIME_OF_ONE_CYCLE)*X_MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            gearFaultArray[sw->expectGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_X);
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > X_MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_X);
        }
        else if(num > X_R_OF_ONE_GEAR + X_OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            gearFaultArray[sw->expectGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_X);
        }
        if(flagGetGear == 0)
        {
            Read_Gear_No_Delay();
            if(XGear == expectXGear)
            {
                delay++;
                if(delay > DELAY_GEAR_REMOTE_SIGNAL)
                {
                    flagGetGear = 1;
                    Motor_Standby();
                    return Go_To_Middle(dir, motor, sw, MOTOR_X);
                }
            }
            else
                delay = 0;
        }
        if(count % 200 == 0)
        {
            Motor_Set_Speed(num, motor, MOTOR_X);
        }
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(X_TIME_OF_ONE_CYCLE);
    }
}
/*****************************************************************************
 Function    : Turn_Q_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Turn_Q_Gear(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw)
{
    uint16_t num         = 0;
    uint16_t delay       = 0;
    uint32_t count       = 0;
    uint8_t  flagGetGear = 0;
    float    speed       = 0.0;
    motor->dutyCycle     = 700;   
    
    uint8_t expectQGear;
    /************预期档位***************************/
    if(dir == FORWARD)
    {
        expectQGear = 1;
    }
    else
        expectQGear = 0;

    Motor_Clear_Number_Of_Turns();
    while(1)
    {
        if(count < (1000/Q_TIME_OF_ONE_CYCLE)*Q_MOTOR_TURN_TIMEOUT)
            count++;
        else  //超时
        {
            sw->gearFault = 1;
            gearFaultArray[sw->currentGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_Q);
        }
        num = Motor_Get_Number_Of_Turns();
        speed = count/(num+1)/1000;
        if(speed > Q_MOTOR_TURN_OVER_SPEED)                   //速度异常
        {
            motor->motorFault = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_Q);
        }
        else if(num > Q_R_OF_ONE_GEAR + Q_OVER_ON_GEAR)  //超转数
        {
            sw->gearFault = 1;
            gearFaultArray[sw->currentGear] = 1;
            Motor_Standby();
            return Back_Gear(num , dir^1, motor, MOTOR_Q);
        }
        if(flagGetGear == 0)
        {
            if((REMOTE_SIGNAL_GEARA == expectQGear)  && (REMOTE_SIGNAL_GEARB == (expectQGear^1)))
            {
                delay++;
                if(delay > DELAY_GEAR_REMOTE_SIGNAL)
                {
                    flagGetGear = 1;
                    sw->currentGear = sw->expectGear;
                    sw->memoryGear  = sw->expectGear;
                    Motor_Standby();
                    return Go_To_Middle(dir, motor, sw, MOTOR_Q);
                }
            }
            else
                delay = 0;
        }
        if(count % 200 == 0)
        {
            Motor_Set_Speed(num, motor, MOTOR_Q);
        }
        Motor_Run(dir, (uint16_t)(motor->dutyCycle));
        delay_us(Q_TIME_OF_ONE_CYCLE);
    }
}
/*****************************************************************************
 Function    : Motor_Select
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Select(uint8_t motorType)
{
    uint16_t waitTime = 0;
    uint8_t value = 0;
    delay_ms(500);
    MOTOR_DISABLE;
    delay_ms(200);
    if(motorType == MOTOR_Q)
    {
        value = 1;
        QIE_MOTOR_CTL_Q;
    }
    else if(motorType == MOTOR_X)
    {
        value = 0;
        QIE_MOTOR_CTL_X;
    }
    //切换继电器动作需要时间
    while(REMOTE_SIGNAL_QIE_MOTOR != value && waitTime < 1000)
    {
        delay_ms(1);
        waitTime ++;
    }
    delay_ms(1000);//切换继电器需要时间500ms延时不够
    MOTOR_ENABLE;
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
        MOTOR_DISABLE;
        return SWITCH_GEAR_ERROR;
    }
    //双电机，只找中，找不了档
    if(sw->currentGear > 0 && sw->currentGear < sw->totalGear/2 + 2)
    {
        Motor_Select(MOTOR_X);
        if(REMOTE_SIGNAL_QIE_MOTOR == 0)
            res = Find_Middle_Of_Gear(FORWARD, motor, sw, MOTOR_X);
        else
            res = SWITCH_ERROR;
    }
    else if(sw->currentGear > sw->totalGear/2 + 1 && sw->currentGear < sw->totalGear + 1)
    {
        Motor_Select(MOTOR_X);
        if(REMOTE_SIGNAL_QIE_MOTOR == 0)
            res = Find_Middle_Of_Gear(REVERSE, motor, sw, MOTOR_X);
        else
            res = SWITCH_ERROR;
    }
    if(res != SWITCH_OK)
    {
        MOTOR_DISABLE;
        return res;
    }
    if(realGear.gearA == 1)
    {
        Motor_Select(MOTOR_Q);
        if(REMOTE_SIGNAL_QIE_MOTOR == 1)
            res = Find_Middle_Of_Gear(FORWARD, motor, sw, MOTOR_Q);
        else
            res = SWITCH_ERROR;
    }
    else 
    {
        Motor_Select(MOTOR_Q);
        if(REMOTE_SIGNAL_QIE_MOTOR == 1)
            res = Find_Middle_Of_Gear(REVERSE, motor, sw, MOTOR_Q);
        else
            res = SWITCH_ERROR;
    }
   
    return res;
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
    SwitchStatusTypeDef res = SWITCH_OK;;
    sw->totalGear     = toatlGear;
    sw->motion        = 0;
    sw->lastMotion    = 0;
    Motor_Init(&g_motor);
    MOTOR_ENABLE;
    Motor_Standby();
    res = Switch_Calibration(sw, &g_motor);
    Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
    MOTOR_DISABLE;
    return res; 
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
    MOTOR_ENABLE;
    SwitchStatusTypeDef res = SWITCH_OK;
    if(sw->motion == 1)
    {
        sw->expectGear = sw->currentGear + 1;
        if(XGear%2 == 0 && realGear.gearA == 1)
        {
            Motor_Select(MOTOR_X);
            if(REMOTE_SIGNAL_QIE_MOTOR == 0)
                res = Turn_X_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
            if(res != SWITCH_OK)
            {
                Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
                MOTOR_DISABLE;
                return res;
            }
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 1 && realGear.gearA == 1) 
        {
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 0 && realGear.gearB == 1)
        {
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 1 && realGear.gearB == 1)
        {
            Motor_Select(MOTOR_X);
            if(REMOTE_SIGNAL_QIE_MOTOR == 0)
                res = Turn_X_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
            if(res != SWITCH_OK)
            {
                Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
                MOTOR_DISABLE;
                return res;
            }
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
    }
    else if(sw->motion == 2)
    {
        sw->expectGear = sw->currentGear - 1;
        if(XGear%2 == 0 && realGear.gearA == 1)
        {
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 1 && realGear.gearA == 1) 
        {
            Motor_Select(MOTOR_X);
            if(REMOTE_SIGNAL_QIE_MOTOR == 0)
                res = Turn_X_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
            
            if(res != SWITCH_OK)
            {
                Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
                MOTOR_DISABLE;
                return res;
            }
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(FORWARD, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 0 && realGear.gearB == 1)
        {
            Motor_Select(MOTOR_X);
            if(REMOTE_SIGNAL_QIE_MOTOR == 0)
                res = Turn_X_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
            if(res != SWITCH_OK)
            {
                Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
                MOTOR_DISABLE;
                return res;
            }
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
        else if(XGear%2 == 1 && realGear.gearB == 1)
        {
            Motor_Select(MOTOR_Q);
            if(REMOTE_SIGNAL_QIE_MOTOR == 1)
                res = Turn_Q_Gear(REVERSE, &g_motor, sw);
            else
                res = SWITCH_ERROR;
        }
    }
    Motor_Select(MOTOR_Q); //结束后，停在Q电机，控制继电器处于断电状态
    MOTOR_DISABLE;
    return res;
}
/************************ZXDQ *****END OF FILE****/

