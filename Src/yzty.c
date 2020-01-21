/**
  ******************************************************************************
  * File Name          : yzty.c
  * Description        : This file provides the user's entry program.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "yzty.h"
#include "auto.h"
#include "storage.h"
#include "delay.h"
#include "telemetry.h"
#include "remote_signal.h"
#include "modbus_interface.h"
#include "private_comm_interface.h"
#include "lcd.h"
#include "oil_temp.h"
#include "switch_driver.h"
#include "led.h"

ACSampleCalibrationParaTypeDef       g_calibrationPara;
ACSampleCalibrationInputTypeDef      g_calibrationInput;
TelemetryTypeDef                     g_telemetry;
DeviceStatusTypeDef                  g_deviceStatus;
ConfigParaTypeDef                    g_configPara;
RemoteSignalTypeDef                  g_remoteSignal;
SwitchTypeDef                        g_switch;
SaveFlagTypeDef                      g_saveFlag;
ClockTypeDef                         g_clock;
LCDStatusTypeDef                     g_lcdStatus;


volatile uint32_t tySpace = 0;                   //开关动作时间间隔
uint32_t msCount = 0;                            //毫秒计数器
int16_t  telemetryReg[32] = {0};                 //遥测输入寄存器，通信地址映射用
extern volatile  uint8_t  flagHandBtOK;          //手动按钮检测标志
volatile  uint16_t remoteHeartTimeout = 0;       //远程通信心跳超时
/***********设备状态开始************************************************************/
uint16_t lastDeciveStatus    = 0;
uint16_t currentDeciveStatus = 0;
/***********设备状态结束************************************************************/
/***********电源切换开始************************************************************/
volatile uint16_t timeToCBPCounter = 120;        //关闭大电源的时间计数
#define  POWER_ON_CBP_TIMEOUT     120            //秒
#define  SW_CTR_AGO_CBP_TIMEOUT   60             //秒
/***********电源切换结束************************************************************/

/***********油温相关开始************************************************************/
volatile  uint16_t readOilTmpSpace    = 0;       //读油温时间间隔
volatile  uint16_t oilTmpConvertSpace = 0;       //油温转换时间间隔
volatile  uint8_t  flagReadOilTmp     = 0;
/***********油温相关结束************************************************************/

/***********保护相关开始************************************************************/
volatile  uint8_t overCurrentCount = 1;
volatile  uint8_t overCurrentFlag  = 1;
volatile  uint8_t lowVoltageCount  = 1;
volatile  uint8_t lowVoltageFlag   = 1;
volatile  uint8_t highVoltageCount = 1;
volatile  uint8_t highVoltageFlag  = 1;
/***********保护相关结束************************************************************/
/*****************************************************************************
 Function    : YZTY_Init
 Description : Initialization of YZTY.
 Input       : None
 Output      : None
 Return      : The result of init.
 *****************************************************************************/
