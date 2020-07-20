/**
  ******************************************************************************
  * File Name          : tmc262.h
  * Description        : This file contains all functions about TMC262.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TMC262_H
#define __TMC262_H
#include "main.h"
	



#define  TMC262_STEP_PORT    DOUT7_GPIO_Port
#define  TMC262_STEP_PIN     DOUT7_Pin
#define  TMC262_DIR_PORT     DOUT6_GPIO_Port
#define  TMC262_DIR_PIN      DOUT6_Pin
#define  TMC262_EN_PORT      DOUT9_GPIO_Port
#define  TMC262_EN_PIN       DOUT9_Pin

//#define  TMC262_STAT_PORT    DOUT9_GPIO_Port  //？
//#define  TMC262_STAT_PIN     DOUT9_Pin        //?


#define TMC262_STEP_LOW      HAL_GPIO_WritePin(TMC262_STEP_PORT, TMC262_STEP_PIN, GPIO_PIN_RESET)   
#define TMC262_STEP_HIGH     HAL_GPIO_WritePin(TMC262_STEP_PORT, TMC262_STEP_PIN, GPIO_PIN_SET) 
#define TMC262_DIR_LOW       HAL_GPIO_WritePin(TMC262_DIR_PORT, TMC262_DIR_PIN, GPIO_PIN_RESET)
#define TMC262_DIR_HIGH      HAL_GPIO_WritePin(TMC262_DIR_PORT, TMC262_DIR_PIN, GPIO_PIN_SET) 
#define TMC262_EN_LOW        HAL_GPIO_WritePin(TMC262_EN_PORT, TMC262_EN_PIN, GPIO_PIN_RESET)
#define TMC262_EN_HIGH       HAL_GPIO_WritePin(TMC262_EN_PORT, TMC262_EN_PIN, GPIO_PIN_SET) 


#define  TMC262_READ_STEP    HAL_GPIO_ReadPin(TMC262_STEP_PORT, TMC262_STEP_PIN)
#define  TMC262_READ_STAT    HAL_GPIO_ReadPin(TMC262_STAT_PORT, TMC262_STAT_PIN)




#define TMC262_RB_MSTEP        0    
#define TMC262_RB_STALL_GUARD  1    
#define TMC262_RB_SMART_ENERGY 2      

typedef struct __tmc262_state
{
    uint8_t dir;                    //微步的极性
    uint16_t mstep;                 //微步数
    uint16_t sg_data;               //sg的值
    uint8_t se_data;                //se的值
    uint8_t stst:1;                 //静止指示器
    uint8_t ola:1;                  //负载指示器
    uint8_t olb:1;
    uint8_t s2ga:1;                 //短路检测
    uint8_t s2gb:1;
    uint8_t otpw:1;                 //过温报警
    uint8_t ot:1;                   //过温停止
    uint8_t sgst:1;                 //是否失速的标志
    uint8_t type;                   //TMC262_RB_MSTEP 0, TMC262_RB_STALL_GUARD 1, TMC262_RB_SMART_ENERGY 2
}_tmc262_state;

 

//脉冲和方向控制配置
typedef struct __driver_control_reg
{
    uint8_t APolarity;               //rev   
    uint8_t BPolarity;               //rev   
    uint8_t NPolarity;               //rev   
    uint8_t NContinous;              //rev   
    uint8_t ClearOnN;                //rev   
    uint8_t intpol;                  //STEP信号修改的使能     
    uint8_t dedge;                   //0：STEP信号为上升沿有效，下降沿无效。 1：STEP信号为双沿有效（上升沿、下降沿都有效）
    uint8_t mres;                    //0000~1000 一共 9 种方式：256，128，64，32，16，8，4，2，1
} _driver_control_reg;


//斩波器的配置，一个斩波周期包括充电、慢衰减、快衰减、慢衰减
typedef struct __chopper_control_reg
{
    uint8_t blank_time;              //设置斩波控制的空白时间00--11，对应16--54个时钟周期。%00: 16  %01: 24  %10: 36  %11: 54
    uint8_t chopper_mode;            //斩波模式 此位对：HDEC，HEND，HSTRT参数的设置有影响。 0：斩波模式, 1: 常系数模式，一般选择0。   
    uint8_t hysteresis_decay;        //迟滞衰退
    uint8_t random_time_off;         //0：斩波时间固定       1：斩波时间可调
    uint8_t hysteresis_end;          //迟滞结束 
    uint8_t hysteresis_start;        //迟滞开始 
    uint8_t time_off;                // Off time/MOSFET  disable //off time
    uint8_t disable_flag;           
} _chopper_control_reg;


//智能节能控制
typedef struct __coolstep_control_reg
{
    uint8_t smart_i_min;              //智能最小电流    
    uint8_t smart_down_step;          //智能电流的减少速度
    uint8_t smart_stallLevel_max;     //最小电流控制时负载 的磁滞值（高位安全门限）
    uint8_t smart_up_step;            //智能电流的增加速度
    uint8_t smart_stallLevel_min;     //最小负载检测值
} _coolstep_control_reg;   


//丢步保护配置，负载检测和电流设置寄存器
typedef struct __stallguard_control_reg
{
    uint8_t filter_enable;         
    char stallguard_threshold;      //堵转监测值
    uint8_t current_scale;             //电流模式        
} _stallguard_control_reg;


//驱动程序配置
typedef struct __driver_config_reg
{
    uint8_t test_mode;                 //为测试模式保留，设置为0，当此位变成1时，SG_TST输出数字测试值,TEST_ANA输出模拟测试值，通过SGT1和SGT0进行设置
    uint8_t slope_high_side;           //高端上升沿控制，00：最小值，01：最小值+tc，10：中间值+tc 11：最大值  在温度补偿模式下，这里的设定值作为驱动强度的一个补偿。
    uint8_t slope_low_side;            //低端上升沿控制，00&01：最小值，10：中间值，11：最大值
    uint8_t protection_disable;        //短接到地保护使能，0：使能，1：非使能
    uint8_t protection_timer;          //短接到地检测定时器，00：3.2us，01：1.6us，10：1.2us，0.8us
    uint8_t step_direction_disable;    //脉冲和方向模式输入关断，0：使能脉冲和方向模式，1：使能SPI模式
    uint8_t vsense_scale;              //作为电流设定的采样电阻值边界，0：采样电压值最大305mV,1:采样电压值最大165mV
    uint8_t read_back_select;          //读输出返回值选择
} _driver_config_reg;



void TMC262_GPIO_Init(void);
void TMC262_Init(void);
void Motor_Config_Current_Scale(uint8_t current_scale);
void Motor_Config_Zhengzhuan(void);
void Motor_Config_Fanzhuan(void);
void Read_TMC262_State(uint8_t type);

#endif


/************************ZXDQ *****END OF FILE****/
