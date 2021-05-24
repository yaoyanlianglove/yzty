/**
  ******************************************************************************
  * File Name          : telemetry.h
  * Description        : This file contains all functions about telemetry.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __telemetry_H
#define __telemetry_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "ac_sample.h"
/**
  * @brief  Telemetry data structures definition
  */
typedef struct 
{
    ACSampleTypeDef sample; 
    ACSampleTypeDef bfSample;
    uint8_t swMotionCnt[6];             //调压次数   
    float oilTmp;          
}TelemetryTypeDef;


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  
