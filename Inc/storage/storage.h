/**
  ******************************************************************************
  * File Name          : storage.h
  * Description        : This file provides functions about fram operations.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __storage_H
#define __storage_H
#ifdef __cplusplus
extern "C" {
#endif



#include "fram.h"
#include "ac_sample.h"
#include "config_para.h"
#include "clock.h"
#include "switch.h"

#define SWITCH_MOTION_RECORD_PER_LEN      37                 //每条动作记录的长度

#define ALARM_RECORD_PER_LEN              26                 //每条报警记录的长度

#define FIRST_PROGRAM_FLAG                0xaa               //第一次烧程序标志

/**
  * @brief  Storage status structures definition
  */
typedef enum
{
    STORAGE_OK                      = 0x00U,
    STORAGE_WRITE_ERROR             = 0x01U,
    STORAGE_READ_ERROR              = 0x02U,
} StorageStatusTypeDef;
/**
  * @brief  Switch structures definition
  */
typedef struct 
{
    uint8_t  switchMotionFlag;
    uint8_t  alarmFlag;
}SaveFlagTypeDef;

StorageStatusTypeDef Save_AC_Sample_Calibration_Para(ACSampleCalibrationParaTypeDef *calPara);
StorageStatusTypeDef Save_AC_Sample_Calibration_Input(ACSampleCalibrationInputTypeDef *input);
StorageStatusTypeDef Save_Config_Para(ConfigParaTypeDef *cfgPara);
StorageStatusTypeDef Save_Memory_Gear(uint8_t gear);
StorageStatusTypeDef Save_Reboot_Counter(void);
StorageStatusTypeDef Save_Switch_Motion(ClockTypeDef          *clock, 
                                        ACSampleTypeDef       *sample,
                                        ACSampleTypeDef       *beforeMotionSample,
                                        SwitchTypeDef         *sw,
                                        uint8_t               mode);
StorageStatusTypeDef Save_Alarm(ClockTypeDef          *clock,
                                ACSampleTypeDef       *sample,
                                SwitchTypeDef         *sw,
                                uint16_t              lds,
                                uint16_t              cds,
                                uint8_t               mode);
StorageStatusTypeDef Save_Default_Config(ConfigParaTypeDef *cfgPara);
StorageStatusTypeDef Save_First_Program_Flag(void);

StorageStatusTypeDef Read_AC_Sample_Calibration_Para(ACSampleCalibrationParaTypeDef *calPara);
StorageStatusTypeDef Read_AC_Sample_Calibration_Input(ACSampleCalibrationInputTypeDef *input);
StorageStatusTypeDef Read_Config_Para(ConfigParaTypeDef *cfgPara);
StorageStatusTypeDef Read_Memory_Gear(uint8_t *data);
StorageStatusTypeDef Read_Reboot_Counter(uint8_t *data);
StorageStatusTypeDef Read_Switch_Motion_Counter(uint8_t *data);
StorageStatusTypeDef Read_Switch_Motion_Num(uint8_t *data);
StorageStatusTypeDef Read_Alarm_Counter(uint8_t *data);
StorageStatusTypeDef Read_Alarm_Num(uint8_t *data);
StorageStatusTypeDef Read_First_Program_Flag(uint8_t *data);

StorageStatusTypeDef Read_One_Rec(uint16_t addr, uint8_t *data, uint8_t length);
StorageStatusTypeDef Clear_Data(uint16_t addr, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