uint8_t YZTY_Init(void)
{
    uint8_t res = 0;
    uint8_t initData[64] = {0};
    SwitchStatusTypeDef swStatus = SWITCH_OK;

    timeToCBPCounter = POWER_ON_CBP_TIMEOUT;
    
    if(Save_Reboot_Counter() != STORAGE_OK)
    {
        res = 1;
        g_remoteSignal.storeErr = 1;
    }
    LCD_Init(); 
    Clock_Get_Date_And_Time(&g_clock); 
    LCD_Display_Logo(&g_clock);
    YZTY_Comm_Init();
    
    g_configPara.deviceInfo.switchType  = SWITCH_TYPE;
    g_configPara.deviceInfo.hardVersion = HARD_VERSION;
    g_configPara.deviceInfo.softVersion = SOFT_VERSION;

    if(Read_First_Program_Flag(initData) != STORAGE_OK)
        Save_Default_Config(&g_configPara);
    else if(initData[0] != FIRST_PROGRAM_FLAG)
        Save_Default_Config(&g_configPara);
    if(Read_Config_Para(&g_configPara) != STORAGE_OK)
    {
        res = 1;
        g_remoteSignal.storeErr = 1;
    } 

    if(Read_AC_Sample_Calibration_Para(&g_calibrationPara) != STORAGE_OK)
    {
        res = 1;
        g_remoteSignal.storeErr = 1;
    }
 
    if(Read_AC_Sample_Calibration_Input(&g_calibrationInput) != STORAGE_OK)
    {
        res = 1;
        g_remoteSignal.storeErr = 1;
    }

    if(AC_Sample_Init(&g_calibrationPara, &g_calibrationInput) != AC_SAMPLE_OK)
    {
        res = 1;
        g_remoteSignal.sampleErr = 1;
    }

    if(Read_Gear(&g_switch, &g_remoteSignal.gearSignal) == GEAR_OK)
    {
        g_switch.memoryGear = g_switch.currentGear; 
        Save_Memory_Gear(g_switch.memoryGear);
    }
    else if(Read_Memory_Gear(initData) != STORAGE_OK)
    {
        g_switch.memoryGear = g_configPara.tapTotalNum/2;
        Save_Memory_Gear(g_configPara.tapTotalNum/2);
    }
    else
    {
        if(initData[0] > 0 && initData[0] < g_configPara.tapTotalNum + 1)
            g_switch.memoryGear = initData[0];
        else
        {
            g_switch.memoryGear = g_configPara.tapTotalNum/2;
            Save_Memory_Gear(g_configPara.tapTotalNum/2);
        }
    }

    Remote_Signal_Init(&g_remoteSignal);
    Switch_Driver_Init();
    swStatus = Switch_Init(&g_switch, g_configPara.tapTotalNum);
    if(swStatus != SWITCH_OK)
    {
        res = 1;
        if(swStatus == SWITCH_GEAR_ERROR)
            g_remoteSignal.gearFault = 1;
        else if(swStatus == SWITCH_MOTOR_ERROR)
            g_remoteSignal.motorFault = 1;
    }
    if(res != 0)
    {
        g_remoteSignal.initFail = 1;
    }
    else
    {
        g_remoteSignal.initFail = 0;
    }
    return res;
}

/*****************************************************************************
 Function    : YZTY_Read_Telemetry
 Description : Read telemetry data.
 Input       : g_telemetry.sample, g_calibrationInput
 Output      : g_telemetry.sample, sampleErr
 Return      : None
 *****************************************************************************/
