/**
  ******************************************************************************
  * File Name          : ac_sample.c
  * Description        : This file contains all functions about AC-sampling.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */

  
#include "ac_sample.h"


/*****************************************************************************
 Function    : AC_Sample_Init
 Description : None
 Input       : calPara-Calibration parameter, input-AC input during calibration.
 Output      : None
 Return      : The results of AC sampling initialization.
 *****************************************************************************/ 
ACSampleStatusTypeDef AC_Sample_Init(ACSampleCalibrationParaTypeDef *calPara, 
                                     ACSampleCalibrationInputTypeDef *input)
{
    if(ATT7022_Init(calPara, input, 1000) != ATT7022_OK)
        return AC_SAMPLE_INIT_FAIL;
    return AC_SAMPLE_OK;
}
/*****************************************************************************
 Function    : AC_Sample_Calibration
 Description : None
 Input       : calPara-Calibration parameter, input-AC input during calibration.
 Output      : None
 Return      : The results of AC sampling Calibration.
 *****************************************************************************/ 
ACSampleStatusTypeDef AC_Sample_Calibration(ACSampleCalibrationParaTypeDef *calPara, 
                                            ACSampleCalibrationInputTypeDef *input)
{
    if(ATT7022_Calibration(calPara, input) != ATT7022_OK)
        return AC_SAMPLE_CALI_FAIL;
    return AC_SAMPLE_OK;
}
/*****************************************************************************
 Function    : AC_Sample_Calibration
 Description : None
 Input       : calPara-Calibration parameter, input-AC input during calibration.
 Output      : None
 Return      : The results of AC sampling Calibration.
 *****************************************************************************/ 
ACSampleStatusTypeDef AC_Sample_Read(ACSampleTypeDef *sample, 
                                     ACSampleCalibrationInputTypeDef *input)
{

    sample->ua   = Read_U_Data(0x0D);
    sample->ub   = Read_U_Data(0x0E);
    sample->uc   = Read_U_Data(0x0F);
    sample->ia   = Read_I_Data(0x10);
    sample->ib   = Read_I_Data(0x11);
    sample->ic   = Read_I_Data(0x12);
    sample->pa   = Read_P_Data(0x01, input);
    sample->pb   = Read_P_Data(0x02, input);
    sample->pc   = Read_P_Data(0x03, input);
    sample->qa   = Read_P_Data(0x05, input);
    sample->qb   = Read_P_Data(0x06, input);
    sample->qc   = Read_P_Data(0x07, input);
    sample->pf   = Read_PF_Data(0x17);
    sample->freq = Read_U_Data(0x1C);
    sample->tmp  = Read_TMP_Data(0x2A);
    if(Read_Check() != ATT7022_OK)
        return AC_SAMPLE_READ_FAIL;
    return AC_SAMPLE_OK;
}
/************************ZXDQ *****END OF FILE****/
