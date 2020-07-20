/**
  ******************************************************************************
  * File Name          : fram_addr_map.h
  * Description        : This file provides all data address assignments.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fram_addr_map_H
#define __fram_addr_map_H
#ifdef __cplusplus
extern "C" {
#endif

#define FRAM_64                          64        //64K大小的存储

#ifdef  FRAM_64
#define SWITCH_MOTION_RECORD_SUM         150       //开关动作最大记录条数
#define ALARM_RECORD_SUM                 50        //报警最大记录条数
#endif

#ifdef  FRAM_256
#define SWITCH_MOTION_RECORD_SUM         600       //开关动作最大记录条数
#define ALARM_RECORD_SUM                 200       //报警最大记录条数
#endif

#define FIRST_PROGRAM_FLAG_ADDR          00        //第一次烧写程序标志地址      
#define REBOOT_COUNTER_ADDR              70        //70~71 计数  72 73 校验和
       
#define CALIBRATION_INPUT_ADDR           80        //校表时输入的电流电压值保存地址80~83电压 84~87 电流 88 89 校验和  
       
#define GEAR_MEMORY_ADDR                 97        //挡位记忆存储, 98 99是档位记忆的校验和
       
#define YZTY_CFG_ADDR                    100       //有载调压参数配置 100~199  

#define CALIBRATION_DATA_ADDR            200       //校表数据开始 200~299

#define SWITCH_MOTION_COUNTER_ADDR       300       //开关动作次数开始地址300~303  304 305校验和    
#define SWITCH_MOTION_RECORD_NUM_ADDR    306       //开关动作记录存储到第几条306~307 308 309 校验和

#define ALARM_COUNTER_ADDR               310       //报警次数地址
#define ALARM_RECORD_NUM_ADDR            314       //报警记录存储到第几条


#ifdef  FRAM_64
#define SWITCH_MOTION_RECORD_ADDR        320       //开关动作记录开始地址  37*150 5550 320~5869  
#define ALARM_RECORD_ADDR                5870      //报警记录开始地址  26*50 1300 5870~7169  
#endif


#ifdef  FRAM_256
#define SWITCH_MOTION_RECORD_ADDR        320       //开关动作记录开始地址  37*600 22200 320~22519 
#define ALARM_RECORD_ADDR                22520     //报警记录开始地址  26*200 5200 22520~27719 
#endif


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

