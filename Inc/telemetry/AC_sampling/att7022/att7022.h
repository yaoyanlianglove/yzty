/**
  ******************************************************************************
  * File Name          : att7022.h
  * Description        : This file provides code for how to use the ATT7022 chip.
  ******************************************************************************
  * @attention
  * The ATT7022 chip adopts SPI communication interface.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __att7022_H
#define __att7022_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "spi.h"

#define  ATT7022_CS_HIGH    HAL_GPIO_WritePin(ATT7022_CS_GPIO_Port, ATT7022_CS_Pin, GPIO_PIN_SET)
#define  ATT7022_CS_LOW     HAL_GPIO_WritePin(ATT7022_CS_GPIO_Port, ATT7022_CS_Pin, GPIO_PIN_RESET)

#define  ATT7022_RESET_HIGH HAL_GPIO_WritePin(ATT7022_RESET_GPIO_Port, ATT7022_RESET_Pin, GPIO_PIN_SET)
#define  ATT7022_RESET_LOW  HAL_GPIO_WritePin(ATT7022_RESET_GPIO_Port, ATT7022_RESET_Pin, GPIO_PIN_RESET)

#define  ATT7022_IRQ_STATE  HAL_GPIO_ReadPin(ATT7022_IRQ_GPIO_Port, ATT7022_IRQ_Pin)
/**
 * @brief  Redefining according to hardware circuit requirements.
 */
#define  HSPI hspi2


#define  ATT7022_SPI_TIMEOUT_VALUE  10
#define  ATT7022_N                  12                          //7022的电流系数
#define  P_K                        0.00176                     //功率系数K=2.592*10^10/(HFconst*EC*2^23)   EC 3200  HFconst 109
#define  HFconst                    0x106
//HFconst计算公式：
//HFConst＝INT[2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib)]
//注：上式中G=1.163，INT[ ]表示取整操作，如：INT[5.68]=5。
//Un 220v Ib 1A
#define  CALIBRATION_DATA_LEN       38
/**
  * @brief  ATT7022 Status structures definition
  */
typedef enum
{
    ATT7022_OK                = 0x00U,
    ATT7022_ERROR             = 0x01U,
    ATT7022_RESET_TIMEOUT     = 0x02U
} ATT7022StatusTypeDef;

/**
  * @brief  ATT7022 calibration parameter structures definition
  */
typedef struct 
{
    uint32_t ua;
    uint32_t ub;
    uint32_t uc;
    uint32_t ia;
    uint32_t ib;
    uint32_t ic;
    uint32_t pa;
    uint32_t pb;
    uint32_t pc;
    uint16_t crc;                
}ACSampleCalibrationParaTypeDef;

/**
  * @brief  ATT7022 calibration input structures definition
  */
typedef struct 
{
    float u;
    float i;
    uint16_t crc;                
}ACSampleCalibrationInputTypeDef;


ATT7022StatusTypeDef ATT7022_Init(ACSampleCalibrationParaTypeDef *calPara, 
                                   ACSampleCalibrationInputTypeDef *input, 
                                   uint32_t timeout);

ATT7022StatusTypeDef ATT7022_Calibration(ACSampleCalibrationParaTypeDef *calPara, 
                                    ACSampleCalibrationInputTypeDef *input);
float Read_U_Data(uint8_t addr);
float Read_I_Data(uint8_t addr);
float Read_P_Data(uint8_t addr, ACSampleCalibrationInputTypeDef *input);
float Read_PT_Data(uint8_t addr, ACSampleCalibrationInputTypeDef *input);
float Read_PF_Data(uint8_t addr);
float Read_TMP_Data(uint8_t addr);
ATT7022StatusTypeDef Read_Check(void);

#ifdef __cplusplus
}
#endif
#endif 
/************************ZXDQ *****END OF FILE****/

