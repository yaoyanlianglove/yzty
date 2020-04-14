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
volatile  uint16_t largeCount    = 1;
volatile  uint16_t smallCount    = 1;
volatile  uint16_t chargeCount   = 1;

volatile  uint8_t gear1IO = 0;
volatile  uint8_t gear2IO = 0;
volatile  uint8_t gear3IO = 0;
volatile  uint8_t largeIO = 0;
volatile  uint8_t smallIO = 0;
volatile  uint8_t flagCharge = 0;

uint8_t flagFakeGear_100 = 0;
uint8_t faultCount = 0;

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
    if(gear1IO == REMOTE_SIGNAL_GEAR1)
    {
        if(gear1Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear1Count++;
        else
        {
            gear1IO = REMOTE_SIGNAL_GEAR1 ^ 1;
        }
    }
    else 
        gear1Count = 0;

    if(gear2IO == REMOTE_SIGNAL_GEAR2)
    {
        if(gear2Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear2Count++;
        else
        {
            gear2IO = REMOTE_SIGNAL_GEAR2 ^ 1;
        }
    }
    else 
        gear2Count = 0;

    if(gear3IO == REMOTE_SIGNAL_GEAR3)
    {
        if(gear3Count < REMOTE_SIGNAL_SUB_DITH_TIME)
            gear3Count++;
        else
        {
            gear3IO = REMOTE_SIGNAL_GEAR3 ^ 1;
        }
    }
    else 
        gear3Count = 0;

    //容量

    if(largeIO == REMOTE_SIGNAL_LARGE)
    {
        if(largeCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            largeCount++;
        else
        {
            largeIO = REMOTE_SIGNAL_LARGE ^ 1;
        }
    }
    else 
        largeCount = 0;

    if(smallIO == REMOTE_SIGNAL_SMALL)
    {
        if(smallCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            smallCount++;
        else
        {
            smallIO = REMOTE_SIGNAL_SMALL ^ 1;
        }
    }
    else 
        smallCount = 0;

    //是否充满电
    if(flagCharge == REMOTE_SIGNAL_CHARGE)
    {
        if(chargeCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            chargeCount++;
        else
        {
            flagCharge = REMOTE_SIGNAL_CHARGE ^ 1;
        }
    }
    else 
        chargeCount = 0;
}
/*****************************************************************************
 Function    : Read_Cap_No_Delay
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Read_Cap_No_Delay(void)
{
    if(REMOTE_SIGNAL_LARGE == 0 && REMOTE_SIGNAL_SMALL == 1)
        return 0;
    else if(REMOTE_SIGNAL_SMALL == 0 && REMOTE_SIGNAL_LARGE == 1)
        return 1;
    return 2;
}
/*****************************************************************************
 Function    : Read_Gear_No_Delay
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Read_Gear_No_Delay(void)
{
    uint8_t gear;
    uint8_t gear1 = 0; 
    uint8_t gear2 = 0; 
    uint8_t gear3 = 0; 

    if(REMOTE_SIGNAL_GEAR1 == 0)
    {
        gear1 = 1;
    }
    if(REMOTE_SIGNAL_GEAR2 == 0)
    {
        gear2 = 1;
    }
    if(REMOTE_SIGNAL_GEAR3 == 0)
    {
        gear3 = 1;
    }
    if(gear1 == 0 && gear2 == 0 && gear3 == 0)
    {
        gear = 1;
    }
    else if(gear1 == 0 && gear2 == 0 && gear3 == 1)
    {
        gear = 2;
    }
    else if(gear1 == 0 && gear2 == 1 && gear3 == 0)
    {
        gear = 3;
    }
    else if(gear1 == 0 && gear2 == 1 && gear3 == 1)
    {
        flagFakeGear_100 = 0;
        gear = 4;
    }
    else if(gear1 == 1 && gear2 == 0 && gear3 == 1)
    {
        gear = 5;
    }
    else if(gear1 == 1 && gear2 == 1 && gear3 == 0)
    {
        gear = 6;
    }
    else if(gear1 == 1 && gear2 == 1 && gear3 == 1)
    {
        gear = 7;
    }
    else if(gear1 == 1 && gear2 == 0 && gear3 == 0)
    {
        flagFakeGear_100 = 1;
        gear = 4;
    }
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
    if(faultCount > 3)
    {
        return GEAR_ERROR; 
    }
    else
        return Read_Gear(sw, gearSignal);
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
    uint8_t gear = 0;
    if(gear1IO == 0 && gear2IO == 0 && gear3IO == 0)
    {
        gear = 1;
        gearSignal->gear1 = 1;
        gearSignal->gear2 = 0;
        gearSignal->gear3 = 0;
    }
    else if(gear1IO == 0 && gear2IO == 0 && gear3IO == 1)
    {
        gear = 2;
        gearSignal->gear1 = 0;
        gearSignal->gear2 = 1;
        gearSignal->gear3 = 0;
    }
    else if(gear1IO == 0 && gear2IO == 1 && gear3IO == 0)
    {
        gear = 3;
        gearSignal->gear1 = 1;
        gearSignal->gear2 = 1;
        gearSignal->gear3 = 0;
    }
    else if(gear1IO == 0 && gear2IO == 1 && gear3IO == 1)
    {
        flagFakeGear_100 = 0;
        gear = 4;
        gearSignal->gear1 = 0;
        gearSignal->gear2 = 0;
        gearSignal->gear3 = 1;
    }
    else if(gear1IO == 1 && gear2IO == 0 && gear3IO == 1)
    {
        gear = 5;
        gearSignal->gear1 = 1;
        gearSignal->gear2 = 0;
        gearSignal->gear3 = 1;
    }
    else if(gear1IO == 1 && gear2IO == 1 && gear3IO == 0)
    {
        gear = 6;
        gearSignal->gear1 = 0;
        gearSignal->gear2 = 1;
        gearSignal->gear3 = 1;
    }
    else if(gear1IO == 1 && gear2IO == 1 && gear3IO == 1)
    {
        gear = 7;
        gearSignal->gear1 = 1;
        gearSignal->gear2 = 1;
        gearSignal->gear3 = 1;
    }
    else if(gear1IO == 1 && gear2IO == 0 && gear3IO == 0)
    {
        flagFakeGear_100 = 1;
        gear = 4;
        gearSignal->gear1 = 0;
        gearSignal->gear2 = 0;
        gearSignal->gear3 = 1;
    }
    //容量
    if(largeIO == 0)
    {
        gearSignal->capacity = 0;
        sw->currentCapa = 0;
    }
    else
    {
        gearSignal->capacity = 1;
        sw->currentCapa = 1;
    }

    sw->currentGear = gear;   
    return GEAR_OK;
}
/*****************************************************************************
 Function    : Igbt_Opration_Start
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Igbt_Opration_Start(uint8_t IGBT, uint8_t dir)
{
    switch(IGBT)
    {
        case IGBT3:
            IGBT3_MOTION_HIGH;
            if(dir == FORWARD)           
                IGBT3_DIR_HIGH;
            else           
                IGBT3_DIR_LOW;
        break;
        case IGBT2fan_IGBT3zheng:
            IGBT2_MOTION_HIGH;
            IGBT3_MOTION_HIGH;
            IGBT2_DIR_LOW;
            IGBT3_DIR_HIGH;
        break;
        case IGBT2zheng_IGBT3fan:
            IGBT2_MOTION_HIGH;
            IGBT3_MOTION_HIGH;
            IGBT2_DIR_HIGH;
            IGBT3_DIR_LOW;
        break;
        case IGBT1zheng_IGBT2fan:
            IGBT1_MOTION_HIGH;
            IGBT2_MOTION_HIGH;
            IGBT1_DIR_HIGH;
            IGBT2_DIR_LOW;
        break;
        case IGBT1fan_IGBT2zheng:
            IGBT1_MOTION_HIGH;
            IGBT2_MOTION_HIGH;
            IGBT1_DIR_LOW;
            IGBT2_DIR_HIGH;
        break;
    }
}
/*****************************************************************************
 Function    : Igbt_Opration_Run
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Igbt_Opration_Run(uint8_t IGBT)
{
    switch(IGBT)
    {
        case IGBT3:
            IGBT3_MOTION_LOW;
        break;
        case IGBT2fan_IGBT3zheng:
            IGBT2_MOTION_LOW;
            IGBT3_MOTION_LOW;
        break;
        case IGBT2zheng_IGBT3fan:
            IGBT2_MOTION_LOW;
            IGBT3_MOTION_LOW;
        break;
        case IGBT1zheng_IGBT2fan:
            IGBT1_MOTION_LOW;
            IGBT2_MOTION_LOW;
        break;
        case IGBT1fan_IGBT2zheng:
            IGBT1_MOTION_LOW;
            IGBT2_MOTION_LOW;
        break;
    }
}
/*****************************************************************************
 Function    : Igbt_Opration_Stop
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Igbt_Opration_Stop(uint8_t IGBT)
{
    switch(IGBT)
    {
        case IGBT3:
            IGBT3_MOTION_HIGH;
        break;
        case IGBT2fan_IGBT3zheng:
            IGBT2_MOTION_HIGH;
            IGBT3_MOTION_HIGH;
        break;
        case IGBT2zheng_IGBT3fan:
            IGBT2_MOTION_HIGH;
            IGBT3_MOTION_HIGH;
        break;
        case IGBT1zheng_IGBT2fan:
            IGBT1_MOTION_HIGH;
            IGBT2_MOTION_HIGH;
        break;
        case IGBT1fan_IGBT2zheng:
            IGBT1_MOTION_HIGH;
            IGBT2_MOTION_HIGH;
        break;
    }
}
/*****************************************************************************
 Function    : Igbt_Opration_End
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Igbt_Opration_End(uint8_t IGBT)
{
    switch(IGBT)
    {
        case IGBT3:
            IGBT3_MOTION_HIGH;  
            IGBT3_DIR_HIGH;
        break;
        case IGBT2fan_IGBT3zheng:
            IGBT2_MOTION_HIGH;
            IGBT2_DIR_HIGH;
            IGBT3_MOTION_HIGH;
            IGBT3_DIR_HIGH;
        break;
        case IGBT2zheng_IGBT3fan:
            IGBT2_MOTION_HIGH;
            IGBT2_DIR_HIGH;
            IGBT3_MOTION_HIGH;
            IGBT3_DIR_HIGH;
        break;
        case IGBT1zheng_IGBT2fan:
            IGBT1_MOTION_HIGH;
            IGBT1_DIR_HIGH;
            IGBT2_MOTION_HIGH;
            IGBT2_DIR_HIGH;
        break;
        case IGBT1fan_IGBT2zheng:
            IGBT1_MOTION_HIGH;
            IGBT1_DIR_HIGH;
            IGBT2_MOTION_HIGH;
            IGBT2_DIR_HIGH;
        break;
    }
}
/*****************************************************************************
 Function    : Igbt_Opration
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
uint8_t Igbt_Opration(uint8_t IGBT, uint8_t dir, SwitchTypeDef* sw)           
{
    uint8_t res = 0;
    uint8_t readDCTCount = 1;
    uint8_t readGearCount = 1;
    uint8_t flagIGBTOn   = 0;
    uint8_t gear;
    uint16_t timeOutIGBT  = 0;
    uint16_t timeOutDCT   = 0;

    Igbt_Opration_Start(IGBT, dir);
    delay_ms(1000);                                                   
    TY_DIANCITIE_LOW;
    while(1)
    {
        //读电磁铁闭合信号
        if(REMOTE_SIGNAL_TY_DIANCITIE == ON)
        {
            if(readDCTCount < REMOTE_SIGNAL_SUB_DITH_TIME)
                readDCTCount++;
            else
            {
                Igbt_Opration_Run(IGBT);
                flagIGBTOn = 1;
            }
        }
        //IGBT超时关闭
        if(flagIGBTOn == 1)
        {
            gear = Read_Gear_No_Delay();
            if(gear == sw->expectGear)
            {
                if(readGearCount < REMOTE_SIGNAL_SUB_DITH_TIME)
                    readGearCount++;
                else
                {
                    faultCount = 0;
                    sw->currentGear = sw->expectGear;
                    sw->memoryGear  = sw->expectGear;
                    break;
                }
            }
            if(timeOutIGBT < IGBT_TIME_OUT)
                timeOutIGBT++;
            else
                Igbt_Opration_Stop(IGBT);
        }
        //200ms超时
        if(timeOutDCT < DCT_TIME_OUT)
            timeOutDCT++;
        else
        {
            faultCount++;
            res = 1;
            break;
        }
        delay_ms(1);
    }
    TY_DIANCITIE_HIGH;
    Igbt_Opration_End(IGBT);
    return res;   
}
/*****************************************************************************
 Function    : Turn_Cap
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Turn_Cap(SwitchTypeDef* sw)
{
    uint8_t res = 0;
    uint8_t expectCap = 0;
    uint8_t readDCTCount = 1;
    uint8_t readGearCount = 1;
    uint8_t flagIGBTOn   = 0;
    uint8_t cap;
    uint16_t timeOutIGBT  = 0;
    uint16_t timeOutDCT   = 0;

    IGBT_TR_MOTION_HIGH;
    if(sw->currentCapa == 0)
    {
        expectCap = 1;
        IGBT_TR_DIR_LOW;
    }
    else
    {
        expectCap = 0;
        IGBT_TR_DIR_HIGH;
    }
    delay_ms(1000);                                                   
    TR_DIANCITIE_LOW;
    while(1)
    {
        //读电磁铁闭合信号
        if(REMOTE_SIGNAL_TR_DIANCITIE == ON)
        {
            if(readDCTCount < REMOTE_SIGNAL_SUB_DITH_TIME)
                readDCTCount++;
            else
            {
                IGBT_TR_MOTION_LOW;
                flagIGBTOn = 1;
            }
        }
        //IGBT超时关闭
        if(flagIGBTOn == 1)
        {
            cap = Read_Cap_No_Delay();
            if(cap == expectCap)
            {
                if(readGearCount < REMOTE_SIGNAL_SUB_DITH_TIME)
                    readGearCount++;
                else
                {
                    faultCount = 0;
                    sw->currentCapa = expectCap;
                    break;
                }
            }
            if(timeOutIGBT < IGBT_TIME_OUT)
                timeOutIGBT++;
            else
                IGBT_TR_MOTION_HIGH;
        }
        //200ms超时
        if(timeOutDCT < DCT_TIME_OUT)
            timeOutDCT++;
        else
        {
            faultCount++;
            res = 1;
            break;
        }
        delay_ms(1);
    }
    TR_DIANCITIE_HIGH;
    IGBT_TR_MOTION_HIGH;
    if(res == 0)
        return SWITCH_OK;
    else
        return SWITCH_TURN_FAIL;   
}
/*****************************************************************************
 Function    : Turn_Gear
 Description : None
 Input       : None
 Output      : None
 Return      : Switch status.
 *****************************************************************************/
SwitchStatusTypeDef Turn_Gear(SwitchTypeDef* sw)
{
    uint8_t res = 0;
    switch(sw->expectGear)
    {
        case 1:       
            if(sw->currentGear == 2)          
            {
                res = Igbt_Opration(IGBT3, FORWARD, sw);      
            }
        break; 
        case 2:      
            if(sw->currentGear == 1)    
            {
                res = Igbt_Opration(IGBT3, REVERSE, sw); 
            }   
            else if(sw->currentGear == 3)
            {
                res = Igbt_Opration(IGBT2fan_IGBT3zheng, 3, sw);
            }
        break;
        case 3:       
            if(sw->currentGear == 2)    
            {
                res = Igbt_Opration(IGBT2zheng_IGBT3fan, 3, sw);
            }   
            else if(sw->currentGear == 4)
            {
                if(flagFakeGear_100 == 1)
                    res = Igbt_Opration(IGBT1fan_IGBT2zheng, 3, sw);
                else
                    res = Igbt_Opration(IGBT3, REVERSE, sw); 
            }
        break;
        case 4:       
            if(sw->currentGear == 3)    
            {
                res = Igbt_Opration(IGBT3, FORWARD, sw);   
            }   
            else if(sw->currentGear == 5)
            {
                res = Igbt_Opration(IGBT1fan_IGBT2zheng, 3, sw);
            }
        break;
        case 5:       
            if(sw->currentGear == 4)    
            {
                if(flagFakeGear_100 == 1)
                    res = Igbt_Opration(IGBT3, FORWARD, sw); 
                else
                    res = Igbt_Opration(IGBT1zheng_IGBT2fan, 3, sw);        
            }   
            else if(sw->currentGear == 6)
            {
                res = Igbt_Opration(IGBT2fan_IGBT3zheng, 3, sw);
            }
        break;
        case 6:       
            if(sw->currentGear == 5)    
            {
                res = Igbt_Opration(IGBT2zheng_IGBT3fan, 3, sw);   
            }   
            else if(sw->currentGear == 7)
            {
                res = Igbt_Opration(IGBT3, REVERSE, sw); 
            }
        break;
        case 7:       
            if(sw->currentGear == 6)    
            {
                res = Igbt_Opration(IGBT3, FORWARD, sw);   
            }   
        break;
        default:
        break; 
    }
    if(res == 0)
        return SWITCH_OK; //不存在档位错误
    else
        return SWITCH_TURN_FAIL;
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
    return SWITCH_OK; 
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
    //电容是否充满电
    if(flagCharge == 0)
        return SWITCH_OK;
    if(sw->motion == 1)
    {
        sw->expectGear = sw->currentGear + 1;
        return Turn_Gear(sw);
    }
    else if(sw->motion == 2)
    {
        sw->expectGear = sw->currentGear - 1;
        return Turn_Gear(sw);
    }
    else if(sw->motion == 3)
        return Turn_Cap(sw);
    return SWITCH_OK;
}
/************************ZXDQ *****END OF FILE****/

