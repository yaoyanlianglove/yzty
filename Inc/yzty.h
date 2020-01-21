/**
  ******************************************************************************
  * File Name          : yzty.h
  * Description        : This file provides the user's entry program.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __yzty_H
#define __yzty_H
#ifdef __cplusplus
extern "C" {
#endif


#include "switch.h"


#define     PROTECT_SUB_DITH_TIME 1       //保护告警去抖时间 
   



/**
  * @brief  Device status definition
  */
typedef struct 
{
    uint8_t initFail       :1;          //低字节
    uint8_t sampleErr      :1;
    uint8_t storeErr       :1;
    uint8_t gearFault      :1;
    uint8_t motorFault     :1;
    uint8_t powerOff       :1;
    uint8_t overCurrent    :1;
    uint8_t lowVoltage     :1;
    uint8_t highVoltage    :1;
    uint8_t lockSwitch     :1;           
    uint8_t oilPosAlarm    :1;
    uint8_t oilTemAlarm    :1;
    uint8_t turnGearFail   :1;
    uint8_t turnCapFail    :1;
                                       //高字节
}DeviceStatusTypeDef;



uint8_t YZTY_Init(void);
uint8_t YZTY_AC_Sample_Calibration(void);
void    YZTY_Time_Counter(void);
void    YZTY_Device_Status_Judge(void);
void    YZTY_Read_Clock(void);
void    YZTY_Read_Telemetry(void);
void    YZTY_Read_Oil_Temp(void);
void    YZTY_Protect_Judge(void);
void    YZTY_Lock_Judge(void);
void    YZTY_Lcd(void);
void    YZTY_Read_Gear_Signal(void);
void    YZTY_Storage(void);
void    YZTY_Hand_Judge(void);
void    YZTY_Control_Judge(void);
void    YZTY_Reset_Judge(void);
void    YZTY_Display_Logo(void);
void    YZTY_Communication(void);
void    YZTY_Comm_Init(void);
void    YZTY_Heart_Judge(void);
void    YZTY_Power_Check(void)

#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

