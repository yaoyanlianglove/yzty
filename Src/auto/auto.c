/**
  ******************************************************************************
  * File Name          : auto.c
  * Description        : This file provides code for how to auto control.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "auto.h"
#include <math.h>


volatile  uint32_t autoUpVoltageConfigCount     = 0;
volatile  uint8_t  autoUpVoltageFlag            = 0;
volatile  uint32_t autoDownVoltageConfigCount   = 0;
volatile  uint8_t  autoDownVoltageFlag          = 0;
volatile  uint32_t autoCancleCount              = 0;

volatile  uint32_t autoChangeCapaConfigCount    = 0;
volatile  uint8_t  autoChangeCapaFlag           = 0;
volatile  uint32_t autoCapaCancleCount          = 0;

volatile  uint8_t  autoControlMotion            = 0;  //0 无，1 升压，2 降压 3 调容
/*****************************************************************************
 Function    : Auto_Control
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Auto_Control(ACSampleTypeDef *sample, ConfigParaTypeDef *configPara
#ifdef FUNCTION_TURN_CAPACITY  
    , RemoteSignalTypeDef *remoteSignal
#endif
    )
{
    float uMin = 0.0;
    float uMax = 0.0;
    float ua, ub, uc;
    float uAvg = 0.0;
#ifdef FUNCTION_TURN_CAPACITY
    float s = 0.0;
#endif
    ua = sample->ua;
    ub = sample->ub;
    uc = sample->uc;
    uAvg = (ua + ub + uc)/3.0;
    uMin = ua;
    if(ub < uMin)
        uMin = ub;
    if(uc < uMin)
        uMin = uc;
    uMax = ua;
    if(ub > uMax)
        uMax = ub;
    if(uc > uMax)
        uMax = uc;
    
    if(uAvg > configPara->thDownVoltage && uMin > configPara->lockVoltageLowLimit)      
    {
        autoDownVoltageFlag = 1;
        autoUpVoltageFlag   = 0;
    }
    else if(uAvg < configPara->thUpVoltage && uMax < configPara->lockVoltageUpLimit)
    {
        autoDownVoltageFlag = 0;
        autoUpVoltageFlag   = 1;
    }
    else
    {
        autoUpVoltageFlag   = 0;
        autoDownVoltageFlag = 0;
    }
#ifdef FUNCTION_TURN_CAPACITY
    s = (sqrt(sample->pa * sample->pa + sample->qa * sample->qa) +
         sqrt(sample->pb * sample->pb + sample->qb * sample->qb) +
         sqrt(sample->pc * sample->pc + sample->qc * sample->qc)) * configPara->ctRatio;
    if(remoteSignal->gearSignal.capacity == 0)
    {
        if(s < configPara->thChangeCapacity)
            autoChangeCapaFlag = 1;
        else
            autoChangeCapaFlag = 0;
    }
    else if(remoteSignal->gearSignal.capacity == 1)
    {
        if(s > configPara->thChangeCapacity)
            autoChangeCapaFlag = 1;
        else
            autoChangeCapaFlag = 0;
    }
#endif
    return autoControlMotion;
}
/*****************************************************************************
 Function    : Auto_Control_Capa_Clear_Motion
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Auto_Control_Capa_Clear_Motion(void)
{
    autoControlMotion          = 0;
    autoChangeCapaConfigCount  = 0;
    autoCapaCancleCount        = 0;
}
/*****************************************************************************
 Function    : Auto_Control_Delay
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Auto_Control_Clear_Motion(void)
{
    autoControlMotion          = 0;
    autoUpVoltageConfigCount   = 0;
    autoDownVoltageConfigCount = 0;
    autoCancleCount            = 0;
}
/*****************************************************************************
 Function    : Auto_Control_Time_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Auto_Control_Time_Counter(uint32_t tyDelay
#ifdef FUNCTION_TURN_CAPACITY  
    , uint32_t trDelay, uint8_t currentCapa
#endif
    )
{
    if(autoUpVoltageFlag == 1)
    {
        if(autoUpVoltageConfigCount < tyDelay)
            autoUpVoltageConfigCount++;
        else
        {
            autoControlMotion = 1;
        }
    }
    else if(autoDownVoltageFlag == 1)
    {
        if(autoDownVoltageConfigCount < tyDelay)
            autoDownVoltageConfigCount++;
        else
        {
            autoControlMotion = 2;
        }
    }
    else
    {
        if(autoCancleCount < 3)
            autoCancleCount++;
        else
        {
            Auto_Control_Clear_Motion();
        }
        
    }
#ifdef FUNCTION_TURN_CAPACITY   
    if(currentCapa == 1)
        trDelay = 60;
    if(autoChangeCapaFlag == 1)
    {
        if(autoChangeCapaConfigCount < trDelay)
            autoChangeCapaConfigCount++;
        else     //调容调压同时满足，调容后判断先执行
        {
            autoControlMotion = 3;
        }
    }
    else
    {
        if(autoCapaCancleCount < 3)
            autoCapaCancleCount++;
        else
        {
            Auto_Control_Capa_Clear_Motion();
        }
    }
#endif
}
/************************ZXDQ *****END OF FILE****/

