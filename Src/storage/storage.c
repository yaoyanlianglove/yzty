/**
  ******************************************************************************
  * File Name          : storage.c
  * Description        : This file provides functions about store operations.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "storage.h"
#include "fram_addr_map.h"
#include "crc.h"
#include "config_para.h"
/*****************************************************************************
 Function    : Save_AC_Sample_Calibration_Para
 Description : None
 Input       : calPara-AC sample calibration parameter.
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_AC_Sample_Calibration_Para(ACSampleCalibrationParaTypeDef *calPara)
{
    uint8_t writeData[64] = {0};

    writeData[0 ] = (calPara->ua >> 24) & 0xFF;
    writeData[1 ] = (calPara->ua >> 16) & 0xFF;
    writeData[2 ] = (calPara->ua >> 8 ) & 0xFF;
    writeData[3 ] =  calPara->ua & 0xFF;

    writeData[4 ] = (calPara->ub >> 24) & 0xFF;
    writeData[5 ] = (calPara->ub >> 16) & 0xFF;
    writeData[6 ] = (calPara->ub >> 8 ) & 0xFF;
    writeData[7 ] =  calPara->ub & 0xFF;

    writeData[8 ] = (calPara->uc >> 24) & 0xFF;
    writeData[9 ] = (calPara->uc >> 16) & 0xFF;
    writeData[10] = (calPara->uc >> 8 ) & 0xFF;
    writeData[11] =  calPara->uc & 0xFF;

    writeData[12] = (calPara->ia >> 24) & 0xFF;
    writeData[13] = (calPara->ia >> 16) & 0xFF;
    writeData[14] = (calPara->ia >> 8 ) & 0xFF;
    writeData[15] =  calPara->ia & 0xFF;

    writeData[16] = (calPara->ib >> 24) & 0xFF;
    writeData[17] = (calPara->ib >> 16) & 0xFF;
    writeData[18] = (calPara->ib >> 8 ) & 0xFF;
    writeData[19] =  calPara->ib & 0xFF;

    writeData[20] = (calPara->ic >> 24) & 0xFF;
    writeData[21] = (calPara->ic >> 16) & 0xFF;
    writeData[22] = (calPara->ic >> 8 ) & 0xFF;
    writeData[23] =  calPara->ic & 0xFF;

    writeData[24] = (calPara->pa >> 24) & 0xFF;
    writeData[25] = (calPara->pa >> 16) & 0xFF;
    writeData[26] = (calPara->pa >> 8 ) & 0xFF;
    writeData[27] =  calPara->pa & 0xFF;

    writeData[28] = (calPara->pb >> 24) & 0xFF;
    writeData[29] = (calPara->pb >> 16) & 0xFF;
    writeData[30] = (calPara->pb >> 8 ) & 0xFF;
    writeData[31] =  calPara->pb & 0xFF;

    writeData[32] = (calPara->pc >> 24) & 0xFF;
    writeData[33] = (calPara->pc >> 16) & 0xFF;
    writeData[34] = (calPara->pc >> 8 ) & 0xFF;
    writeData[35] =  calPara->pc & 0xFF;

    writeData[36] = (calPara->crc >> 8) & 0xFF;
    writeData[37] =  calPara->crc & 0xFF;

    if(FRAM_Write_With_CRC(CALIBRATION_DATA_ADDR, writeData, CALIBRATION_DATA_LEN) != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_AC_Sample_Calibration_Input
 Description : None
 Input       : calPara-AC sample calibration input.
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_AC_Sample_Calibration_Input(ACSampleCalibrationInputTypeDef *input)
{
    uint8_t  writeData[10] = {0};
    uint16_t crc = 0;
    uint32_t adjustU, adjustI;
    adjustU =(uint32_t) (input->u*10);
    adjustI =(uint32_t) (input->i*1000);

    writeData[0] =  adjustU >> 24;
    writeData[1] = (adjustU >> 16) & 0xFF;
    writeData[2] = (adjustU >> 8) & 0xFF;
    writeData[3] =  adjustU & 0xFF;
    writeData[4] =  adjustI >> 24;
    writeData[5] = (adjustI >> 16) & 0xFF;
    writeData[6] = (adjustI >> 8) & 0xFF;
    writeData[7] =  adjustI & 0xFF;
    crc = CRC_16(writeData, 8);
    writeData[8] = crc >> 8;
    writeData[9] = crc & 0xFF;
    if(FRAM_Write_With_CRC(CALIBRATION_INPUT_ADDR, writeData, 10) != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_Config_Para
 Description : None
 Input       : cfgPara-Config parameter.
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_Config_Para(ConfigParaTypeDef *cfgPara)
{
    uint8_t writeData[64] = {0};
               
    writeData[0 ] = (cfgPara->thDownVoltage >>8) & 0xFF;
    writeData[1 ] =  cfgPara->thDownVoltage      & 0xFF;
    writeData[2 ] = (cfgPara->thUpVoltage >>8) & 0xFF;
    writeData[3 ] =  cfgPara->thUpVoltage      & 0xFF;
    writeData[4 ] = (cfgPara->tySpace >>8) & 0xFF;
    writeData[5 ] =  cfgPara->tySpace      & 0xFF;
    writeData[6 ] = (cfgPara->tyDelay >>8) & 0xFF;
    writeData[7 ] =  cfgPara->tyDelay      & 0xFF;
    writeData[8 ] = (cfgPara->ctRatio >>8) & 0xFF;
    writeData[9 ] =  cfgPara->ctRatio      & 0xFF;
    writeData[10] = (cfgPara->lockOverCurrent >>8) & 0xFF;
    writeData[11] =  cfgPara->lockOverCurrent      & 0xFF;
    writeData[12] = (cfgPara->lockVoltageUpLimit >>8) & 0xFF;
    writeData[13] =  cfgPara->lockVoltageUpLimit      & 0xFF;
    writeData[14] = (cfgPara->lockVoltageLowLimit >>8) & 0xFF;
    writeData[15] =  cfgPara->lockVoltageLowLimit      & 0xFF;
    writeData[16] = (cfgPara->lockHighVoltage >>8) & 0xFF;
    writeData[17] =  cfgPara->lockHighVoltage      & 0xFF;
    writeData[18] = (cfgPara->lockLowVoltage >>8) & 0xFF;
    writeData[19] =  cfgPara->lockLowVoltage      & 0xFF;
    writeData[20] = (cfgPara->thChangeCapacity >>8) & 0xFF;
    writeData[21] =  cfgPara->thChangeCapacity      & 0xFF;
    writeData[22] = (cfgPara->ccDelay >>8) & 0xFF;
    writeData[23] =  cfgPara->ccDelay      & 0xFF;
    writeData[24] = (cfgPara->thAlarmTemp >>8) & 0xFF;
    writeData[25] =  cfgPara->thAlarmTemp      & 0xFF;
    writeData[26] = (cfgPara->tranCapacity >>8) & 0xFF;
    writeData[27] =  cfgPara->tranCapacity      & 0xFF;
    
    writeData[28] = (cfgPara->code >>8) & 0xFF;
    writeData[29] =  cfgPara->code      & 0xFF;

    cfgPara->crc = CRC_16(writeData, CONFIG_PARA_DATA_LEN - 2);
    writeData[30] = (cfgPara->crc >> 8) & 0xFF;
    writeData[31] =  cfgPara->crc       & 0xFF;

    if(FRAM_Write_With_CRC(YZTY_CFG_ADDR, writeData, CONFIG_PARA_DATA_LEN) != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK; 
}
/*****************************************************************************
 Function    : Save_Memory_Gear
 Description : None
 Input       : gear
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef  Save_Memory_Gear(uint8_t gear)
{
    uint8_t writeData[3];
    uint16_t crc = 0;
    writeData[0] = gear;
    crc = CRC_16(&gear, 1);
    writeData[1] = crc >> 8;
    writeData[2] = crc & 0xFF; 
    if(FRAM_Write_With_CRC(GEAR_MEMORY_ADDR, writeData, 3) != FRAM_WRITE_READ_OK)
         return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_Reboot_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_Reboot_Counter(void)
{
    FRAM_StatusTypeDef  res = FRAM_WRITE_READ_OK;
    uint8_t  writeData[4] = {0};
    uint16_t rebootNum    = 0;
    uint16_t crc          = 0;

    res = FRAM_Read_With_CRC(REBOOT_COUNTER_ADDR, writeData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    rebootNum = (writeData[0] << 8) + writeData[1];
    rebootNum ++;
    writeData[0] = rebootNum >> 8;
    writeData[1] = rebootNum & 0xFF;
    crc = CRC_16(writeData, 2);
    writeData[2] = crc >> 8;
    writeData[3] = crc & 0xFF;
    res = FRAM_Write_With_CRC(REBOOT_COUNTER_ADDR, writeData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_Switch_Motion
 Description : None
 Input       : clock-The date and time of motion,
               sample-The real time AC sampling,
               beforeMotionSample-The AC sampling before motion,
               sw-The struct of switch,
               mode-The current mode of switch.
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_Switch_Motion(ClockTypeDef          *clock, 
                                        ACSampleTypeDef       *sample,
                                        ACSampleTypeDef       *beforeMotionSample,
                                        SwitchTypeDef         *sw,
                                        uint8_t               mode)
{
    FRAM_StatusTypeDef  res = FRAM_WRITE_READ_OK;
    uint8_t  writeData[SWITCH_MOTION_RECORD_PER_LEN] = {0};
    uint16_t crc           = 0;
    uint16_t recordNum     = 0;
    uint8_t  readData[6] = {0};
    uint32_t recordSum     = 0;
    writeData[0 ] = clock->y;
    writeData[1 ] = clock->m;
    writeData[2 ] = clock->d;
    writeData[3 ] = clock->h;
    writeData[4 ] = clock->min;
    writeData[5 ] = clock->s;
 
    writeData[6 ] = ((uint16_t)(beforeMotionSample->ua * 10.0)) >> 8;
    writeData[7 ] = ((uint16_t)(beforeMotionSample->ua * 10.0)) & 0xFF;
    writeData[8 ] = ((uint16_t)(beforeMotionSample->ub * 10.0)) >> 8;
    writeData[9 ] = ((uint16_t)(beforeMotionSample->ub * 10.0)) & 0xFF;
    writeData[10] = ((uint16_t)(beforeMotionSample->uc * 10.0)) >> 8;
    writeData[11] = ((uint16_t)(beforeMotionSample->uc * 10.0)) & 0xFF;

    writeData[12] = ((uint16_t)(beforeMotionSample->ia * 1000.0)) >> 8;
    writeData[13] = ((uint16_t)(beforeMotionSample->ia * 1000.0)) & 0xFF;
    writeData[14] = ((uint16_t)(beforeMotionSample->ib * 1000.0)) >> 8;
    writeData[15] = ((uint16_t)(beforeMotionSample->ib * 1000.0)) & 0xFF;
    writeData[16] = ((uint16_t)(beforeMotionSample->ic * 1000.0)) >> 8;
    writeData[17] = ((uint16_t)(beforeMotionSample->ic * 1000.0)) & 0xFF;

    writeData[18] = (uint16_t)(beforeMotionSample->pf * 100.0);

    writeData[19] = ((sw->beforeMotionGear)<< 1) | sw->beforeMotionCapa;

    writeData[20] = ((uint16_t)(sample->ua * 10.0)) >> 8;
    writeData[21] = ((uint16_t)(sample->ua * 10.0)) & 0xFF;
    writeData[22] = ((uint16_t)(sample->ub * 10.0)) >> 8;
    writeData[23] = ((uint16_t)(sample->ub * 10.0)) & 0xFF;
    writeData[24] = ((uint16_t)(sample->uc * 10.0)) >> 8;
    writeData[25] = ((uint16_t)(sample->uc * 10.0)) & 0xFF;

    writeData[26] = ((uint16_t)(sample->ia * 1000.0)) >> 8;
    writeData[27] = ((uint16_t)(sample->ia * 1000.0)) & 0xFF;
    writeData[28] = ((uint16_t)(sample->ib * 1000.0)) >> 8;
    writeData[29] = ((uint16_t)(sample->ib * 1000.0)) & 0xFF;
    writeData[30] = ((uint16_t)(sample->ic * 1000.0)) >> 8;
    writeData[31] = ((uint16_t)(sample->ic * 1000.0)) & 0xFF;

    writeData[32] = (uint16_t)(sample->pf * 100.0);
    writeData[33] = ((sw->currentGear)<< 1) | sw->currentCapa;

    writeData[34] = ((sw->lastMotion) << 1) | mode;
    crc = CRC_16(writeData, SWITCH_MOTION_RECORD_PER_LEN - 2);
    writeData[SWITCH_MOTION_RECORD_PER_LEN - 2] = crc >> 8;
    writeData[SWITCH_MOTION_RECORD_PER_LEN - 1] = crc & 0xFF;

    res = FRAM_Read_With_CRC(SWITCH_MOTION_RECORD_NUM_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    recordNum = (readData[0] << 8) + readData[1];
    //存调压记录
    res = FRAM_Write_With_CRC(SWITCH_MOTION_RECORD_ADDR + (recordNum)* SWITCH_MOTION_RECORD_PER_LEN, writeData, SWITCH_MOTION_RECORD_PER_LEN);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    //写当前存储到第几条记录
    if(recordNum < SWITCH_MOTION_RECORD_SUM)
        recordNum++;
    else
        recordNum = 0;
    readData[0] = recordNum >> 8;
    readData[1] = recordNum & 0xFF;
    crc = CRC_16(readData, 2);
    readData[2] = crc >> 8;
    readData[3] = crc & 0xFF;
    res = FRAM_Write_With_CRC(SWITCH_MOTION_RECORD_NUM_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    //写调压总次数
    res = FRAM_Read_With_CRC(SWITCH_MOTION_COUNTER_ADDR, readData, 6);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    recordSum = (readData[0] << 24) + (readData[1] << 16) + (readData[2] << 8) + readData[3];
    recordSum++;
    readData[0] = (recordSum >> 24) & 0xFF;
    readData[1] = (recordSum >> 16) & 0xFF;
    readData[2] = (recordSum >> 8 ) & 0xFF;
    readData[3] = recordSum & 0xFF;
    crc = CRC_16(readData, 4);
    readData[4] = crc >> 8;
    readData[5] = crc & 0xFF;
    res = FRAM_Write_With_CRC(SWITCH_MOTION_COUNTER_ADDR, readData, 6);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_Alarm
 Description : None
 Input       : clock-The date and time of motion,
               sample-The real time AC sampling,
               sw-The struct of switch,
               lds-Last device status,
               cds-Current device status,
               mode-The current mode of switch.
 Output      : None
 Return      : None
 *****************************************************************************/
