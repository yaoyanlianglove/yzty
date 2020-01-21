/**
  ******************************************************************************
  * File Name          : switch.h
  * Description        : This file contains all functions about switch.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _switch_H
#define _switch_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "motor.h"
#define  REMOTE_SIGNAL_GEAR1       HAL_GPIO_ReadPin(DIN1_GPIO_Port, DIN1_Pin)
#define  REMOTE_SIGNAL_GEAR2       HAL_GPIO_ReadPin(DIN2_GPIO_Port, DIN2_Pin)
#define  REMOTE_SIGNAL_GEAR3       HAL_GPIO_ReadPin(DIN3_GPIO_Port, DIN3_Pin)
#define  REMOTE_SIGNAL_GEAR4       HAL_GPIO_ReadPin(DIN4_GPIO_Port, DIN4_Pin)
#define  REMOTE_SIGNAL_GEAR5       HAL_GPIO_ReadPin(DIN5_GPIO_Port, DIN5_Pin)

#define  REMOTE_SIGNAL_SUB_DITH_TIME 10        //遥信去抖时间 


/**
  * @brief  Gear status structures definition
  */
typedef enum
{
    GEAR_OK                      = 0x00U,
    GEAR_ERROR                   = 0x01U
} GearStatusTypeDef;
/**
  * @brief  Switch status structures definition
  */
typedef enum
{
    SWITCH_OK                    = 0x00U,
    SWITCH_GEAR_ERROR            = 0x01U,
    SWITCH_MOTOR_ERROR           = 0x02U
} SwitchStatusTypeDef; 

/**
  * @brief  Switch motion definition
  */
typedef enum
{
    RC_NONE                      = 0x00U,
    RC_UP_VOL                    = 0x01U,
    RC_DOWN_VOL                  = 0x02U,
    RC_TURN_CAPA                 = 0x03U
} SwitchMotionTypeDef;
/**
  * @brief  Gear signal status structures definition
  */
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
    uint8_t  gear9;                    //档位：9
    uint8_t  gear10;                   //档位：10
    uint8_t  gear11;                   //档位：11
    uint8_t  gear12;                   //档位：12
    uint8_t  gear13;                   //档位：13
    uint8_t  gear14;                   //档位：14
    uint8_t  gear15;                   //档位：15
    uint8_t  gear16;                   //档位：16
    uint8_t  gear17;                   //档位：17       
}GearSignalTypeDef;  
/**
  * @brief  Switch structures definition
  */
typedef struct 
{
    uint8_t  currentGear;         //当前档位
    uint8_t  memoryGear;          //记忆档位
    uint8_t  expectGear;          //预期档位
    uint8_t  beforeMotionGear;    //动作前档位
    uint8_t  totalGear;           //总档位
    uint8_t  beforeMotionCapa;    //动作前容量
    uint8_t  currentCapa;         //当前容量
    uint8_t  autoMotion;          //0 没动作， 1 升压 2 降压 3 调容 
    uint8_t  remoteMotion;        //0 没动作， 1 升压 2 降压 3 调容 
    uint8_t  lcdMotion;           //0 没动作， 1 升压 2 降压 3 调容 
    uint8_t  handMotion;          //0 没动作， 1 升压 2 降压 3 调容 
    uint8_t  motion;              //0 没动作， 1 升压 2 降压 3 调容  
    uint8_t  lastMotion;          //上一次动作
    uint8_t  gearFault;           //开关状态 0 正常，1 故障
    uint8_t  resetFlag;           //重启标志
}SwitchTypeDef;


void     Gear_Signal_Time_Counter(GearSignalTypeDef *gearSignal);
GearStatusTypeDef    Read_Gear(SwitchTypeDef* sw, GearSignalTypeDef* gearSignal);
GearStatusTypeDef    Gear_Check(SwitchTypeDef *sw, GearSignalTypeDef *gearSignal);
SwitchStatusTypeDef  Switch_Calibration(SwitchTypeDef* sw, MotorTypeDef* motor);
SwitchStatusTypeDef  Switch_Init(SwitchTypeDef* sw, uint8_t toatlGear);
SwitchStatusTypeDef  Switch_Control(SwitchTypeDef* sw);
SwitchStatusTypeDef  Go_To_Middle(uint8_t dir, MotorTypeDef* motor, SwitchTypeDef* sw);


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

