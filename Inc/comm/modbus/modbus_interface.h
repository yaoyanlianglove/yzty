/**
  ******************************************************************************
  * File Name          : modbus_interface.h
  * Description        : This file provides the function about interface of modbus.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __modbus_interface_H
#define __modbus_interface_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "modbus.h"
#include "clock.h"
#include "config_para.h"
#include "remote_signal.h"
#include "storage.h"


void    Modbus_Interface_Init(RemoteSignalTypeDef *remoteSignal, ConfigParaTypeDef *configPara,
                      ClockTypeDef *clock);
uint8_t Modbus_Interface_Remote_Control_Process(RemoteSignalTypeDef *remoteSignal, 
                                                ConfigParaTypeDef   *configPara,
                                                SwitchTypeDef       *sw,
                                                uint8_t regAddr, uint8_t *data, uint8_t regNum);
uint8_t Modbus_Interface_Set_Config_Process(RemoteSignalTypeDef *remoteSignal, 
                                            ConfigParaTypeDef   *configPara,
                                            ClockTypeDef        *clock,
                                            SaveFlagTypeDef     *saveFlag,
                                            uint8_t regAddr, uint16_t *data, uint8_t regNum);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

