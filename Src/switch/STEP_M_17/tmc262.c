/**
  ******************************************************************************
  * File Name          : tmc262.c
  * Description        : This file contains all functions about motor.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "tmc262.h"
#include "spi_tmc262.h"
#include <stdio.h>
#include <string.h>  

//Variablen
static _driver_control_reg     driver_control_reg;            //Softwarekopien der DRVCTRL-Register aller Treiberchips
static _chopper_control_reg    chopper_control_reg;           //Softwarekopien der CHOPCONF-Register aller Treiberchips
static _coolstep_control_reg   coolstep_control_reg;          //Softwarekopien der SMARTEN-Register aller Treiberchips
static _stallguard_control_reg stallguard_control_reg;        //Softwarekopien der SGSCONF-Register aller Treiberchips
static _driver_config_reg      driver_config_reg;             //Softwarekopien der DRVCONF-Register aller Treiberchips
_tmc262_state           tmc262_state;


/*****************************************************************************
 Function    : TMC262_GPIO_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void TMC262_GPIO_Init(void)
{
    TMC262_EN_LOW;
    TMC262_DIR_HIGH;
    TMC262_STEP_HIGH;
}
/*****************************************************************************
 Function    : Driver_Control_REG_Config
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static uint32_t Driver_Control_REG_Config(void)
{
    uint32_t spi_read_data;       
    uint32_t spi_write_data; 
    spi_write_data = 0;
    spi_read_data  = 0;
    /*************SDOFF=0,STEPDIR模式，做使能和微步精度的配置。*************/
    if (driver_control_reg.intpol) 
        spi_write_data |= 1 << 9;                  //0：失能。1：使能脉冲修改
    if (driver_control_reg.dedge) 
        spi_write_data |= 1 << 8;                  //1：使能双边沿脉冲。减少步进频率要求
    if (driver_control_reg.mres > 15) 
        driver_control_reg.mres = 15;               //微步距进度配置
    spi_write_data |= driver_control_reg.mres;

    spi_read_data = TMC262_Read_Write(spi_write_data);
    return spi_read_data;
}
/*****************************************************************************
 Function    : Chopper_Control_REG_Config
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static uint32_t Chopper_Control_REG_Config(void)
{
    uint32_t spi_read_data;       
    uint32_t spi_write_data; 
    spi_write_data = 0;
    spi_read_data  = 0;
    if (chopper_control_reg.blank_time > 3) 
        chopper_control_reg.blank_time = 3;         //空白时间选择。00：15，01：24，10：36，11：54.个时钟周期          
    if (chopper_control_reg.hysteresis_decay > 3) 
        chopper_control_reg.hysteresis_decay = 3;   //迟滞损耗，和快衰减模式。
    if (chopper_control_reg.hysteresis_end > 15) 
        chopper_control_reg.hysteresis_end = 15;    //迟滞低位值或者正弦波偏置0000~1111：-3，~12 迟滞。
    if (chopper_control_reg.hysteresis_start > 7) 
        chopper_control_reg.hysteresis_start = 7;   //迟滞开始值，或快衰时间设置
    if (chopper_control_reg.time_off > 15) 
        chopper_control_reg.time_off = 15;          //OFF TIME 和驱动使能，1：TBL最小24个时钟周期。0010~1111：2~15                  

    spi_write_data |= 1 << 19;                      //Registeraddresse CHOPCONF; 
    spi_write_data |= ((uint32_t) chopper_control_reg.blank_time) << 15;
    if (chopper_control_reg.chopper_mode) 
        spi_write_data |= 1 << 14;                  //斩波模式 0：标准模式。1：time_off为常数的快衰减模式。
    if (chopper_control_reg.random_time_off) 
        spi_write_data |= 1 << 13;                  //随机time_off时间，0：斩波时间固定 1：斩波时间可调。
    spi_write_data |= ((uint32_t) chopper_control_reg.hysteresis_decay) << 11;//
    spi_write_data |= ((uint32_t) chopper_control_reg.hysteresis_end) << 7;
    spi_write_data |= ((uint32_t) chopper_control_reg.hysteresis_start) << 4;
    if (!chopper_control_reg.disable_flag) 
        spi_write_data |= ((uint32_t) chopper_control_reg.time_off);  //wenn DisableFlag gesetzt wird 0 gesendet如果DisableFlag设置为0发送

    spi_read_data = TMC262_Read_Write(spi_write_data);
    return spi_read_data;
}
/*****************************************************************************
 Function    : Driver_Config_REG_Config
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static uint32_t Driver_Config_REG_Config(void)
{
    uint32_t spi_read_data;       
    uint32_t spi_write_data; 
    spi_write_data = 0;
    spi_read_data  = 0;
    spi_write_data |= (1 << 19) | (1 << 18) | (1 << 17);                         //Registeradresse DRVCONF
    spi_write_data |= ((uint32_t) driver_config_reg.slope_high_side & 0x03) << 14;    //高端上升沿控制 00：最小值。01：最小值+tc。10：中间值+tc
    spi_write_data |= ((uint32_t) driver_config_reg.slope_low_side & 0x03) << 12;     //低端上升沿控制 00&01：最小值。10：中间值 11：最大值
    if (driver_config_reg.protection_disable == 1) 
        spi_write_data |= 1 << 10;                                               //短接到地保护使能0：使能 1：失能
    spi_write_data |=((uint32_t) driver_config_reg.protection_timer & 0x03) << 8;     //短接到地检测定时器
    if (driver_config_reg.step_direction_disable == 1) 
        spi_write_data |= 1 << 7;                                                //脉冲和方向模式输入0：使能脉冲和方向模式 1：使能SPI模式
    if (driver_config_reg.vsense_scale == 1) 
        spi_write_data |= 1 << 6;                                                //作为电流设定的采样电阻值边界  0：采样电压值最大305mV,1：采样电压值最大165mV
    
    /*****读输出选择值  00：返回微步位置 01：返回负载值 10：读回负载和智能电流级别 11：保留未使用******/
    spi_write_data |= ((uint32_t) driver_config_reg.read_back_select & 0x03) << 4;
    if (driver_config_reg.test_mode == 1) 
        spi_write_data |= 1 << 16;

    spi_read_data = TMC262_Read_Write(spi_write_data);
    return spi_read_data;
}
/*****************************************************************************
 Function    : Coolstep_Control_REG_Config
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static uint32_t Coolstep_Control_REG_Config(void)
{
    uint32_t spi_read_data;       
    uint32_t spi_write_data; 
    if (coolstep_control_reg.smart_i_min > 1) 
        coolstep_control_reg.smart_i_min = 1;                                     //智能电流控制的最小电流，0：设置电流的1/2。1：设置电流的1/4               
    if (coolstep_control_reg.smart_down_step > 3) 
        coolstep_control_reg.smart_down_step = 3;                                 //电流减小速度。         
    if (coolstep_control_reg.smart_stallLevel_max > 15) 
        coolstep_control_reg.smart_stallLevel_max = 15;                           //最小电流控制时负载的磁滞值
    if (coolstep_control_reg.smart_up_step > 3) 
        coolstep_control_reg.smart_up_step = 3;                                   //电流上升幅度           
    if (coolstep_control_reg.smart_stallLevel_min > 15) 
        coolstep_control_reg.smart_stallLevel_min = 15;                           //最小负载检测值和电流使能

    spi_write_data = 0;
    spi_read_data  = 0;
    spi_write_data |= (1 << 19) | (1 << 17);       //Registeradresse SMARTEN  寄存器地址1010
    spi_write_data |= ((uint32_t) coolstep_control_reg.smart_i_min) << 15;
    spi_write_data |= ((uint32_t) coolstep_control_reg.smart_down_step)  << 13;
    spi_write_data |= ((uint32_t) coolstep_control_reg.smart_stallLevel_max) << 8;
    spi_write_data |= ((uint32_t) coolstep_control_reg.smart_up_step) << 5;
    spi_write_data |= ((uint32_t) coolstep_control_reg.smart_stallLevel_min);

    
    spi_read_data = TMC262_Read_Write(spi_write_data);
    return spi_read_data;
}
/*****************************************************************************
 Function    : Stallguard_Control_REG_Config
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static uint32_t Stallguard_Control_REG_Config(void)
{
    uint32_t spi_read_data;       
    uint32_t spi_write_data; 
    if (stallguard_control_reg.stallguard_threshold > 63) 
        stallguard_control_reg.stallguard_threshold = 63; //阀值设定
    if (stallguard_control_reg.current_scale > 31) 
        stallguard_control_reg.current_scale = 31;        //电流标准                            

  	spi_write_data = 0;
    spi_read_data  = 0;
  	spi_write_data |= (1 << 19) | (1 << 18);              //Registeradresse SGSCONF
  	if (stallguard_control_reg.filter_enable == 1) 
        spi_write_data |= 1 << 16;
  	spi_write_data |= ((uint32_t) stallguard_control_reg.stallguard_threshold & 0x7f) << 8;
  	spi_write_data |= ((uint32_t) stallguard_control_reg.current_scale);

  	spi_read_data = TMC262_Read_Write(spi_write_data);
    return spi_read_data;
}
/*****************************************************************************
 Function    : TMC262_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void TMC262_Init(void)
{

    //负载检测和电流设置寄存器初始化
    stallguard_control_reg.filter_enable        = 1;      /* 失步检测滤波0:标准模式，1:4个fullstep滤波*/
    stallguard_control_reg.stallguard_threshold = 0;      /* 带符号失步门限-64~63，值越大灵敏度越低，不建议小于-10*/
    stallguard_control_reg.current_scale        = 10;     /* 驱动电流0~31*/

    //驱动配置寄存器初始化
    driver_config_reg.slope_high_side           = 3;      /* 上功率管控制斜率0:最小3: 最大*/
    driver_config_reg.slope_low_side            = 3;      /* 下功率管控制斜率0:最小3: 最大*/
    driver_config_reg.protection_disable        = 0;      /* 0:短路保护使能1:不使能*/
    driver_config_reg.protection_timer          = 0;	  /* 短路保护时间0: 3.2uS, 1: 1.6uS, 2: 1.2uS, 3: 0.8uS*/
    driver_config_reg.step_direction_disable    = 0;      /* 0: 脉冲方向模式，1:SPI模式*/
    driver_config_reg.vsense_scale              = 0;      /* 检测参考电压，0: 305mV, 1: 165mV */
    driver_config_reg.read_back_select          = 1;      /* 读取返回项,0:微步,1:负载值,2:负载值高5位及设置电流大小*/

    //智能节能配置寄存器初始化
    coolstep_control_reg.smart_i_min            = 0;      /* 智能控制最小电流0: 1/2, 1: 1/4*/
    coolstep_control_reg.smart_down_step        = 1;      /* 电流减少速度0: 32, 1: 8, 2: 2, 3: 1*/
    coolstep_control_reg.smart_stallLevel_max   = 0;      /* 高位安全门限0~15*/
    coolstep_control_reg.smart_up_step          = 0;      /* 电流增加速度0: 1, 1: 2, 2: 4, 3: 8*/
    coolstep_control_reg.smart_stallLevel_min   = 0;      /* 低位安全门限0~15,0为关闭智能动态控制*/


    //SDOFF=0时用于STEPDIR模式时，做使能和微步精度的配置。
    driver_control_reg.intpol                   = 0;      /* 0: 关闭插值, 1: 16clock模式*/
    driver_control_reg.dedge                    = 0;      /* 0: 上升沿触发, 1: 双边沿模式*/
    driver_control_reg.mres                     = 6;      /* 细分数  4细分     */


    //斩波器的配置，
    chopper_control_reg.blank_time              = 2;      /* 斩波空白时间，也就是关断时间，0: 16, 1: 24, 2: 36, 3: 54*/
    chopper_control_reg.chopper_mode            = 0;      /* 0: 斩波模式, 1: 常系数模式，一般选择0*/
    chopper_control_reg.hysteresis_decay        = 0;      /* 内阻及磁滞损耗导致实际电流与预想不符的补偿*/
    chopper_control_reg.hysteresis_end          = 15;     /* 内阻及磁滞损耗导致实际电流与预想不符的补偿*/
    chopper_control_reg.hysteresis_start        = 7;      /* 内阻及磁滞损耗导致实际电流与预想不符的补偿*/
    chopper_control_reg.time_off                = 5;      /* 斩波慢衰减时间，影响开关频率*/

    Read_TMC262_State(0);

    Coolstep_Control_REG_Config();
    Stallguard_Control_REG_Config();
    Driver_Config_REG_Config();
    Driver_Control_REG_Config();
    Chopper_Control_REG_Config();
}

