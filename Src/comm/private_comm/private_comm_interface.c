/**
  ******************************************************************************
  * File Name          : private_comm_interface.h
  * Description        : This file provides the function about interface of 
                         private communication protocol.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
#include "private_comm_interface.h"
#include "auto.h"
#include "fram_addr_map.h"
extern PrivateCommTypeDef  privateComm;
extern int16_t telemetryReg[32];              //遥测输入寄存器
/*****************************************************************************
 Function    : Private_Comm_Interface_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Private_Comm_Interface_Init(RemoteSignalTypeDef *remoteSignal, 
                                 ConfigParaTypeDef   *configPara,
                                 ClockTypeDef        *clock,
                                 SwitchTypeDef       *sw,
                                 uint16_t            *deviceStat)
{
    uint8_t i;
    privateComm.coilReg[0 ] = &remoteSignal->autoMode;
    privateComm.coilReg[1 ] = &remoteSignal->remoteMode;

    privateComm.txFinishFlag =  1;
    privateComm.deviceStatus =  deviceStat;
    privateComm.currentGear  =  &sw->currentGear;
    privateComm.memoryGear   =  &sw->memoryGear;

    
    privateComm.discreteInputReg[0 ] = &remoteSignal->autoMode;
    privateComm.discreteInputReg[1 ] = &remoteSignal->remoteMode;
    privateComm.discreteInputReg[2 ] = &remoteSignal->gearSignal.gear1;
    privateComm.discreteInputReg[3 ] = &remoteSignal->gearSignal.gear2;
    privateComm.discreteInputReg[4 ] = &remoteSignal->gearSignal.gear3;
    privateComm.discreteInputReg[5 ] = &remoteSignal->gearSignal.gear4;
    privateComm.discreteInputReg[6 ] = &remoteSignal->gearSignal.gear5;
    privateComm.discreteInputReg[7 ] = &remoteSignal->gearSignal.gear6;
    privateComm.discreteInputReg[8 ] = &remoteSignal->gearSignal.gear7;
    privateComm.discreteInputReg[9 ] = &remoteSignal->gearSignal.gear8;
    privateComm.discreteInputReg[10] = &remoteSignal->gearSignal.gear9;
    privateComm.discreteInputReg[11] = &remoteSignal->gearSignal.gear10;
    privateComm.discreteInputReg[12] = &remoteSignal->gearSignal.gear11;
    privateComm.discreteInputReg[13] = &remoteSignal->gearSignal.gear12;
    privateComm.discreteInputReg[14] = &remoteSignal->gearSignal.gear13;
    privateComm.discreteInputReg[15] = &remoteSignal->gearSignal.gear14;
    privateComm.discreteInputReg[16] = &remoteSignal->gearSignal.gear15;
    privateComm.discreteInputReg[17] = &remoteSignal->gearSignal.gear16;
    privateComm.discreteInputReg[18] = &remoteSignal->gearSignal.gear17;
    privateComm.discreteInputReg[19] = &remoteSignal->gearSignal.capacity;
    privateComm.discreteInputReg[20] = &remoteSignal->lockSwitch;
    privateComm.discreteInputReg[21] = &remoteSignal->oilPosAlarm;
    privateComm.discreteInputReg[22] = &remoteSignal->oilTemAlarm;
    privateComm.discreteInputReg[23] = &remoteSignal->powerOffAlarm;
    privateComm.discreteInputReg[24] = &remoteSignal->overCurrentAlarm;
    privateComm.discreteInputReg[25] = &remoteSignal->lowVoltageAlarm;
    privateComm.discreteInputReg[26] = &remoteSignal->highVoltageAlarm;
    privateComm.discreteInputReg[27] = &remoteSignal->gearFault;
    privateComm.discreteInputReg[28] = &remoteSignal->switchFault;
    privateComm.discreteInputReg[29] = &remoteSignal->turnGearFail;
    privateComm.discreteInputReg[30] = &remoteSignal->turnCapFail;
    privateComm.discreteInputReg[31] = &remoteSignal->initFail;


    for(i = 0; i < PRIVATE_COMM_INPUT_REG_LIMIT; i++)
        privateComm.inputReg[i ] = &telemetryReg[i];


    privateComm.holdingReg[0 ] = &configPara->thDownVoltage;
    privateComm.holdingReg[1 ] = &configPara->thUpVoltage;
    privateComm.holdingReg[2 ] = &configPara->tySpace;
    privateComm.holdingReg[3 ] = &configPara->tyDelay;
    privateComm.holdingReg[4 ] = &configPara->ctRatio;
    privateComm.holdingReg[5 ] = &configPara->lockOverCurrent;
    privateComm.holdingReg[6 ] = &configPara->lockVoltageUpLimit;
    privateComm.holdingReg[7 ] = &configPara->lockVoltageLowLimit;
    privateComm.holdingReg[8 ] = &configPara->lockHighVoltage;
    privateComm.holdingReg[9 ] = &configPara->lockLowVoltage;
    privateComm.holdingReg[10] = &configPara->thChangeCapacity;
    privateComm.holdingReg[11] = &configPara->ccDelay;
    privateComm.holdingReg[12] = &configPara->thAlarmTemp;
    privateComm.holdingReg[13] = &configPara->tranCapacity;
    privateComm.holdingReg[14] = &configPara->code;

    privateComm.holdingReg[15] = &configPara->deviceInfo.tapTotalNum;
    privateComm.holdingReg[16] = &configPara->deviceInfo.tapWideHigh;
    privateComm.holdingReg[17] = &configPara->deviceInfo.tapWideLow;
    privateComm.holdingReg[18] = &configPara->deviceInfo.switchType;
    privateComm.holdingReg[19] = &configPara->deviceInfo.hardVersion;
    privateComm.holdingReg[20] = &configPara->deviceInfo.softVersion;


    privateComm.holdingReg[21] = &clock->y;
    privateComm.holdingReg[22] = &clock->m;
    privateComm.holdingReg[23] = &clock->d;
    privateComm.holdingReg[24] = &clock->h;
    privateComm.holdingReg[25] = &clock->min;
    privateComm.holdingReg[26] = &clock->s;

    Private_Comm_Start_Rec();
}
/*****************************************************************************
 Function    : Private_Comm_Interface_Remote_Control_Process
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Interface_Remote_Control_Process(RemoteSignalTypeDef *remoteSignal, 
                                                       ConfigParaTypeDef   *configPara,
                                                       SwitchTypeDef       *sw,
                                                       uint8_t codeType, uint8_t data)
{
    switch(codeType)
    {
        case FUNCTION_CODE_MODE:
            if(data == 0)
            {
                remoteSignal->autoMode   = 1;
                remoteSignal->remoteMode = 0;
            }
            else if(data == 1)
            {
                remoteSignal->autoMode   = 0;
                remoteSignal->remoteMode = 1;
            }
            else
                return STATUS_CODE_OVER_ERR;
        break;
        case FUNCTION_CODE_YK:
            if(remoteSignal->autoMode == 1)
                return STATUS_CODE_MODE_ERR;
            if(remoteSignal->lockSwitch == 1)
                return STATUS_CODE_LOCK_ERR;
            if(data == 1)
            {
#ifdef STEP_MOTOR                
                if(sw->memoryGear >= configPara-> deviceInfo.tapTotalNum)
#else
                if(sw->currentGear >= configPara-> deviceInfo.tapTotalNum)
#endif
                    return STATUS_CODE_DW_HIGH_ERR;
            }
            else if(data == 2)
            {
#ifdef STEP_MOTOR                
                if(sw->memoryGear == 1)
#else
                if(sw->currentGear == 1)
#endif
                    return STATUS_CODE_DW_LOW_ERR;
            }
            else if(data != 3)
                return STATUS_CODE_OVER_ERR;
            privateComm.switchMotion = data;
            privateComm.flagYkReady  = 1;
        break;
        case FUNCTION_CODE_YK_CONFIRM:
            if(remoteSignal->autoMode == 1)
                return STATUS_CODE_MODE_ERR;
            if(remoteSignal->lockSwitch == 1)
                return STATUS_CODE_LOCK_ERR;
            if(privateComm.flagYkReady  == 0)
                return STATUS_CODE_NO_YK_ERR;
#ifdef STEP_MOTOR                
                if(sw->memoryGear == 1 && privateComm.switchMotion == 2)
#else
                if(sw->currentGear == 1 && privateComm.switchMotion == 2)
#endif
                return STATUS_CODE_DW_LOW_ERR;
#ifdef STEP_MOTOR                
                if(sw->memoryGear >= configPara-> deviceInfo.tapTotalNum && privateComm.switchMotion == 1)
#else
                if(sw->currentGear >= configPara-> deviceInfo.tapTotalNum && privateComm.switchMotion == 1)
#endif
                return STATUS_CODE_DW_HIGH_ERR;
            if(data == 0xFF)
                sw->remoteMotion = privateComm.switchMotion;
            else if(data != 0x00)
                return STATUS_CODE_OVER_ERR;
            privateComm.flagYkReady         = 0;
            privateComm.ykReturnTimeCounter = 0;
        break;
        case FUNCTION_CODE_REBOOT:
            if(data == 0xFF)
                sw->resetFlag = 1;
        break;
        case FUNCTION_CODE_CLR_REC:
            if(data == 0)
            {
                if(Clear_Data(SWITCH_MOTION_COUNTER_ADDR, 6) != STORAGE_OK)
                {
                    remoteSignal->storeErr = 1;
                    return STATUS_CODE_STORE_ERR;
                }
                if(Clear_Data(SWITCH_MOTION_RECORD_NUM_ADDR, 4) != STORAGE_OK)
                {
                    remoteSignal->storeErr = 1;
                    return STATUS_CODE_STORE_ERR;
                }
            }
            else if(data == 1)
            {
                if(Clear_Data(ALARM_COUNTER_ADDR, 4) != STORAGE_OK)
                {
                    remoteSignal->storeErr = 1;
                    return STATUS_CODE_STORE_ERR;
                }
                if(Clear_Data(ALARM_RECORD_NUM_ADDR, 4) != STORAGE_OK)
                {
                    remoteSignal->storeErr = 1;
                    return STATUS_CODE_STORE_ERR;
                }
            }
            else if(data == 2)
            {
                if(Clear_Data(REBOOT_COUNTER_ADDR, 4) != STORAGE_OK)
                {
                    return STATUS_CODE_STORE_ERR;
                }
            }
        break;
        default:;
        break;
    }
    return 0;
}

/*****************************************************************************
 Function    : Private_Comm_Interface_Set_Config_Process
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Interface_Set_Config_Process(RemoteSignalTypeDef *remoteSignal,
                                                   ConfigParaTypeDef   *configPara,
                                                   ClockTypeDef        *clock,
                                                   SaveFlagTypeDef     *saveFlag,
                                                   uint8_t codeType, uint16_t *data, uint8_t length)
{
    uint8_t i;
    uint16_t res = 0;
    uint8_t regAddr, regNum;
    if(codeType == FUNCTION_CODE_SET_CFG)
        regAddr = 0;
    else
        regAddr = 21;
    regNum = length;
    for(i = regAddr; i < regAddr+regNum; i++)
    {
        switch(i)
        {
            case 0:
                if(data[i] < 225 || data[i] > 240)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 1:
                if(data[i] < 198 || data[i] > 225)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 2:
                if(data[i] < 5 || data[i] > 3600)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 3:
                if(data[i] < 5 || data[i] > 1200)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 4:
                if(data[i] == 300 || data[i] == 600 || data[i] == 1000)
                    ;
                else
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 5:;
            break;
            case 6:;
            break;
            case 7:;
            break;
            case 8:
                if(data[i] < 250 || data[i] > 300)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 9:
                if(data[i] < 150 || data[i] > 200)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 10:
                if(data[i] > configPara->tranCapacity/3)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 11:
                if(data[i] < 600)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 12:;
            break;
            case 13:;
            break;
            case 14:;
            break;

            case 15:;
            break;
            case 16:;
            break;
            case 17:;
            break;

            case 18:;
            break;
            case 19:;
            break;
            case 20:;
            break;

            case 21:
                if(data[i] < 19 || data[i] > 100)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 22:
                if(data[i] == 0 || data[i] > 12)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 23:
                if(data[i] == 0 || data[i] > 31)
                    res = STATUS_CODE_OVER_ERR;
            break;

            case 24:
                if(data[i] > 23)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 25:
                if(data[i] > 59)
                    res = STATUS_CODE_OVER_ERR;
            break;
            case 26:
                if(data[i] > 59)
                    res = STATUS_CODE_OVER_ERR;
            break;
            default:;
            break;
        }
    }
    if(res == 0)
    {
        for(i = regAddr; i < regAddr+regNum; i++)
        {
            switch(i)
            {
                case 0:
                    configPara->thDownVoltage = data[i];
                break;
                case 1:
                    configPara->thUpVoltage = data[i];
                break;
                case 2:
                    configPara->tySpace = data[i];
                break;
                case 3:
                    configPara->tyDelay = data[i];
                break;
                case 4:
                    configPara->ctRatio = data[i];
                break;
                case 5:
                    configPara->lockOverCurrent = data[i];
                break;
                case 6:
                    configPara->lockVoltageUpLimit = data[i];
                break;
                case 7:
                    configPara->lockVoltageLowLimit = data[i];
                break;
                case 8:
                    configPara->lockHighVoltage = data[i];
                break;
                case 9:
                    configPara->lockLowVoltage = data[i];
                break;
                case 10:
                    configPara->thChangeCapacity = data[i];
                break;
                case 11:
                    configPara->ccDelay = data[i];
                break;
                case 12:
                    configPara->thAlarmTemp = data[i];
                break;
                case 13:
                
                    configPara->tranCapacity = data[i];
                break;
                case 14:
                    configPara->code = data[i];
                break;
    
                case 15:
                    ;
                break;
                case 16:
                    ;
                break;
                case 17:
                    
                break;
                case 18:
                    
                break;
                case 19:
                    
                break;
                case 20:
                    
                break;

                case 21:
                    clock->y = data[i];
                break;
                case 22:
                    clock->m = data[i];
                break;
                case 23:
                    clock->d = data[i];
                break;
                case 24:
                    clock->h = data[i];
                break;
                case 25:
                    clock->min = data[i];
                break;
                case 26:
                    clock->s = data[i];
                break;
             
                default:;
                break;
            }
        }
        if(regAddr > 20)
            Clock_Set_Date_And_Time(clock);
        else
        {
            if(Save_Config_Para(configPara) != STORAGE_OK)
            {
                return STATUS_CODE_STORE_ERR;
            }
        }
    }
    return res;
}
/*****************************************************************************
 Function    : Private_Comm_Interface_Get_Rec_Process
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint16_t Private_Comm_Interface_Get_Rec_Process(uint8_t codeType, uint16_t addrNum,  uint8_t *data)
{
    uint16_t res = 0;
    uint16_t addrStart, length;
    if(codeType == FUNCTION_CODE_GET_T_REC)
    {
        length = SWITCH_MOTION_RECORD_PER_LEN;
        addrStart = SWITCH_MOTION_RECORD_ADDR;
    }
    else if(codeType == FUNCTION_CODE_GET_A_REC)
    {

        length = ALARM_RECORD_PER_LEN;
        addrStart = ALARM_RECORD_ADDR;
    }
    if(Read_One_Rec(addrStart + (addrNum - 1)*length , data, length) != STORAGE_OK)
    {
        return STATUS_CODE_STORE_ERR;
    }
    return res;
}
/************************ZXDQ *****END OF FILE****/