void YZTY_Read_Telemetry(void)
{
    if(AC_Sample_Read(&(g_telemetry.sample), &g_calibrationInput) != AC_SAMPLE_OK)
        g_remoteSignal.sampleErr = 1;
    else
        g_remoteSignal.sampleErr = 0;
    if(g_remoteSignal.sampleErr == 0)
    {
        telemetryReg[0 ] =  (int16_t)(g_telemetry.sample.ua     * 10.0);
        telemetryReg[1 ] =  (int16_t)(g_telemetry.sample.ub     * 10.0);
        telemetryReg[2 ] =  (int16_t)(g_telemetry.sample.uc     * 10.0);
        telemetryReg[3 ] =  (int16_t)(g_telemetry.sample.ia     * 1000.0);
        telemetryReg[4 ] =  (int16_t)(g_telemetry.sample.ib     * 1000.0);
        telemetryReg[5 ] =  (int16_t)(g_telemetry.sample.ic     * 1000.0);
        telemetryReg[6 ] =  (int16_t)(g_telemetry.sample.pa     );
        telemetryReg[7 ] =  (int16_t)(g_telemetry.sample.pb     );
        telemetryReg[8 ] =  (int16_t)(g_telemetry.sample.pc     );
        telemetryReg[9 ] =  (int16_t)(g_telemetry.sample.qa     );
        telemetryReg[10] =  (int16_t)(g_telemetry.sample.qb     );
        telemetryReg[11] =  (int16_t)(g_telemetry.sample.qc     );
        telemetryReg[12] =  (int16_t)(g_telemetry.sample.pf     * 100.0);
        telemetryReg[13] =  (int16_t)(g_telemetry.sample.freq   * 100.0);
        telemetryReg[14] =  (int16_t)(g_telemetry.sample.tmp    * 10.0);
        telemetryReg[15] =  (int16_t)(g_telemetry.oilTmp        * 10.0);
    }   
}
/*****************************************************************************
 Function    : YZTY_Read_Clock
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Read_Clock(void)
{
    Clock_Get_Date_And_Time(&g_clock);
}
/*****************************************************************************
 Function    : YZTY_Reset_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
 void YZTY_Reset_Judge(void)
 {
    if(g_switch.resetFlag == 1)
    {
        delay_ms(1000);
        __set_FAULTMASK(1); 
        NVIC_SystemReset(); //关闭所有中断，系统复位;  
    }
 }
 /*****************************************************************************
 Function    : YZTY_Read_Oil_Temp
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Read_Oil_Temp(void)
{
    float tmp = 0;
    if(flagReadOilTmp == 0 && readOilTmpSpace == 0)
    {
        /*-----------------关闭中断------------------------*/
        __disable_irq();
        if(Temp_Convert_Start() == TMP_OK)
        {
            oilTmpConvertSpace = OIL_TMP_CONVERT_TIME;
            flagReadOilTmp = 1;
        }
        else
            readOilTmpSpace = READ_OIL_TMP_PERIOD;
        /*-----------------打开中断------------------------*/
        __enable_irq();
    }
    if(flagReadOilTmp == 1)
    {
        if(oilTmpConvertSpace == 0)
        {
            /*-----------------关闭中断------------------------*/
            __disable_irq();
            if(Temp_Read(&tmp) == TMP_OK)
            {
                g_telemetry.oilTmp = tmp;
                if(tmp >= g_configPara.thAlarmTemp)
                    g_remoteSignal.oilTemAlarm = 1;
                else
                    g_remoteSignal.oilTemAlarm = 0;
            }
            readOilTmpSpace = READ_OIL_TMP_PERIOD;
            flagReadOilTmp = 0;
            /*-----------------打开中断------------------------*/
            __enable_irq();
        }
    }
}
/*****************************************************************************
 Function    : YZTY_Power_Check
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Power_Check(void)
{
    if(timeToCBPCounter == 0)
    {
        CONTROL_POWER_OFF;
        g_remoteSignal.flagPowerON   = 0;
    }
}
/*****************************************************************************
 Function    : YZTY_Time_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Time_Counter(void)
{
    if(msCount < 1000)
        msCount++;
    else
    {
        msCount = 0;
        if(tySpace > 0)
            tySpace--;
        Auto_Control_Time_Counter(g_configPara.tyDelay
#ifdef FUNCTION_TURN_CAPACITY  
    , g_configPara.ccDelay, g_configPara.tranCapacity
#endif
        );
        if(readOilTmpSpace > 0)
            readOilTmpSpace--;
        if(remoteHeartTimeout > 0)
            remoteHeartTimeout--;
        Private_Comm_Yk_Return_Counter();
        if(timeToCBPCounter > 0)
            timeToCBPCounter--;
    }
    if(oilTmpConvertSpace > 0)
        oilTmpConvertSpace--;
    Remote_Signal_Time_Counter(&g_remoteSignal);
    Led_Control_Counter(currentDeciveStatus, g_switch.currentGear);
    Modbus_Rx_Space_Counter();
}
/*****************************************************************************
 Function    : YZTY_Heart_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Heart_Judge(void)
{
    if(remoteHeartTimeout == 0)
    {
        if(g_remoteSignal.autoMode == 0 && g_remoteSignal.remoteMode == 1)
        {
            g_remoteSignal.autoMode = 1;
            g_remoteSignal.remoteMode = 0;
            Auto_Control_Clear_Motion();
#ifdef FUNCTION_TURN_CAPACITY
            Auto_Control_Capa_Clear_Motion();
#endif
        }
    }
}
/*****************************************************************************
 Function    : YZTY_Device_Status_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Device_Status_Judge(void)
{
    g_deviceStatus.initFail     = g_remoteSignal.initFail         ; 
    g_deviceStatus.sampleErr    = g_remoteSignal.sampleErr        ; 
    g_deviceStatus.storeErr     = g_remoteSignal.storeErr         ;     
    g_deviceStatus.gearFault    = g_remoteSignal.gearFault        ;     
    g_deviceStatus.motorFault   = g_remoteSignal.motorFault       ;     
    g_deviceStatus.powerOff     = g_remoteSignal.powerOffAlarm    ;     
    g_deviceStatus.overCurrent  = g_remoteSignal.overCurrentAlarm ;     
    g_deviceStatus.lowVoltage   = g_remoteSignal.lowVoltageAlarm  ;     
    g_deviceStatus.highVoltage  = g_remoteSignal.highVoltageAlarm ;     
    g_deviceStatus.lockSwitch   = g_remoteSignal.lockSwitch       ;     
    g_deviceStatus.oilPosAlarm  = g_remoteSignal.oilPosAlarm      ;     
    g_deviceStatus.oilTemAlarm  = g_remoteSignal.oilTemAlarm      ;  
    g_deviceStatus.turnGearFail = g_remoteSignal.turnGearFail     ;   
    g_deviceStatus.turnCapFail  = g_remoteSignal.turnCapFail      ; 
    currentDeciveStatus = *((uint16_t *)&g_deviceStatus);
    if(currentDeciveStatus != lastDeciveStatus)
    {
        g_saveFlag.alarmFlag = 1;
    }
}

/*****************************************************************************
 Function    : YZTY_Protect_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Protect_Judge(void)
{
    if(g_telemetry.sample.ia * g_configPara.ctRatio > g_configPara.lockOverCurrent 
        || g_telemetry.sample.ib * g_configPara.ctRatio > g_configPara.lockOverCurrent 
        || g_telemetry.sample.ic * g_configPara.ctRatio > g_configPara.lockOverCurrent)
        overCurrentFlag = 1;   
    else
        overCurrentFlag = 0;
    if(g_telemetry.sample.ua < g_configPara.lockLowVoltage || 
       g_telemetry.sample.ub < g_configPara.lockLowVoltage || 
       g_telemetry.sample.uc < g_configPara.lockLowVoltage)
        lowVoltageFlag = 1;
    else
        lowVoltageFlag = 0;
    if(g_telemetry.sample.ua > g_configPara.lockHighVoltage || 
       g_telemetry.sample.ub > g_configPara.lockHighVoltage || 
       g_telemetry.sample.uc > g_configPara.lockHighVoltage)
        highVoltageFlag = 1;
    else
        highVoltageFlag = 0;
    if(g_remoteSignal.overCurrentAlarm != overCurrentFlag)
    {
        if(overCurrentCount < PROTECT_SUB_DITH_TIME)
            overCurrentCount++;
        else
            g_remoteSignal.overCurrentAlarm = overCurrentFlag;
    }
    else
        overCurrentCount = 1;
    if(g_remoteSignal.highVoltageAlarm != highVoltageFlag)
    {
        if(highVoltageCount < PROTECT_SUB_DITH_TIME)
            highVoltageCount++;
        else
            g_remoteSignal.highVoltageAlarm = highVoltageFlag;
    }
    else
        highVoltageCount = 1;
    if(g_remoteSignal.lowVoltageAlarm != lowVoltageFlag)
    {
        if(lowVoltageCount < PROTECT_SUB_DITH_TIME)
            lowVoltageCount++;
        else
            g_remoteSignal.lowVoltageAlarm = lowVoltageFlag;
    }
    else
        lowVoltageCount = 0;
}
/*****************************************************************************
 Function    : YZTY_Lock_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Lock_Judge(void)
{
    if(g_remoteSignal.autoMode == 1)  //自动模式
    {
 
        if(g_remoteSignal.overCurrentAlarm == 1 || g_remoteSignal.highVoltageAlarm == 1 || 
           g_remoteSignal.lowVoltageAlarm  == 1 || g_remoteSignal.gearFault        == 1 ||
           g_remoteSignal.motorFault       == 1 )
            g_remoteSignal.lockSwitch = 1;
        else
            g_remoteSignal.lockSwitch = 0;
    }
    else if(g_remoteSignal.remoteMode == 1) //手动模式
    {
        if(g_remoteSignal.gearFault        == 1 || 
           g_remoteSignal.motorFault       == 1 )
            g_remoteSignal.lockSwitch = 1;
        else
            g_remoteSignal.lockSwitch = 0;
    }
}
/*****************************************************************************
 Function    : YZTY_Read_Gear_Signal
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Read_Gear_Signal(void)
{
    if(Gear_Check(&g_switch, &g_remoteSignal.gearSignal) == GEAR_OK)
        g_remoteSignal.gearFault  = 0;
    else
        g_remoteSignal.gearFault  = 1;
}
/*****************************************************************************
 Function    : YZTY_Switch_Action
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t YZTY_Switch_Action(void)
{
    SwitchStatusTypeDef res = SWITCH_OK;

    /*-----------------关闭中断------------------------*/
    __disable_irq();
    res = Switch_Control(&g_switch);
    Save_Memory_Gear(g_switch.memoryGear);
    /*-----------------打开中断------------------------*/
    __enable_irq();
    if(res != SWITCH_OK)
    {
        if(res == SWITCH_GEAR_ERROR)
            g_remoteSignal.gearFault = 1;
        else if(res == SWITCH_MOTOR_ERROR)
            g_remoteSignal.motorFault = 1;
        return 1;
    }
    else
    {
        g_remoteSignal.gearFault  = 0;
        g_remoteSignal.motorFault = 0;
    }
    timeToCBPCounter = SW_CTR_AGO_CBP_TIMEOUT;
    return 0;
}
/*****************************************************************************
 Function    : YZTY_Hand_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Hand_Judge(void)
{
    if(flagHandBtOK == 1)
    {
        if(g_remoteSignal.handButton.up == 1)
            g_switch.handMotion = 1;
        else if(g_remoteSignal.handButton.down == 1)
            g_switch.handMotion = 2;
        else if(g_remoteSignal.handButton.cp == 1)
            g_switch.handMotion = 3;
    }
}
/*****************************************************************************
 Function    : YZTY_Control_Judge
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Control_Judge(void)
{
    SwitchMotionTypeDef action = RC_NONE;
    if(g_remoteSignal.autoMode == 1 && g_remoteSignal.lockSwitch == 0)  //自动模式
    {
        g_switch.autoMotion = Auto_Control(&g_telemetry.sample, &g_configPara
#ifdef FUNCTION_TURN_CAPACITY  
    , &g_remoteSignal
#endif
            );
    }
    if(g_remoteSignal.lockSwitch == 0)
    {
        if(g_remoteSignal.autoMode == 1)
            g_switch.motion = g_switch.autoMotion;
        else
            g_switch.motion = g_switch.remoteMotion;

        if(g_switch.handMotion != 0)
            g_switch.motion = g_switch.handMotion;
        else if(g_switch.lcdMotion != 0)
            g_switch.motion = g_switch.lcdMotion;
        if(g_switch.motion == RC_UP_VOL)
        {
            if(g_switch.currentGear > g_configPara.tapTotalNum - 1)
                action = RC_NONE;
            else
                action = RC_UP_VOL;
        }
        else if(g_switch.motion == RC_DOWN_VOL)
        {
            if(g_switch.currentGear < 2 )
                action = RC_NONE;
            else
                action = RC_DOWN_VOL;
        }
        else
            action = RC_NONE;
    }
    else
        action = RC_NONE;

    if((action == RC_UP_VOL || action == RC_DOWN_VOL))
    {
        if(tySpace == 0)
        {
            CONTROL_POWER_ON;
            g_remoteSignal.flagPowerON = 1;
            delay_ms(REMOTE_SIGNAL_SUB_DITH_TIME + 50); //主电源开启后，等待读主电源遥信
            if(g_remoteSignal.powerOffAlarm == 0)
            {
                if(YZTY_Switch_Action() != 0)
                    g_remoteSignal.turnGearFail = 1;
                else
                    g_remoteSignal.turnGearFail = 0;
            }
            else
                g_remoteSignal.turnGearFail = 1;
            g_switch.lastMotion     = g_switch.motion;
            g_saveFlag.switchMotionFlag = 1;
            tySpace = g_configPara.tySpace;
            if(g_remoteSignal.autoMode == 1)
                Auto_Control_Clear_Motion();
        }
    }
#ifdef FUNCTION_TURN_CAPACITY
    else if(action == RC_TURN_CAPA)
    {
        if(tySpace == 0)
        {
            CONTROL_POWER_ON;
            g_remoteSignal.flagPowerON = 1;
            delay_ms(REMOTE_SIGNAL_SUB_DITH_TIME + 50); //主电源开启后，等待读主电源遥信
            if(g_remoteSignal.powerOffAlarm == 0)
            {
                if(YZTY_Switch_Action() != 0)
                    g_remoteSignal.turnGearFail = 1;
                else
                    g_remoteSignal.turnGearFail = 0;
            }
            else
                g_remoteSignal.turnGearFail = 1;
            g_switch.lastMotion     = g_switch.motion;
            g_saveFlag.switchMotionFlag = 1;
            tySpace = g_configPara.tySpace;
            if(g_remoteSignal.autoMode == 1)
                Auto_Control_Capa_Clear_Motion();
        } 
    }
#endif
    g_switch.autoMotion     = 0;
    g_switch.remoteMotion   = 0;
    g_switch.lcdMotion      = 0;
    g_switch.handMotion     = 0;
}
/*****************************************************************************
 Function    : YZTY_AC_Sample_Calibration
 Description : AC_Sample_Calibration.
 Input       : g_calibrationPara,g_calibrationInput
 Output      : sampleErr,storeErr.
 Return      : Calibration status. 0 success 1 failure
 *****************************************************************************/
