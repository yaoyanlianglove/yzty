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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __private_comm_interface_H
#define __private_comm_interface_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "private_comm.h"
#include "clock.h"
#include "config_para.h"
#include "remote_signal.h"
#include "storage.h"


void    Private_Comm_Interface_Init(RemoteSignalTypeDef *remoteSignal, 
                                    ConfigParaTypeDef   *configPara,
                                    ClockTypeDef        *clock,
                                    SwitchTypeDef       *sw,
                                    uint16_t            *deviceStat);
uint16_t Private_Comm_Interface_Remote_Control_Process(RemoteSignalTypeDef *remoteSignal, 
                                                       ConfigParaTypeDef   *configPara,
                                                       SwitchTypeDef       *sw,
                                                       uint8_t codeType, uint8_t data);
uint16_t Private_Comm_Interface_Set_Config_Process(RemoteSignalTypeDef *remoteSignal,
                                                   ConfigParaTypeDef   *configPara,
                                                   ClockTypeDef        *clock,
                                                   SaveFlagTypeDef     *saveFlag,
                                                   uint8_t codeType, uint16_t *data, uint8_t length);
uint16_t Private_Comm_Interface_Get_Rec_Process(uint8_t codeType, uint16_t addrNum,  uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

