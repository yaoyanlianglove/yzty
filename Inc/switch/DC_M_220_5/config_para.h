/**
  ******************************************************************************
  * File Name          : config_para.h
  * Description        : This file contains all configuration related functions.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __config_para_H
#define __config_para_H
#ifdef __cplusplus
extern "C" {
#endif

#include  "stdint.h"

#define   SWITCH_TYPE      02    //直流电机222转5档


#define   CONFIG_PARA_DATA_LEN        32
//#define   FUNCTION_TURN_CAPACITY      1    //1具有调容功能 0 无调容功能 
#define   HEART_TIMEOUT_PERIOD        180  //秒

#define   GEAR_TOTAL                  5    //开关总档位
#define   TRAN_TAP_WIDE_HIGH          1000 //调节上限1000/10000=10%
#define   TRAN_TAP_WIDE_LOW           1000 //调节下限1000/10000=10%


/**
  * @brief  Device info structures definition
  */
typedef struct 
{
    uint16_t tapTotalNum;                //总档数
    uint16_t tapWideHigh;                //调压范围高
    uint16_t tapWideLow;                 //调压范围低
    uint16_t switchType;
    uint16_t hardVersion;
    uint16_t softVersion;
}DeviceInfoTypeDef;

/**
  * @brief  Config parameter structures definition
  */
typedef struct 
{
    //电气参数
    uint16_t thDownVoltage;              //降压阈值
    uint16_t thUpVoltage;                //升压阈值
    uint16_t tySpace;                    //调压间隔
    uint16_t tyDelay;                    //调压延时
    uint16_t ctRatio;                    //CT变比
    uint16_t lockOverCurrent;            //过流闭锁阈值
    uint16_t lockVoltageUpLimit;         //调压闭锁上限
    uint16_t lockVoltageLowLimit;        //调压闭锁下限
    uint16_t lockHighVoltage;            //高压闭锁阈值
    uint16_t lockLowVoltage;             //低压闭锁阈值
    uint16_t thChangeCapacity;           //调容阈值
    uint16_t ccDelay;                    //调容延时
    uint16_t thAlarmTemp;                //温度告警阈值
    //变压器参数
    uint16_t tranCapacity;               //变压器容量
    //设备参数
    uint16_t code;                       //设备编号
    uint16_t crc; 
    DeviceInfoTypeDef  deviceInfo;       //设备固定信息  
      
}ConfigParaTypeDef;



#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

