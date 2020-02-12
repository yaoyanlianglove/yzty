/**
  ******************************************************************************
  * File Name          : remote_signal.c
  * Description        : This file contains all functions about remote signal.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "crc.h"
#include "fram_addr_map.h"
#include "fram.h"
#include "remote_signal.h"



volatile  uint16_t oilCount      = 1;
volatile  uint16_t powerOffCount = 1;
volatile  uint16_t handEnBCT     = 1;
volatile  uint16_t handUpBCT     = 1;
volatile  uint16_t handDwBCT     = 1;
volatile  uint16_t handCpBCT     = 1;
volatile  uint16_t lcdHomeBCT    = 1;
volatile  uint16_t lcdNextBCT    = 1;
volatile  uint16_t lcdUpBCT      = 1;
volatile  uint16_t lcdDownBCT    = 1;
volatile  uint16_t lcdEnterBCT   = 1;

volatile  uint8_t  flagHandBtOK    = 0;
volatile  uint16_t handPeriodCount = 0;
volatile  uint8_t  flagLcdBtOK     = 0;
volatile  uint16_t lcdPeriodCount  = 0;

/*****************************************************************************
 Function    : Remote_Signal_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Remote_Signal_Init(RemoteSignalTypeDef *remoteSignal)
{
    remoteSignal->autoMode    = 1;       //默认自动模式
    remoteSignal->oilPosAlarm = 1;       //默认油位告警
}
/*****************************************************************************
 Function    : LCD_Button_Time_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void LCD_Button_Time_Counter(LCDButtonSignalTypeDef *lcdButton)
{
    if(flagHandBtOK == 1)
        return;
    if(REMOTE_SIGNAL_LCD_ENTER == 0)
    {
        if(lcdEnterBCT < BUTTON_CONFIRM_TIME)
            lcdEnterBCT++;
        else
            lcdButton->enter = 1;
    }
    else
        lcdEnterBCT = 0;
    if(REMOTE_SIGNAL_LCD_UP == 0)
    {
        if(lcdUpBCT < BUTTON_CONFIRM_TIME)
            lcdUpBCT++;
        else
            lcdButton->up = 1;
    }
    else
        lcdUpBCT = 0;
    if(REMOTE_SIGNAL_LCD_DOWN == 0)
    {
        if(lcdDownBCT < BUTTON_CONFIRM_TIME)
            lcdDownBCT++;
        else
            lcdButton->down = 1;
    }
    else
        lcdDownBCT = 0;
    if(REMOTE_SIGNAL_LCD_HOME == 0)
    {
        if(lcdHomeBCT < BUTTON_CONFIRM_TIME)
            lcdHomeBCT++;
        else
            lcdButton->home = 1;
    }
    else
        lcdHomeBCT = 0;
    if(REMOTE_SIGNAL_LCD_NEXT == 0)
    {
        if(lcdNextBCT < BUTTON_CONFIRM_TIME)
            lcdNextBCT++;
        else
            lcdButton->next = 1;
    }
    else
        lcdNextBCT = 0;

    if(lcdPeriodCount < BUTTON_HANDLE_PERIOD_TIME)
        lcdPeriodCount++;
    else
    {
        lcdPeriodCount = 0;
        if(lcdButton->home + lcdButton->next + lcdButton->up + lcdButton->down + lcdButton->enter == 1)
            flagLcdBtOK = 1;
        lcdHomeBCT    = 0;
        lcdNextBCT    = 0;
        lcdUpBCT      = 0;
        lcdDownBCT    = 0;
        lcdEnterBCT   = 0;
    }
}
/*****************************************************************************
 Function    : Hand_Button_Time_Counter
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static void Hand_Button_Time_Counter(HandButtonSignalTypeDef *handButton)
{
    if(flagHandBtOK == 1)
        return;
    if(REMOTE_SIGNAL_HAND_EN == 0)
    {
        if(handEnBCT < BUTTON_CONFIRM_TIME)
            handEnBCT++;
        else
            handButton->en = 1;
    }
    else
        handEnBCT = 0;
    if(REMOTE_SIGNAL_HAND_UP == 0)
    {
        if(handUpBCT < BUTTON_CONFIRM_TIME)
            handUpBCT++;
        else
            handButton->up = 1;
    }
    else
        handUpBCT = 0;
    if(REMOTE_SIGNAL_HAND_DW == 0)
    {
        if(handDwBCT < BUTTON_CONFIRM_TIME)
            handDwBCT++;
        else
            handButton->down = 1;
    }
    else
        handDwBCT = 0;
    if(REMOTE_SIGNAL_HAND_CP == 0)
    {
        if(handCpBCT < BUTTON_CONFIRM_TIME)
            handCpBCT++;
        else
            handButton->cp = 1;
    }
    else
        handCpBCT = 0;

    if(handPeriodCount < BUTTON_HANDLE_PERIOD_TIME)
        handPeriodCount++;
    else
    {
        handPeriodCount = 0;
        if(handButton->en == 1)
        {
            if((handButton->up == 1 && handButton->cp == 0 && handButton->down == 0) ||
               (handButton->up == 0 && handButton->cp == 1 && handButton->down == 0) ||
               (handButton->up == 0 && handButton->cp == 0 && handButton->down == 1))
            {
                flagHandBtOK = 1;
            } 
        } 
        handEnBCT        = 0;
        handUpBCT        = 0;
        handDwBCT        = 0;
        handCpBCT        = 0;
    }
}
/*****************************************************************************
 Function    : Remote_Signal_Time_Counter
 Description : Read remote signal data.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Remote_Signal_Time_Counter(RemoteSignalTypeDef *remoteSignal)
{
    //油位
    if(remoteSignal->oilPosAlarm != REMOTE_SIGNAL_OIL_POS)
    {
        if(oilCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            oilCount++;
        else
        {
            remoteSignal->oilPosAlarm = REMOTE_SIGNAL_OIL_POS;
        }
    }
    else 
        oilCount = 0;
    //主电源失电
    if(remoteSignal->powerOffAlarm == REMOTE_SIGNAL_POWER_CHECK)
    {
        if(powerOffCount < REMOTE_SIGNAL_SUB_DITH_TIME)
            powerOffCount++;
        else
        {
            remoteSignal->powerOffAlarm = REMOTE_SIGNAL_POWER_CHECK ^ 1;
        }
    }
    else 
        powerOffCount = 0; 
    Gear_Signal_Time_Counter(&(remoteSignal->gearSignal));
    Hand_Button_Time_Counter(&(remoteSignal->handButton));
    LCD_Button_Time_Counter(&(remoteSignal->lcdButton));
}

/************************ZXDQ *****END OF FILE****/

