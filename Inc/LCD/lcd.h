/**
  ******************************************************************************
  * File Name          : lcd.h
  * Description        : This file contains all functions about lcd.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __lcd_H
#define __lcd_H
#ifdef __cplusplus
extern "C" {
#endif

#include "clock.h"
#include "switch.h"
#include "remote_signal.h"
#include "telemetry.h"



//LCD KEY---------------------------------------------------------------------
    
#define  LCD_DELAY_TIME      10     //数据读写周期
#define  LCD_BKL_ON_TIMEOUT  30

#define  LCD_BKL_ON      HAL_GPIO_WritePin(LCD_BKL_GPIO_Port, LCD_BKL_Pin, GPIO_PIN_SET)
#define  LCD_BKL_OFF     HAL_GPIO_WritePin(LCD_BKL_GPIO_Port, LCD_BKL_Pin, GPIO_PIN_RESET)
#define  LCD_BKL_STATUS  HAL_GPIO_ReadPin(LCD_BKL_GPIO_Port, LCD_BKL_Pin)

#define  LCD_CS_LOW      HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define  LCD_CS_HIGH     HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define  LCD_RST_LOW     HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define  LCD_RST_HIGH    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

#define  LCD_A0_LOW      HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET)
#define  LCD_A0_HIGH     HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET)

#define  LCD_SCL_LOW     HAL_GPIO_WritePin(LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_PIN_RESET)
#define  LCD_SCL_HIGH    HAL_GPIO_WritePin(LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_PIN_SET)

#define  LCD_SDA_LOW     HAL_GPIO_WritePin(LCD_SDA_GPIO_Port, LCD_SDA_Pin, GPIO_PIN_RESET)
#define  LCD_SDA_HIGH    HAL_GPIO_WritePin(LCD_SDA_GPIO_Port, LCD_SDA_Pin, GPIO_PIN_SET)

/**
  * @brief  LCD KEY type
  */
typedef enum
{
    KEY_TYPE_NONE      = 0x00U,
    KEY_TYPE_HOME      = 0x01U,
    KEY_TYPE_NEXT      = 0x02U,
    KEY_TYPE_UP        = 0x03U,
    KEY_TYPE_DOWN      = 0x04U,
    KEY_TYPE_ENTER     = 0x05U,
    KEY_TYPE_TIMEOUT   = 0x06U
} LCDKeyTypeTypeDef;

/**
  * @brief  LCD status structures definition
  */
typedef struct 
{
    uint8_t  menuSelect;
    uint8_t  gearSelect;               //0 None.1 UP.2 DOWN
    uint8_t  capacitySelect;           //0 None.1 chang
    uint8_t  monitorCurrentPage; 
    uint8_t  controlCurrentPage;  
    uint8_t  historyCurrentPage;
    uint8_t  isRefresh;                //0 false.1 true.
    uint16_t timeCounter;
    uint8_t  flagTimeout;              //长时间无操作标志
    LCDKeyTypeTypeDef currentKeyType;
    LCDKeyTypeTypeDef refreshKeyType;
}LCDStatusTypeDef;


void LCD_Init(void);
LCDKeyTypeTypeDef LCD_Key_Read(LCDButtonSignalTypeDef *lcdButton);
void LCD_Display_Logo(ClockTypeDef *cloc);
void LCD_Display_Menu(uint8_t select, uint8_t isRefresh);
void LCD_Display_RealStatus(SwitchTypeDef *sw, uint16_t version, float oilTemp, uint16_t deviceStatus, uint8_t isRefresh);
void LCD_Display_Control(LCDStatusTypeDef *lcdStatus, SwitchTypeDef *sw);

void LCD_Display_Monitor(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry);
void LCD_Display_Handle_Next_Key(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry, SwitchTypeDef *sw);
void LCD_Display_Handle_Enter_Key(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry, SwitchTypeDef *sw);
void LCD_Display_Handle_Up_Key(LCDStatusTypeDef *lcdStatus, SwitchTypeDef *sw);
void LCD_Display_Handle_Down_Key(LCDStatusTypeDef *lcdStatus, SwitchTypeDef *sw);
#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