StorageStatusTypeDef Save_Alarm(ClockTypeDef          *clock,
                                ACSampleTypeDef       *sample,
                                SwitchTypeDef         *sw,
                                uint16_t              lds,
                                uint16_t              cds,
                                uint8_t               mode)
{
    FRAM_StatusTypeDef  res = FRAM_WRITE_READ_OK;
    uint8_t  writeData[ALARM_RECORD_PER_LEN] = {0};
    uint16_t recordNum   = 0;
    uint8_t  readData[4] = {0};
    uint32_t recordSum   = 0;
    uint16_t crc         = 0;
    writeData[0 ] = clock->y;
    writeData[1 ] = clock->m;
    writeData[2 ] = clock->d;
    writeData[3 ] = clock->h;
    writeData[4 ] = clock->min;
    writeData[5 ] = clock->s; 

    writeData[6 ] = ((uint16_t)(sample->ua * 10.0)) >> 8;
    writeData[7 ] = ((uint16_t)(sample->ua * 10.0)) & 0xFF;
    writeData[8 ] = ((uint16_t)(sample->ub * 10.0)) >> 8;
    writeData[9 ] = ((uint16_t)(sample->ub * 10.0)) & 0xFF;
    writeData[10] = ((uint16_t)(sample->uc * 10.0)) >> 8;
    writeData[11] = ((uint16_t)(sample->uc * 10.0)) & 0xFF;

    writeData[12] = ((uint16_t)(sample->ia * 1000.0)) >> 8;
    writeData[13] = ((uint16_t)(sample->ia * 1000.0)) & 0xFF;
    writeData[14] = ((uint16_t)(sample->ib * 1000.0)) >> 8;
    writeData[15] = ((uint16_t)(sample->ib * 1000.0)) & 0xFF;
    writeData[16] = ((uint16_t)(sample->ic * 1000.0)) >> 8;
    writeData[17] = ((uint16_t)(sample->ic * 1000.0)) & 0xFF;
    

    writeData[18] = ((sw->currentGear)<< 1) | sw->currentCapa;
    writeData[19] = mode;

    writeData[20] = lds >> 8;
    writeData[21] = lds & 0xFF;

    writeData[22] = cds >> 8;
    writeData[23] = cds & 0xFF;
    
    crc = CRC_16(writeData, ALARM_RECORD_PER_LEN - 2);
    writeData[ALARM_RECORD_PER_LEN - 2] = crc >> 8;
    writeData[ALARM_RECORD_PER_LEN - 1] = crc & 0xFF;

    res = FRAM_Read_With_CRC(ALARM_RECORD_NUM_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    recordNum = (readData[0] << 8) + readData[1];

    
    res = FRAM_Write_With_CRC(ALARM_RECORD_ADDR + (recordNum )* ALARM_RECORD_PER_LEN, writeData, ALARM_RECORD_PER_LEN);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;

    if(recordNum < ALARM_RECORD_SUM)
        recordNum++;
    else
        recordNum = 0;
    readData[0] = recordNum >> 8;
    readData[1] = recordNum & 0xFF;
    crc = CRC_16(readData, 2);
    readData[2] = crc >> 8;
    readData[3] = crc & 0xFF;
    res = FRAM_Write_With_CRC(ALARM_RECORD_NUM_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
   
    res = FRAM_Read_With_CRC(ALARM_COUNTER_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    recordSum = (readData[0] << 8) + readData[1];
    recordSum++;
    readData[0] = recordSum >> 8;
    readData[1] = recordSum & 0xFF;
    crc = CRC_16(readData, 2);
    readData[2] = crc >> 8;
    readData[3] = crc & 0xFF;
    res = FRAM_Write_With_CRC(ALARM_COUNTER_ADDR, readData, 4);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_Default_Config
 Description : None
 Input       : cfgPara-Config parameter.
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_Default_Config(ConfigParaTypeDef *cfgPara)
{

    uint8_t writeData[64] = {0};

    cfgPara->thDownVoltage        = 233;
    cfgPara->thUpVoltage          = 225;
    cfgPara->tySpace              = 10; 
    cfgPara->tyDelay              = 300;
    cfgPara->ctRatio              = 300; 
    cfgPara->lockOverCurrent      = 400;
    cfgPara->lockVoltageUpLimit   = 240;
    cfgPara->lockVoltageLowLimit  = 198;
    cfgPara->lockHighVoltage      = 280;
    cfgPara->lockLowVoltage       = 150;
    cfgPara->thChangeCapacity     = 80; 
    cfgPara->ccDelay              = 600;
    cfgPara->thAlarmTemp          = 60;
    cfgPara->tranCapacity         = 400;
    cfgPara->code                 = 01;

    cfgPara->deviceInfo.tapTotalNum = GEAR_TOTAL;
    cfgPara->deviceInfo.tapWideHigh = TRAN_TAP_WIDE_HIGH;
    cfgPara->deviceInfo.tapWideLow  = TRAN_TAP_WIDE_LOW;
    cfgPara->deviceInfo.switchType  = SWITCH_TYPE;
    cfgPara->deviceInfo.hardVersion = HARD_VERSION;
    cfgPara->deviceInfo.softVersion = SOFT_VERSION;
         
    writeData[0 ] = (cfgPara->thDownVoltage >>8) & 0xFF;
    writeData[1 ] =  cfgPara->thDownVoltage      & 0xFF;
    writeData[2 ] = (cfgPara->thUpVoltage >>8) & 0xFF;
    writeData[3 ] =  cfgPara->thUpVoltage      & 0xFF;
    writeData[4 ] = (cfgPara->tySpace >>8) & 0xFF;
    writeData[5 ] =  cfgPara->tySpace      & 0xFF;
    writeData[6 ] = (cfgPara->tyDelay >>8) & 0xFF;
    writeData[7 ] =  cfgPara->tyDelay      & 0xFF;
    writeData[8 ] = (cfgPara->ctRatio >>8) & 0xFF;
    writeData[9 ] =  cfgPara->ctRatio      & 0xFF;
    writeData[10] = (cfgPara->lockOverCurrent >>8) & 0xFF;
    writeData[11] =  cfgPara->lockOverCurrent      & 0xFF;
    writeData[12] = (cfgPara->lockVoltageUpLimit >>8) & 0xFF;
    writeData[13] =  cfgPara->lockVoltageUpLimit      & 0xFF;
    writeData[14] = (cfgPara->lockVoltageLowLimit >>8) & 0xFF;
    writeData[15] =  cfgPara->lockVoltageLowLimit      & 0xFF;
    writeData[16] = (cfgPara->lockHighVoltage >>8) & 0xFF;
    writeData[17] =  cfgPara->lockHighVoltage      & 0xFF;
    writeData[18] = (cfgPara->lockLowVoltage >>8) & 0xFF;
    writeData[19] =  cfgPara->lockLowVoltage      & 0xFF;
    writeData[20] = (cfgPara->thChangeCapacity >>8) & 0xFF;
    writeData[21] =  cfgPara->thChangeCapacity      & 0xFF;
    writeData[22] = (cfgPara->ccDelay >>8) & 0xFF;
    writeData[23] =  cfgPara->ccDelay      & 0xFF;
    writeData[24] = (cfgPara->thAlarmTemp >>8) & 0xFF;
    writeData[25] =  cfgPara->thAlarmTemp      & 0xFF;
    writeData[26] = (cfgPara->tranCapacity >>8) & 0xFF;
    writeData[27] =  cfgPara->tranCapacity      & 0xFF;
    writeData[28] = (cfgPara->code >>8) & 0xFF;
    writeData[29] =  cfgPara->code      & 0xFF;

    cfgPara->crc = CRC_16(writeData, CONFIG_PARA_DATA_LEN - 2);

    writeData[30] = (cfgPara->crc >>8) & 0xFF;
    writeData[31] =  cfgPara->crc      & 0xFF;

    if(FRAM_Write_With_CRC(YZTY_CFG_ADDR, writeData, CONFIG_PARA_DATA_LEN) != FRAM_WRITE_READ_OK)
        return STORAGE_WRITE_ERROR;

    if(Clear_Data(SWITCH_MOTION_COUNTER_ADDR, 6) != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    if(Clear_Data(SWITCH_MOTION_RECORD_NUM_ADDR, 4) != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    if(Clear_Data(ALARM_COUNTER_ADDR, 4) != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    if(Clear_Data(ALARM_RECORD_NUM_ADDR, 4) != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    if(Clear_Data(REBOOT_COUNTER_ADDR, 4) != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    
    if(Save_First_Program_Flag() != STORAGE_OK)
        return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Save_First_Program_Flag
 Description : None
 Input       : None
 Output      : None
 Return      : Save status.
 *****************************************************************************/
StorageStatusTypeDef Save_First_Program_Flag(void)
{
    uint8_t writeData[3];
    uint16_t crc = 0;
    writeData[0] = FIRST_PROGRAM_FLAG;
    crc = CRC_16(writeData, 1);
    writeData[1] = crc >> 8;
    writeData[2] = crc & 0xFF; 
    if(FRAM_Write_With_CRC(FIRST_PROGRAM_FLAG_ADDR, writeData, 3) != FRAM_WRITE_READ_OK)
         return STORAGE_WRITE_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_AC_Sample_Calibration_Para
 Description : None
 Input       : calPara-AC sample calibration parameter.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_AC_Sample_Calibration_Para(ACSampleCalibrationParaTypeDef *calPara)
{
    uint8_t readData[64] = {0};
    FRAM_StatusTypeDef res = FRAM_WRITE_READ_OK;
    res = FRAM_Read_With_CRC(CALIBRATION_DATA_ADDR, readData, CALIBRATION_DATA_LEN);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    calPara->ua = (readData[0 ]<< 24) + (readData[1 ]<< 16) + (readData[2 ]<< 8) + readData[3 ];
    calPara->ub = (readData[4 ]<< 24) + (readData[5 ]<< 16) + (readData[6 ]<< 8) + readData[7 ];
    calPara->uc = (readData[8 ]<< 24) + (readData[9 ]<< 16) + (readData[10]<< 8) + readData[11];
    calPara->ia = (readData[12]<< 24) + (readData[13]<< 16) + (readData[14]<< 8) + readData[15];
    calPara->ib = (readData[16]<< 24) + (readData[17]<< 16) + (readData[18]<< 8) + readData[19];
    calPara->ic = (readData[20]<< 24) + (readData[21]<< 16) + (readData[22]<< 8) + readData[23];
    calPara->pa = (readData[24]<< 24) + (readData[25]<< 16) + (readData[26]<< 8) + readData[27];
    calPara->pb = (readData[28]<< 24) + (readData[29]<< 16) + (readData[30]<< 8) + readData[31];
    calPara->pc = (readData[32]<< 24) + (readData[33]<< 16) + (readData[34]<< 8) + readData[35];
    calPara->crc= (readData[36]<< 8)  +  readData[37];
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_AC_Sample_Calibration_Input
 Description : None
 Input       : calPara-AC sample calibration input.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_AC_Sample_Calibration_Input(ACSampleCalibrationInputTypeDef *input)
{
    uint8_t readData[10] = {0};
    FRAM_StatusTypeDef res = FRAM_WRITE_READ_OK;
    res = FRAM_Read_With_CRC(CALIBRATION_INPUT_ADDR, readData, 10);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    input->u = ((readData[0] << 24) + (readData[1] << 16) + (readData[2] << 8) + readData[3])/10.0; 
    input->i = ((readData[4] << 24) + (readData[5] << 16) + (readData[6] << 8) + readData[7])/1000.0; 
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Config_Para
 Description : None
 Input       : cfgPara-Config parameter.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Config_Para(ConfigParaTypeDef *cfgPara)
{
    uint8_t readData[64] = {0};
    FRAM_StatusTypeDef res = FRAM_WRITE_READ_OK;
    res = FRAM_Read_With_CRC(YZTY_CFG_ADDR, readData, CONFIG_PARA_DATA_LEN);
    if(res != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    cfgPara->thDownVoltage        = (readData[0 ]<< 8)  +  readData[1 ];
    cfgPara->thUpVoltage          = (readData[2 ]<< 8)  +  readData[3 ];
    cfgPara->tySpace              = (readData[4 ]<< 8)  +  readData[5 ];
    cfgPara->tyDelay              = (readData[6 ]<< 8)  +  readData[7 ];
    cfgPara->ctRatio              = (readData[8 ]<< 8)  +  readData[9 ];
    cfgPara->lockOverCurrent      = (readData[10]<< 8)  +  readData[11];
    cfgPara->lockVoltageUpLimit   = (readData[12]<< 8)  +  readData[13];
    cfgPara->lockVoltageLowLimit  = (readData[14]<< 8)  +  readData[15];
    cfgPara->lockHighVoltage      = (readData[16]<< 8)  +  readData[17];
    cfgPara->lockLowVoltage       = (readData[18]<< 8)  +  readData[19];
    cfgPara->thChangeCapacity     = (readData[20]<< 8)  +  readData[21];
    cfgPara->ccDelay              = (readData[22]<< 8)  +  readData[23];
    cfgPara->thAlarmTemp          = (readData[24]<< 8)  +  readData[25];
    cfgPara->tranCapacity         = (readData[26]<< 8)  +  readData[27];
    cfgPara->code                 = (readData[28]<< 8)  +  readData[29];
    cfgPara->crc                  = (readData[30]<< 8)  +  readData[31];

    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Memory_Gear
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
 StorageStatusTypeDef Read_Memory_Gear(uint8_t *data)
 {
    if(FRAM_Read_With_CRC(GEAR_MEMORY_ADDR, data, 3) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
 }
 /*****************************************************************************
 Function    : Read_Reboot_Counter
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Reboot_Counter(uint8_t *data)
{
    if(FRAM_Read_With_CRC(REBOOT_COUNTER_ADDR, data, 4) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Switch_Motion_Counter
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Switch_Motion_Counter(uint8_t *data)
{
    if(FRAM_Read_With_CRC(SWITCH_MOTION_COUNTER_ADDR, data, 6) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Switch_Motion_Num
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Switch_Motion_Num(uint8_t *data)
{
    if(FRAM_Read_With_CRC(SWITCH_MOTION_RECORD_NUM_ADDR, data, 4) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_One_Rec
 Description : None
 Input       : None
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_One_Rec(uint16_t addr, uint8_t *data, uint8_t length)
{
    if(FRAM_Read_With_CRC(addr, data, length) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Alarm_Counter
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Alarm_Counter(uint8_t *data)
{
    if(FRAM_Read_With_CRC(ALARM_COUNTER_ADDR, data, 4) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_Alarm_Num
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_Alarm_Num(uint8_t *data)
{
    if(FRAM_Read_With_CRC(ALARM_RECORD_NUM_ADDR, data, 4) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/*****************************************************************************
 Function    : Read_First_Program_Flag
 Description : None
 Input       : data-The pointer of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Read_First_Program_Flag(uint8_t *data)
{
    if(FRAM_Read_With_CRC(FIRST_PROGRAM_FLAG_ADDR, data, 3) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}

/*****************************************************************************
 Function    : Clear_Data
 Description : None
 Input       : addr-The pointer of array.
               length-The length of array.
 Output      : None
 Return      : Read status.
 *****************************************************************************/
StorageStatusTypeDef Clear_Data(uint16_t addr, uint8_t length)
{
    uint16_t crc;
    uint8_t  data[64] = {0};
    crc = CRC_16(data, length - 2);
    data[length - 2] = crc >> 8;
    data[length - 1] = crc & 0xFF;    
    if(FRAM_Write_With_CRC(addr, data, length) != FRAM_WRITE_READ_OK)
        return STORAGE_READ_ERROR;
    return STORAGE_OK;
}
/************************ZXDQ *****END OF FILE****/

