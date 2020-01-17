/**
  ******************************************************************************
  * File Name          : ac_sample.h
  * Description        : This file contains all functions about AC-sampling.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ac_sample_H
#define __ac_sample_H
#ifdef __cplusplus
extern "C" {
#endif

#include "att7022.h"
/**
  * @brief  AC-sampling data structures definition
  */
typedef struct 
{
    float ua;
    float ub;
    float uc;
    float ia;
    float ib;
    float ic;
    float pa;
    float pb;
    float pc;
    float qa;
    float qb;
    float qc;
    float pf;
    float freq;
    float tmp;             
}ACSampleTypeDef;

/**
  * @brief  AC-sampling status definition
  */
typedef enum
{
    AC_SAMPLE_OK          = 0x00U,
    AC_SAMPLE_INIT_FAIL   = 0x01U,
    AC_SAMPLE_READ_FAIL   = 0x02U,
    AC_SAMPLE_CALI_FAIL   = 0x03U
}ACSampleStatusTypeDef;

ACSampleStatusTypeDef AC_Sample_Init(ACSampleCalibrationParaTypeDef *calPara, 
                                     ACSampleCalibrationInputTypeDef *input);
ACSampleStatusTypeDef AC_Sample_Read(ACSampleTypeDef *sample, 
                                     ACSampleCalibrationInputTypeDef *input);
ACSampleStatusTypeDef AC_Sample_Calibration(ACSampleCalibrationParaTypeDef *calPara, 
                                            ACSampleCalibrationInputTypeDef *input);

#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

