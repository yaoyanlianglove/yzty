/**
  ******************************************************************************
  * File Name          : auto.h
  * Description        : This file provides code for how to auto control.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __auto_H
#define __auto_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "config_para.h"   
#include "telemetry.h" 
#include "remote_signal.h"

void Auto_Control_Clear_Motion(void);
void Auto_Control_Time_Counter(uint32_t tyDelay
#ifdef FUNCTION_TURN_CAPACITY  
    , uint32_t trDelay, uint8_t currentCapa
#endif
    ); 
void Auto_Control_Capa_Clear_Motion(void);   
  
uint8_t Auto_Control(ACSampleTypeDef *sample, ConfigParaTypeDef *configPara
#ifdef FUNCTION_TURN_CAPACITY  
    , RemoteSignalTypeDef *remoteSignal
#endif
    );
#ifdef __cplusplus
}
#endif
#endif 
/************************ZXDQ *****END OF FILE****/

