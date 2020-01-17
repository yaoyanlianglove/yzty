/**
  ******************************************************************************
  * File Name          : remote_signal.h
  * Description        : This file contains all functions about remote signal.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _remote_signal_H
#define _remote_signal_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "switch.h"


#define  REMOTE_SIGNAL_OIL_POS               HAL_GPIO_ReadPin(OIL_POS_GPIO_Port, OIL_POS_Pin)
              
#define  REMOTE_SIGNAL_HAND_EN               HAL_GPIO_ReadPin(HADN_EN_GPIO_Port, HADN_EN_Pin)
#define  REMOTE_SIGNAL_HAND_UP               HAL_GPIO_ReadPin(HAND_UP_GPIO_Port, HAND_UP_Pin)
#define  REMOTE_SIGNAL_HAND_DW               HAL_GPIO_ReadPin(HAND_DOWN_GPIO_Port, HAND_DOWN_Pin)
#define  REMOTE_SIGNAL_HAND_CP               HAL_GPIO_ReadPin(HAND_CAP_GPIO_Port, HAND_CAP_Pin)

#define  REMOTE_SIGNAL_LCD_ENTER             HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define  REMOTE_SIGNAL_LCD_UP                HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)
#define  REMOTE_SIGNAL_LCD_DOWN              HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)
#define  REMOTE_SIGNAL_LCD_HOME              HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)
#define  REMOTE_SIGNAL_LCD_NEXT              HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin)
            
#define  REMOTE_SIGNAL_POWER_CHECK           HAL_GPIO_ReadPin(POWER_CHECK_GPIO_Port, POWER_CHECK_Pin)  


#define  BUTTON_CONFIRM_TIME         100       //按钮确认时间 
#define  BUTTON_HANDLE_PERIOD_TIME   200       //按钮确认时间 




/**
  * @brief  Hand button signal structures definition
  */
typedef struct 
{        
    uint8_t  en;                        //使能
    uint8_t  up;                        //升压  
    uint8_t  down;                      //降压
    uint8_t  cp;                        //调容         
}HandButtonSignalTypeDef;   
/**
  * @brief  LCD button signal structures definition
  */
typedef struct 
{        
    uint8_t  home;                       //主页
    uint8_t  next;                       //下一页
    uint8_t  up;                         //选择箭头向上
    uint8_t  down;                       //选择箭头向下
    uint8_t  enter;                      //确认
}LCDButtonSignalTypeDef;    
/**
  * @brief  Remote signal structures definition
  */
typedef struct 
{        
    uint8_t  autoMode;                   //自动
    uint8_t  remoteMode;                 //遥控           
    GearSignalTypeDef gearSignal;        //档位遥信   
    uint8_t  capacity;                   //容量    0 大容量 1 小容量
    uint8_t  lockSwitch;                 //闭锁开关 
    uint8_t  initFail;                   //初始化失败           
    uint8_t  oilPosAlarm;                //油位告警
    uint8_t  oilTemAlarm;                //油温告警
    uint8_t  powerOffAlarm;              //失电告警   闭锁  
    uint8_t  overCurrentAlarm;           //过流告警
    uint8_t  lowVoltageAlarm;            //低压告警
    uint8_t  highVoltageAlarm;           //高压告警
    uint8_t  gearFault;                  //档位故障
    uint8_t  motorFault;                 //电机故障
    uint8_t  turnGearFail;               //调档失败
    uint8_t  turnCapFail;                //调容失败

    uint8_t  sampleErr;                  //采样错误
    uint8_t  storeErr;                   //存储错误
 
    LCDButtonSignalTypeDef lcdButton;    //LCD按钮结构体
    HandButtonSignalTypeDef handButton;  //手动按钮结构体
}RemoteSignalTypeDef;

void     Remote_Signal_Init(RemoteSignalTypeDef *remoteSignal);
void     Remote_Signal_Time_Counter(RemoteSignalTypeDef *remoteSignal);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