/*****************************************************************************
 Function    : Read_TMC262_State
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Read_TMC262_State(uint8_t type)
{
    uint32_t spi_read_data  = 0; 
    driver_config_reg.read_back_select = type;      /* 读取返回项,0:微步,1:负载值,2:负载值高5位及设置电流大小*/
    spi_read_data = Driver_Config_REG_Config();
    memset(&tmc262_state, 0, sizeof(tmc262_state));
    tmc262_state.type = type;
    tmc262_state.sgst = spi_read_data & 0x01;
    tmc262_state.ot   = (spi_read_data & 0x02) << 1;
    tmc262_state.otpw = (spi_read_data & 0x04) << 2;
    tmc262_state.s2gb = (spi_read_data & 0x08) << 3;
    tmc262_state.s2ga = (spi_read_data & 0x10) << 4;
    tmc262_state.olb  = (spi_read_data & 0x20) << 5;
    tmc262_state.ola  = (spi_read_data & 0x40) << 6;
    tmc262_state.stst = (spi_read_data & 0x80) << 7;
    switch(type)
    {
        case TMC262_RB_MSTEP:
            tmc262_state.mstep = (spi_read_data >> 10) & 0x1FF;
            tmc262_state.dir   = spi_read_data >> 19;
        break;
        case TMC262_RB_STALL_GUARD:
            tmc262_state.sg_data = (spi_read_data >> 10) & 0x3FF;
        break;
        case TMC262_RB_SMART_ENERGY:
            tmc262_state.sg_data = (spi_read_data >> 15) & 0x1F;
            tmc262_state.sg_data <<= 5;
            tmc262_state.se_data = (spi_read_data >> 10) & 0x1F;
        break;
    }
}
/*****************************************************************************
 Function    : Motor_Config_Current_Scale
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Config_Current_Scale(uint8_t current_scale)
{    
    chopper_control_reg.time_off = 5;
    Chopper_Control_REG_Config();
    stallguard_control_reg.current_scale = current_scale;  
    Stallguard_Control_REG_Config();
}
/*****************************************************************************
 Function    : Motor_Config_Zhengzhuan
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Config_Zhengzhuan(void)
{ 
    TMC262_DIR_HIGH;
    TMC262_Init();
}
/*****************************************************************************
 Function    : Motor_Config_Fanzhuan
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Motor_Config_Fanzhuan(void)
{
    TMC262_DIR_LOW;
    TMC262_Init(); 
}

/************************ZXDQ *****END OF FILE****/