uint8_t YZTY_AC_Sample_Calibration(void)
{
    if(AC_Sample_Calibration(&g_calibrationPara, &g_calibrationInput) != AC_SAMPLE_OK)
    {
        g_deviceStatus.sampleErr = 1;
        return 1;
    }
    else
        g_deviceStatus.sampleErr = 0;

    if(Save_AC_Sample_Calibration_Para(&g_calibrationPara) != STORAGE_OK)
    {
        g_deviceStatus.storeErr = 1;
        return 1;
    }
    else
        g_deviceStatus.storeErr = 0;
    if(Save_AC_Sample_Calibration_Input(&g_calibrationInput) != STORAGE_OK)
    {
        g_deviceStatus.storeErr = 1;
        return 1;
    }
    else
        g_deviceStatus.storeErr = 0;

    if(AC_Sample_Init(&g_calibrationPara, &g_calibrationInput) != AC_SAMPLE_OK)
    {
        g_deviceStatus.sampleErr = 1;
        return 1;
    }
    else
        g_deviceStatus.sampleErr = 0;
  
    return 0;
}
/*****************************************************************************
 Function    : YZTY_Storage
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Storage(void)
{
    uint8_t mode;
    if(g_remoteSignal.autoMode == 1)
        mode = 0;
    else
        mode = 1;
    if(g_saveFlag.switchMotionFlag == 1)
    {
        if(Save_Switch_Motion(&g_clock, &g_telemetry.sample, 
                              &g_telemetry.bfSample, &g_switch, 
                              mode) != STORAGE_OK)
            g_deviceStatus.storeErr = 1;
        else
            g_deviceStatus.storeErr = 0;
        g_saveFlag.switchMotionFlag = 0;
    }
    if(g_saveFlag.alarmFlag == 1)
    {
        if(Save_Alarm(&g_clock, &g_telemetry.sample, 
                      &g_switch,  
                      lastDeciveStatus, currentDeciveStatus,
                      mode) != STORAGE_OK)
            g_deviceStatus.storeErr = 1;
        else
            g_deviceStatus.storeErr = 0;
        g_saveFlag.alarmFlag = 0;   
    }
    g_switch.beforeMotionGear = g_switch.currentGear;
 #ifdef FUNCTION_TURN_CAPACITY       
    g_switch.beforeMotionCapa = g_switch.currentCapa;
 #endif
    lastDeciveStatus = currentDeciveStatus;
    g_telemetry.bfSample = g_telemetry.sample;
}

/*****************************************************************************
 Function    : YZTY_Comm_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Comm_Init(void)
{
    Modbus_Interface_Init(&g_remoteSignal, &g_configPara, &g_clock);
    Private_Comm_Interface_Init(&g_remoteSignal, &g_configPara, &g_clock, &g_switch, &currentDeciveStatus);
}
/*****************************************************************************
 Function    : Modbus_Remote_CTR_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Modbus_Remote_CTR_Callback(uint8_t regAddr, uint8_t *data, uint8_t regNum)
{
    return Modbus_Interface_Remote_Control_Process(&g_remoteSignal, 
                                      &g_configPara,
                                      &g_switch,
                                      regAddr, data, regNum);
}
 /*****************************************************************************
 Function    : Modbus_Set_CFG_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Modbus_Set_CFG_Callback(uint8_t regAddr, uint16_t *data, uint8_t regNum)
{
    return Modbus_Interface_Set_Config_Process(&g_remoteSignal,
                                        &g_configPara,
                                        &g_clock,
                                        &g_saveFlag,
                                        regAddr, data, regNum);
}
 /*****************************************************************************
 Function    : Private_Comm_Remote_CTR_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Remote_CTR_Callback(uint8_t codeType, uint8_t data)
{
    return Private_Comm_Interface_Remote_Control_Process(&g_remoteSignal, 
                                      &g_configPara,
                                      &g_switch,
                                      codeType, data);
}
 /*****************************************************************************
 Function    : Private_Comm_Set_CFG_Callback
 Description : Lcd task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Set_CFG_Callback(uint8_t codeType, uint16_t *data, uint8_t length)
{
    return Private_Comm_Interface_Set_Config_Process(&g_remoteSignal,
                                        &g_configPara,
                                        &g_clock,
                                        &g_saveFlag,
                                        codeType, data, length);
}
  /*****************************************************************************
 Function    : Private_Comm_Calibration_Callback
 Description : Lcd task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Calibration_Callback(float u, float i)
{
    g_calibrationInput.u = u;
    g_calibrationInput.i = i;
    if(YZTY_AC_Sample_Calibration() != 0)
        return 1;
    return 0;
}
 /*****************************************************************************
 Function    : Private_Comm_Get_Rec_Callback
 Description : Lcd task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Get_Rec_Callback(uint8_t codeType, uint16_t addrNum,  uint8_t *data)
{
    return Private_Comm_Interface_Get_Rec_Process(codeType, addrNum, data);
}
 /*****************************************************************************
 Function    : YZTY_Communication
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Communication(void)
{
    Modbus_Data_Analysis(&remoteHeartTimeout);                  //modbus通信处理
    Private_Comm_Data_Analysis(&remoteHeartTimeout);            //私有通信协议处理
}
/*****************************************************************************
 Function    : YZTY_Lcd
 Description : Lcd task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void YZTY_Lcd(void)
{
    LCDKeyTypeTypeDef keyType;
    keyType = LCD_Key_Read(&(g_remoteSignal.lcdButton));
    g_lcdStatus.currentKeyType = keyType;
    if(keyType != KEY_TYPE_NONE)
    {
        if(g_lcdStatus.flagTimeout == 1)
            g_lcdStatus.flagTimeout = 0;
        g_lcdStatus.timeCounter = LCD_BKL_ON_TIMEOUT;
        g_lcdStatus.refreshKeyType = keyType;
        if(LCD_BKL_STATUS == 0)
            LCD_BKL_ON;
    }
    if(g_lcdStatus.timeCounter > 0)
        g_lcdStatus.timeCounter--;
    else if(g_lcdStatus.flagTimeout == 0)
    {
        g_lcdStatus.flagTimeout = 1;
        keyType = KEY_TYPE_TIMEOUT;
        g_lcdStatus.refreshKeyType = KEY_TYPE_TIMEOUT;
        LCD_BKL_OFF;
        g_lcdStatus.menuSelect         = 0;
        g_lcdStatus.gearSelect         = 0;
        g_lcdStatus.capacitySelect     = 0;
        g_lcdStatus.monitorCurrentPage = 0;
        g_lcdStatus.controlCurrentPage = 0;
        g_lcdStatus.historyCurrentPage = 0;
    }

    if(keyType == KEY_TYPE_NONE && g_lcdStatus.refreshKeyType != KEY_TYPE_NONE)
    {
        g_lcdStatus.isRefresh = 1;
    }
    else
        g_lcdStatus.isRefresh = 0;
    
    switch(g_lcdStatus.refreshKeyType)
    {
        case KEY_TYPE_HOME:
            g_lcdStatus.menuSelect         = 0;
            g_lcdStatus.gearSelect         = 0;
            g_lcdStatus.capacitySelect     = 0;
            g_lcdStatus.monitorCurrentPage = 0;
            g_lcdStatus.controlCurrentPage = 0;
            g_lcdStatus.historyCurrentPage = 0;
            LCD_Display_Menu(g_lcdStatus.menuSelect, g_lcdStatus.isRefresh);
        break;
        case KEY_TYPE_NEXT:
            LCD_Display_Handle_Next_Key(&g_lcdStatus, &g_telemetry, &g_switch);
        break;
        case KEY_TYPE_UP:
            LCD_Display_Handle_Up_Key(&g_lcdStatus, &g_switch);
        break;
        case KEY_TYPE_DOWN:
            LCD_Display_Handle_Down_Key(&g_lcdStatus, &g_switch);
        break;
        case KEY_TYPE_ENTER:
            LCD_Display_Handle_Enter_Key(&g_lcdStatus, &g_telemetry, &g_switch);
        break;
        case KEY_TYPE_TIMEOUT:
            LCD_Display_RealStatus(&g_switch, SOFT_VERSION,
                                   g_telemetry.oilTmp, currentDeciveStatus, 
                                   g_lcdStatus.isRefresh);
        break;
        default:
        ;
        break;
    }
    
}


/************************ZXDQ *****END OF FILE****/

