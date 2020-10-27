/**
  ******************************************************************************
  * File Name          : lcd.c
  * Description        : This file contains all functions about lcd.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "lcd.h"
#include "font.h"
#include "debug.h"
#include <stdlib.h>
#include "config_para.h"
extern volatile  uint8_t  flagLcdBtOK;

/*****************************************************************************
 Function    : LCD_Delay
 Description : Delay function for LCD.
 Input       : num
 Output      : None
 Return      : None
 *****************************************************************************/   
static void LCD_Delay(uint32_t num)
{
    uint16_t i;
    for(i=0; i<num; i++)
        ; 
}
/*****************************************************************************
 Function    : LCD_Write_Byte
 Description : Write byte data to LCD.
 Input       : byte, the data.
               type, 1 display 0 control.
 Output      : None
 Return      : None
 *****************************************************************************/ 
static void LCD_Write_Byte(uint8_t byte, uint8_t type)  
{
    uint8_t i;
    if(type == 1)
        LCD_A0_HIGH;
    else
        LCD_A0_LOW; 
    for(i=0; i<8; i++)
    {
        LCD_SCL_LOW;
        LCD_Delay(LCD_DELAY_TIME);
        if((byte & 0x80) == 0x80)
            LCD_SDA_HIGH;
        else
            LCD_SDA_LOW;
        byte <<= 1;
        LCD_Delay(LCD_DELAY_TIME);
        LCD_SCL_HIGH;
        LCD_Delay(LCD_DELAY_TIME);
    }          
}
/*****************************************************************************
 Function    : LCD_Write_Cmd
 Description : Write command to LCD.
 Input       : cmd, the pointer of cmd data.
               length, the length of cmd data.
 Output      : None
 Return      : None
 *****************************************************************************/ 
static void LCD_Write_Cmd(uint8_t *cmd, uint8_t length)
{
    uint8_t i;
    for(i=0; i<length; i++)    
    {
        LCD_Write_Byte(cmd[i], 0);
    }
}
/*****************************************************************************
 Function    : LCD_Display_Zimu
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static void LCD_Display_Zimu(uint8_t line, uint8_t column, uint8_t *dat, uint8_t length)
{
    uint8_t i;                
    uint8_t page, col_L, col_H;
    page = 0xB0 | line;
    col_L  = column & 0x0f;          
    col_H  = ((column>>4) & 0x0f) | 0x10;
    LCD_Write_Byte(page,  0);          
    LCD_Write_Byte(0x40,  0);             
    LCD_Write_Byte(col_H, 0);            
    LCD_Write_Byte(col_L, 0);          
    for(i=0; i<length; i++)
    {   
        LCD_Write_Byte(*dat, 1);
        dat++;
    }
}  
/*****************************************************************************
 Function    : LCD_Clear_Screen
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static void LCD_Clear_Screen(uint8_t lineBegin, uint8_t lineEnd, uint8_t columnBegin, uint8_t columnEnd)
{
    uint8_t i, page;
    uint8_t col_L, col_H;
    col_L  = columnBegin & 0x0f;          
    col_H  = ((columnBegin>>4) & 0x0f) | 0x10;
    for(page = lineBegin; page < lineEnd + 1; page++)
    {
        LCD_Write_Byte(0xB0|page, 0);        //页起始地址0xB0
        LCD_Write_Byte(0x40, 0);             //显示开始地址
        LCD_Write_Byte(col_H, 0);            //列地址高位MSB
        LCD_Write_Byte(col_L, 0);            //列地址低位LSB
        for (i=0; i<columnEnd; i++)          //行只有65行，前7页共有64行
        {                                    //第8页只有1行
            LCD_Write_Byte(0, 1);
        }
    }
}
/*****************************************************************************
 Function    : LCD_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/   
void LCD_Init(void)
{
    uint8_t LCD_Cmd[9]={0xe2,0x2f,0x23,0xa2,0xa0,0xc8,0xaf,0xf8,0x01};
    //0xe2 软件复位 0x2f 内置电源控制开启 0x23 调节率RR值4.5 0xa2 1/9偏置  0xa0 横向扫描普通显示 
    //0xc8 竖直上下颠倒扫描模式 0xaf 打开显示 0xf8 0x01 设置倍压
    LCD_BKL_ON;                                         
    LCD_CS_LOW;                        //片选，低电平使能
    LCD_RST_LOW;                       //硬件复位,等待>5
    LCD_Delay(LCD_DELAY_TIME);
    LCD_RST_HIGH;                      //关闭硬件复位,等
    LCD_Delay(LCD_DELAY_TIME);
    LCD_Write_Cmd(LCD_Cmd, 9);         //SEG ,1/9偏置,电源控制 
    LCD_Write_Byte(0x24, 0);           //对比度粗调，设置RR5.0
    LCD_Write_Byte(0x81, 0);           
    LCD_Write_Byte(0x20, 0);           //对比度微调，设置EV
    LCD_Clear_Screen(0, 7, 0, 128);
}
/*****************************************************************************
 Function    : LCD_Display_One_Hanzi
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static void LCD_Display_One_Hanzi(uint8_t type, uint8_t line, uint8_t column, uint8_t *dat, uint8_t length)
{
    if(type == 0)//字母数字
    {
        LCD_Display_Zimu(line,   column, dat-16, 8);
        LCD_Display_Zimu(line+1, column, dat-8 , 8); 
    }
    else          //汉字
    {
        LCD_Display_Zimu(line,   column, dat-16, length);
        LCD_Display_Zimu(line+1, column, dat,    length);
    }
}
/*****************************************************************************
 Function    : LCD_Key_Read
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LCDKeyTypeTypeDef LCD_Key_Read(LCDButtonSignalTypeDef* lcdButton)
{
    LCDKeyTypeTypeDef keyType = KEY_TYPE_NONE;
    if(flagLcdBtOK == 1)
    {
        if(lcdButton->home == 1)
            keyType = KEY_TYPE_HOME;
        else if(lcdButton->next == 1)
            keyType = KEY_TYPE_NEXT;
        else if(lcdButton->up == 1)
            keyType = KEY_TYPE_UP;
        else if(lcdButton->down == 1)
            keyType = KEY_TYPE_DOWN;
        else if(lcdButton->enter == 1)
            keyType = KEY_TYPE_ENTER;
    }
    lcdButton->home  = 0;
    lcdButton->next  = 0;
    lcdButton->up    = 0;
    lcdButton->down  = 0;
    lcdButton->enter = 0;

    return keyType;
}
/*****************************************************************************
 Function    : LCD_Refresh_Screen
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static void LCD_Refresh_Screen()
{
    LCD_Clear_Screen(0 , 7 , 0, 128);
}

/*****************************************************************************
 Function    : LCD_Display_Menu
 Description : None
 Input       : select--0 monitor 1 control 2 history
 Output      : None
 Return      : None
 *****************************************************************************/  
void LCD_Display_Menu(uint8_t select, uint8_t isRefresh)
{
    if(isRefresh == 0)
    {
        LCD_Refresh_Screen();
        LCD_Display_One_Hanzi(1, 0, 44, &FONT_Chinese[0 ][16], 16);
        LCD_Display_One_Hanzi(1, 0, 60, &FONT_Chinese[2 ][16], 16);

        LCD_Display_One_Hanzi(0, select*2 +2, 4 , &FONT_Characters[2][16],16);
        LCD_Display_One_Hanzi(0, select*2 +2, 12, &FONT_Characters[3][16],16);
        LCD_Display_One_Hanzi(0, 2, 20, &FONT_Number[1 ][16], 16); 
        LCD_Display_One_Hanzi(0, 2, 28, &FONT_Characters[0][16], 16);
        LCD_Display_One_Hanzi(1, 2, 36, &FONT_Chinese[ 4][16], 16); 
        LCD_Display_One_Hanzi(1, 2, 52, &FONT_Chinese[ 6][16], 16);
        LCD_Display_One_Hanzi(1, 2, 68, &FONT_Chinese[ 8][16], 16);
        LCD_Display_One_Hanzi(1, 2, 84, &FONT_Chinese[10][16], 16);

    
        LCD_Display_One_Hanzi(0, 4, 20, &FONT_Number[2 ][16], 16); 
        LCD_Display_One_Hanzi(0, 4, 28, &FONT_Characters[0][16], 16);
        LCD_Display_One_Hanzi(1, 4, 36, &FONT_Chinese[12][16], 16); 
        LCD_Display_One_Hanzi(1, 4, 52, &FONT_Chinese[14][16], 16);
        LCD_Display_One_Hanzi(1, 4, 68, &FONT_Chinese[16][16], 16);
        LCD_Display_One_Hanzi(1, 4, 84, &FONT_Chinese[18][16], 16);
    }
}
/*****************************************************************************
 Function    : Font_Number_Array
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t Font_Number_Array(uint8_t num)
{
    uint8_t res = 0;
    switch(num)
    {
        case 0:
        res = 0;
        break;
        case 1:
        res = 1;
        break;
        case 2:
        res = 2;
        break;
        case 3:
        res = 3;
        break;
        case 4:
        res = 4;
        break;
        case 5:
        res = 5;
        break;
        case 6:
        res = 6;
        break;
        case 7:
        res = 7;
        break;
        case 8:
        res = 8;
        break;
        case 9:
        res = 9;
        break;
        default:;
    }
    return res;
}
/*****************************************************************************
 Function    : LCD_Display_RealStatus
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_RealStatus(SwitchTypeDef *sw, uint16_t version, float oilTemp, uint16_t deviceStatus, uint8_t isRefresh)
{
    int16_t temp;
    temp = (int16_t)(oilTemp*10.0);
    if(isRefresh == 0)
    {
        LCD_Refresh_Screen();
        LCD_Display_One_Hanzi(0, 0, 22 , &FONT_Capital[21][16],16);//V
        LCD_Display_One_Hanzi(0, 0, 30, &FONT_Number[Font_Number_Array(version/1000) ][16],16);
        LCD_Display_One_Hanzi(0, 0, 38, &FONT_Characters[0][16],16);//.
        LCD_Display_One_Hanzi(0, 0, 46, &FONT_Number[Font_Number_Array((version%1000)/100) ][16],16);
        LCD_Display_One_Hanzi(0, 0, 54, &FONT_Characters[0][16],16);
        LCD_Display_One_Hanzi(0, 0, 62, &FONT_Number[Font_Number_Array((version%100)/10) ][16],16);
        LCD_Display_One_Hanzi(0, 0, 70, &FONT_Characters[0][16],16);
        LCD_Display_One_Hanzi(0, 0, 78, &FONT_Number[Font_Number_Array(version%10) ][16],16);

        LCD_Display_One_Hanzi(1, 2, 12, &FONT_Chinese[30][16], 16); 
        LCD_Display_One_Hanzi(1, 2, 28, &FONT_Chinese[32][16], 16);
        LCD_Display_One_Hanzi(0, 2, 44, &FONT_Characters[1][16], 16);

        LCD_Display_One_Hanzi(1, 4, 12, &FONT_Chinese[34][16], 16);
        LCD_Display_One_Hanzi(1, 4, 28, &FONT_Chinese[36][16], 16);
        LCD_Display_One_Hanzi(0, 4, 44, &FONT_Characters[1][16], 16);

        LCD_Display_One_Hanzi(1, 6, 02, &FONT_Chinese[38][16], 16);
        LCD_Display_One_Hanzi(1, 6, 18, &FONT_Chinese[40][16], 16);
        LCD_Display_One_Hanzi(0, 6, 34, &FONT_Characters[1][16],16);

        LCD_Display_One_Hanzi(1, 6, 62, &FONT_Chinese[28][16], 16);
        LCD_Display_One_Hanzi(1, 6, 78, &FONT_Chinese[10][16], 16);
        LCD_Display_One_Hanzi(0, 6, 94, &FONT_Characters[1][16],16);
    }
    if(temp < 0)
        LCD_Display_One_Hanzi(0, 2, 52, &FONT_Characters[2][16], 16);
    else
        LCD_Display_One_Hanzi(0, 2, 52, &FONT_Cap[0][16], 16);
        

    if(abs(temp) > 999 && abs(temp) < 10000)
        LCD_Display_One_Hanzi(0, 2, 60, &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 2, 60, &FONT_Cap[0][16], 16);
    if(abs(temp) > 99)
        LCD_Display_One_Hanzi(0, 2, 68, &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 2, 68, &FONT_Cap[0][16], 16);
    if(abs(temp) > 9)
        LCD_Display_One_Hanzi(0, 2, 76, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 2, 76, &FONT_Cap[0][16], 16);
    if(abs(temp) > 0)
        LCD_Display_One_Hanzi(0, 2, 84, &FONT_Characters[0][16], 16);
    else
        LCD_Display_One_Hanzi(0, 2, 84, &FONT_Cap[0][16], 16);
    LCD_Display_One_Hanzi(0, 2, 90, &FONT_Number[Font_Number_Array(temp%10)][16], 16);

    if(deviceStatus > 9999 && deviceStatus < 100000)
        LCD_Display_One_Hanzi(0, 4, 60, &FONT_Number[Font_Number_Array(deviceStatus/10000)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 4, 60, &FONT_Cap[0][16], 16);
    if(deviceStatus > 999)
        LCD_Display_One_Hanzi(0, 4, 68, &FONT_Number[Font_Number_Array((deviceStatus%10000)/1000)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 4, 68, &FONT_Cap[0][16], 16);
    if(deviceStatus > 99)
        LCD_Display_One_Hanzi(0, 4, 76, &FONT_Number[Font_Number_Array((deviceStatus%1000)/100)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 4, 76, &FONT_Cap[0][16], 16);
    if(deviceStatus > 9)
        LCD_Display_One_Hanzi(0, 4, 84, &FONT_Number[Font_Number_Array((deviceStatus%100)/10)][16], 16);
    else
        LCD_Display_One_Hanzi(0, 4, 84, &FONT_Cap[0][16], 16);

    LCD_Display_One_Hanzi(0, 4, 90, &FONT_Number[Font_Number_Array((deviceStatus%10)/1)][16], 16);

    if(sw->currentGear < 20)
    {
        LCD_Display_One_Hanzi(0, 6, 42, &FONT_Number[Font_Number_Array(sw->currentGear/10)][16], 16);
        LCD_Display_One_Hanzi(0, 6, 50, &FONT_Number[Font_Number_Array(sw->currentGear%10)][16], 16); 
    }
    else
    {
        LCD_Display_One_Hanzi(0, 6, 42, &FONT_Capital[5][16], 16);
        LCD_Display_One_Hanzi(0, 6, 50, &FONT_Capital[5][16], 16);
    } 
    
    if(sw->currentCapa == 0)
        LCD_Display_One_Hanzi(1, 6, 102, &FONT_Chinese[42][16], 16);  
    else if(sw->currentCapa == 1)
        LCD_Display_One_Hanzi(1, 6, 102, &FONT_Chinese[44][16], 16); 
}
/*****************************************************************************
 Function    : LCD_Display_Monitor
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Monitor(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry)
{
    uint32_t temp = 0;
    int16_t  oilTemp = 0;
    int16_t  envTemp = 0;
    
    uint8_t  i;
    uint8_t  sign;
    
    if(lcdStatus->isRefresh == 0)
    {
        LCD_Refresh_Screen();
        LCD_Display_One_Hanzi(1, 0, 36, &FONT_Chinese[ 4][16], 16); 
        LCD_Display_One_Hanzi(1, 0, 52, &FONT_Chinese[ 6][16], 16);
        LCD_Display_One_Hanzi(1, 0, 68, &FONT_Chinese[ 8][16], 16);
        LCD_Display_One_Hanzi(1, 0, 84, &FONT_Chinese[10][16], 16);
    }
    
    switch(lcdStatus->monitorCurrentPage)
    {
        case 1:
            for(i = 1 ; i < 4; i++)
            {
                if(lcdStatus->isRefresh == 0)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 4 , &FONT_Capital[20][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 12, &FONT_Capital[i-1 ][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 20, &FONT_Characters[1][16], 16);
                }
                if(i == 1 && telemetry->sample.ua < 300)
                    temp = (uint32_t)(telemetry->sample.ua*10);
                else if(i == 2 && telemetry->sample.ub < 300)
                    temp = (uint32_t)(telemetry->sample.ub*10);
                else if(i == 3 && telemetry->sample.uc < 300)
                    temp = (uint32_t)(telemetry->sample.uc*10);
                if(temp < 9999)
				{
                    LCD_Display_One_Hanzi(0, 2*i, 36, &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 44, &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 52, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 60, &FONT_Characters[0][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 68, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
				}
                
            }
        break;
        case 2:
            for(i = 1 ; i < 4; i++)
            {
                if(lcdStatus->isRefresh == 0)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 4 , &FONT_Capital[8][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 12, &FONT_Capital[i-1 ][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 20, &FONT_Characters[1][16], 16);
                }
                
                if(i == 1 && telemetry->sample.ia < 10)
                    temp = (uint32_t)(telemetry->sample.ia*1000);
                else if(i == 2 && telemetry->sample.ib < 10)
                    temp = (uint32_t)(telemetry->sample.ib*1000);
                else if(i == 3 && telemetry->sample.ic < 10)
                    temp = (uint32_t)(telemetry->sample.ic*1000);
                if(temp < 9999)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 36, &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 44, &FONT_Characters[0][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 52, &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 60, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 68, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
                } 
            }
        break;
        case 3:
            for(i = 1 ; i < 4; i++)
            {
                if(lcdStatus->isRefresh == 0)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 4 , &FONT_Capital[15][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 12, &FONT_Capital[i-1 ][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 20, &FONT_Characters[1][16], 16);
                }
                if(i == 1)
                {
                    if(telemetry->sample.pa < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.pa);
                }
                else if(i == 2)
                {
                    if(telemetry->sample.pb < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.pb);
                }
                else if(i == 3)
                {
                    if(telemetry->sample.pc < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.pc);
                }
                if(sign == 1)
                    LCD_Display_One_Hanzi(0, 2*i, 28, &FONT_Characters[2][16], 16);
                else
                    LCD_Display_One_Hanzi(0, 2*i, 28, &FONT_Cap[0][16], 16);
                if(temp < 9999)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 36, &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 44, &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 52, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 60, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
                }
            }
        break;
        case 4:
            for(i = 1 ; i < 4; i++)
            {
                if(lcdStatus->isRefresh == 0)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 4 , &FONT_Capital[16][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 12, &FONT_Capital[i-1 ][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 20, &FONT_Characters[1][16], 16);
                }
                
                if(i == 1)
                {
                    if(telemetry->sample.qa < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.qa);
                }
                else if(i == 2)
                {
                    if(telemetry->sample.qb < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.qb);
                }
                else if(i == 3)
                {
                    if(telemetry->sample.qc < 0)
                        sign = 1;
                    else
                        sign = 0;
                    temp = (uint32_t)(telemetry->sample.qc);
                }
                if(sign == 1)
                    LCD_Display_One_Hanzi(0, 2*i, 28, &FONT_Characters[2][16], 16);
                else
                    LCD_Display_One_Hanzi(0, 2*i, 28, &FONT_Cap[0][16], 16);
                if(temp < 9999)
                {
                    LCD_Display_One_Hanzi(0, 2*i, 36, &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 44, &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 52, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                    LCD_Display_One_Hanzi(0, 2*i, 60, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
                } 
            }
        break;
        case 5:
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 2, 4, &FONT_Chinese[50][16], 16);
                LCD_Display_One_Hanzi(1, 2, 20, &FONT_Chinese[52][16], 16);
                LCD_Display_One_Hanzi(1, 2, 36, &FONT_Chinese[54][16], 16);
                LCD_Display_One_Hanzi(1, 2, 52, &FONT_Chinese[56][16], 16);
                LCD_Display_One_Hanzi(0, 2, 68, &FONT_Characters[1][16], 16);
            }
            if(telemetry->sample.pf < 0)
                sign = 1;
            else
                sign = 0;
            if(telemetry->sample.pf < 2)
                temp = (uint32_t)(telemetry->sample.pf*1000);
            if(sign == 1)
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Characters[2][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Cap[0][16], 16);
            if(temp < 9999)
            {
                LCD_Display_One_Hanzi(0, 2, 84,  &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                LCD_Display_One_Hanzi(0, 2, 92,  &FONT_Characters[0][16], 16);
                LCD_Display_One_Hanzi(0, 2, 100,  &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                LCD_Display_One_Hanzi(0, 2, 108, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                LCD_Display_One_Hanzi(0, 2, 116, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
            }
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 4, 4, &FONT_Chinese[58][16], 16);
                LCD_Display_One_Hanzi(1, 4, 52, &FONT_Chinese[52][16], 16);
                LCD_Display_One_Hanzi(0, 4, 68, &FONT_Characters[1][16], 16);
            }
            if(telemetry->sample.freq < 100)
                temp = (uint32_t)(telemetry->sample.freq*100);
            
            if(temp < 9999)
            {
                LCD_Display_One_Hanzi(0, 4, 76,  &FONT_Number[Font_Number_Array(temp/1000)][16], 16);
                LCD_Display_One_Hanzi(0, 4, 84,  &FONT_Number[Font_Number_Array((temp%1000)/100)][16], 16);
                LCD_Display_One_Hanzi(0, 4, 92,  &FONT_Characters[0][16], 16);
                LCD_Display_One_Hanzi(0, 4, 100, &FONT_Number[Font_Number_Array((temp%100)/10)][16], 16);
                LCD_Display_One_Hanzi(0, 4, 108, &FONT_Number[Font_Number_Array(temp%10)][16], 16);
            }
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 6, 4, &FONT_Chinese[46][16], 16);
                LCD_Display_One_Hanzi(1, 6, 20, &FONT_Chinese[62][16], 16);
                LCD_Display_One_Hanzi(1, 6, 36, &FONT_Chinese[32][16], 16);
                LCD_Display_One_Hanzi(1, 6, 52, &FONT_Chinese[48][16], 16);
                LCD_Display_One_Hanzi(0, 6, 68, &FONT_Characters[1][16], 16);
            }
            
            envTemp = (int16_t)(telemetry->sample.tmp*10);
            if(envTemp < 0)
                LCD_Display_One_Hanzi(0, 6, 76, &FONT_Characters[2][16], 16);
            else
                LCD_Display_One_Hanzi(0, 6, 76, &FONT_Cap[0][16], 16);
            if(abs(envTemp) > 999)
                LCD_Display_One_Hanzi(0, 6, 84, &FONT_Number[Font_Number_Array(envTemp/1000)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 6, 84, &FONT_Cap[0][16], 16);
            if(abs(envTemp) > 99)
                LCD_Display_One_Hanzi(0, 6, 92, &FONT_Number[Font_Number_Array((envTemp%1000)/100)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 6, 92, &FONT_Cap[0][16], 16);
            if(abs(envTemp) > 9)
                LCD_Display_One_Hanzi(0, 6, 100, &FONT_Number[Font_Number_Array((envTemp%100)/10)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 6, 100, &FONT_Cap[0][16], 16);
            if(abs(envTemp) > 0)
                LCD_Display_One_Hanzi(0, 6, 108, &FONT_Characters[0][16], 16);
            else
                LCD_Display_One_Hanzi(0, 6, 108, &FONT_Cap[0][16], 16);
            LCD_Display_One_Hanzi(0, 6, 116, &FONT_Number[Font_Number_Array(envTemp%10)][16], 16);
        break;
        case 6:
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 2, 4, &FONT_Chinese[30][16], 16);
                LCD_Display_One_Hanzi(1, 2, 20, &FONT_Chinese[32][16], 16);
                LCD_Display_One_Hanzi(0, 2, 36, &FONT_Characters[1][16], 16);
            } 
            oilTemp = (int16_t)(telemetry->oilTmp*10.0);
            if(oilTemp < 0)
                LCD_Display_One_Hanzi(0, 2, 44, &FONT_Characters[2][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 44, &FONT_Cap[0][16], 16);
            if(abs(oilTemp) > 999 && abs(oilTemp) < 10000)
                LCD_Display_One_Hanzi(0, 2, 52, &FONT_Number[Font_Number_Array(oilTemp/1000)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 52, &FONT_Cap[0][16], 16);
            if(abs(oilTemp) > 99)
                LCD_Display_One_Hanzi(0, 2, 60, &FONT_Number[Font_Number_Array((oilTemp%1000)/100)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 60, &FONT_Cap[0][16], 16);
            if(abs(oilTemp) > 9)
                LCD_Display_One_Hanzi(0, 2, 68, &FONT_Number[Font_Number_Array((oilTemp%100)/10)][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 68, &FONT_Cap[0][16], 16);
            if(abs(oilTemp) > 0)
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Characters[0][16], 16);
            else
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Cap[0][16], 16);
            LCD_Display_One_Hanzi(0, 2, 104, &FONT_Number[Font_Number_Array(oilTemp%10)][16], 16);  
        break;
        default:;
        break;
    }
}
/*****************************************************************************
 Function    : LCD_Display_Control
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
void LCD_Display_Control(LCDStatusTypeDef *lcdStatus, SwitchTypeDef *sw)
{
    uint8_t d;
    d = sw->currentGear;
    if(lcdStatus->isRefresh == 0)
    {
        LCD_Refresh_Screen();
        LCD_Display_One_Hanzi(1, 0, 36, &FONT_Chinese[12][16], 16); 
        LCD_Display_One_Hanzi(1, 0, 52, &FONT_Chinese[14][16], 16);
        LCD_Display_One_Hanzi(1, 0, 68, &FONT_Chinese[16][16], 16);
        LCD_Display_One_Hanzi(1, 0, 84, &FONT_Chinese[18][16], 16);
    }

    switch(lcdStatus->controlCurrentPage)
    {
        case 1:
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(0, lcdStatus->gearSelect*2 +2, 4 , &FONT_Characters[2][16],16);
                LCD_Display_One_Hanzi(0, lcdStatus->gearSelect*2 +2, 12, &FONT_Characters[3][16],16);
    
                LCD_Display_One_Hanzi(1, 2, 20, &FONT_Chinese[38][16], 16);
                LCD_Display_One_Hanzi(1, 2, 36, &FONT_Chinese[40][16], 16);
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Characters[1 ][16], 16);
            }
            
            
            if(d < 10)
            {
                LCD_Display_One_Hanzi(0, 2, 92, &FONT_Number[Font_Number_Array(d)][16], 16); 
            }
            else if(d >=10 && d < 100)
            {
                LCD_Display_One_Hanzi(0, 2, 92, &FONT_Number[Font_Number_Array(d/10)][16], 16); 
                LCD_Display_One_Hanzi(0, 2, 100, &FONT_Number[Font_Number_Array(d%10)][16], 16); 
            }
            else
            {
                LCD_Display_One_Hanzi(0, 2, 92, &FONT_Capital[5][16], 16);
                LCD_Display_One_Hanzi(0, 2, 100, &FONT_Capital[5][16], 16);
            }
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 4, 20, &FONT_Chinese[20][16], 16);
                LCD_Display_One_Hanzi(1, 4, 44, &FONT_Chinese[24][16], 16);
                LCD_Display_One_Hanzi(1, 4, 68, &FONT_Chinese[60][16], 16);
                LCD_Display_One_Hanzi(1, 4, 92, &FONT_Chinese[62][16], 16);
            
                LCD_Display_One_Hanzi(1, 6, 20, &FONT_Chinese[22][16], 16);
                LCD_Display_One_Hanzi(1, 6, 44, &FONT_Chinese[24][16], 16);
                LCD_Display_One_Hanzi(1, 6, 68, &FONT_Chinese[60][16], 16);
                LCD_Display_One_Hanzi(1, 6, 92, &FONT_Chinese[62][16], 16);
            }
            
        break;
        case 2:
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(0, lcdStatus->capacitySelect*2 +2, 4 , &FONT_Characters[2][16],16);
                LCD_Display_One_Hanzi(0, lcdStatus->capacitySelect*2 +2, 12, &FONT_Characters[3][16],16);
    
                LCD_Display_One_Hanzi(1, 2, 20, &FONT_Chinese[28][16], 16);
                LCD_Display_One_Hanzi(1, 2, 36, &FONT_Chinese[10][16], 16); 
                LCD_Display_One_Hanzi(0, 2, 76, &FONT_Characters[1 ][16], 16);
            }
            if(sw->currentCapa == 0)
            {
                LCD_Display_One_Hanzi(1, 2, 92, &FONT_Chinese[42][16], 16);
            }
            else
            {
                LCD_Display_One_Hanzi(1, 2, 82, &FONT_Chinese[44][16], 16);
            }
            if(lcdStatus->isRefresh == 0)
            {
                LCD_Display_One_Hanzi(1, 4, 20, &FONT_Chinese[26][16], 16);
                LCD_Display_One_Hanzi(1, 4, 44, &FONT_Chinese[28][16], 16);
                LCD_Display_One_Hanzi(1, 4, 68, &FONT_Chinese[60][16], 16);
                LCD_Display_One_Hanzi(1, 4, 92, &FONT_Chinese[62][16], 16);
            }
        break;
        default:;
        break;
    }
}

/*****************************************************************************
 Function    : LCD_Display_Handle_Next_Key
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Handle_Next_Key(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry, SwitchTypeDef* sw)
{
    switch(lcdStatus->menuSelect)
    {
        case 0:
            if(lcdStatus->monitorCurrentPage > 0 && lcdStatus->monitorCurrentPage < 6)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    lcdStatus->monitorCurrentPage++;
                LCD_Display_Monitor(lcdStatus, telemetry);
            }
        break;
        case 1:
            if(lcdStatus->controlCurrentPage > 0 && lcdStatus->controlCurrentPage < 2)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                     lcdStatus->controlCurrentPage++;
                LCD_Display_Control(lcdStatus, sw);
            }
        break;
        default:;
        break;
    }
}
/*****************************************************************************
 Function    : LCD_Display_Handle_Enter_Key
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Handle_Enter_Key(LCDStatusTypeDef *lcdStatus, TelemetryTypeDef *telemetry, SwitchTypeDef* sw)
{
    switch(lcdStatus->menuSelect)
    {
        case 0:
            if(lcdStatus->monitorCurrentPage == 0)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    lcdStatus->monitorCurrentPage++;
                LCD_Display_Monitor(lcdStatus, telemetry);
            }
            else if(lcdStatus->monitorCurrentPage == 1)
            {
                LCD_Display_Monitor(lcdStatus, telemetry);
            }
        break;
        case 1:
            if(lcdStatus->controlCurrentPage == 0)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    lcdStatus->controlCurrentPage++;
                LCD_Display_Control(lcdStatus, sw);
            }
            else if(lcdStatus->controlCurrentPage == 1)
            { 
/**********************************************************/ 
                if(lcdStatus->gearSelect == 1)
                    sw->lcdMotion = 1;
                else if(lcdStatus->gearSelect == 2)
                    sw->lcdMotion = 2;
/**********************************************************/ 
                lcdStatus->gearSelect = 0;     //确认动作后，调压上下选择箭头复位             
                LCD_Display_Control(lcdStatus, sw);
            }
            else if(lcdStatus->controlCurrentPage == 2)
            {
                
/**********************************************************/
#ifdef FUNCTION_TURN_CAPACITY          
                    sw->lcdMotion = 3;
#endif
/**********************************************************/ 
                lcdStatus->capacitySelect = 0;   //确认动作后，调容上下选择箭头复位 
                LCD_Display_Control(lcdStatus, sw);
            }
        break;
        default:;
        break;
    }
}
/*****************************************************************************
 Function    : LCD_Display_Handle_Up_Key
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Handle_Up_Key(LCDStatusTypeDef *lcdStatus, SwitchTypeDef* sw)
{
    switch(lcdStatus->menuSelect)
    {
        case 0:
        break;
        case 1:
            if(lcdStatus->controlCurrentPage == 0)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    lcdStatus->menuSelect--;
                LCD_Display_Menu(lcdStatus->menuSelect, lcdStatus->isRefresh);
            }
            else if(lcdStatus->controlCurrentPage == 1)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    if(lcdStatus->gearSelect > 0)
                        lcdStatus->gearSelect--;
                LCD_Display_Control(lcdStatus, sw);
            }
            else if(lcdStatus->controlCurrentPage == 2)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                    if(lcdStatus->capacitySelect > 0)
                        lcdStatus->capacitySelect--;
                LCD_Display_Control(lcdStatus, sw);
            }
        break;
        default:;
        break;
    }
}
/*****************************************************************************
 Function    : LCD_Display_Handle_Down_Key
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Handle_Down_Key(LCDStatusTypeDef *lcdStatus, SwitchTypeDef *sw)
{
    switch(lcdStatus->menuSelect)
    {
        case 0:
            if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                lcdStatus->menuSelect++;
            LCD_Display_Menu(lcdStatus->menuSelect, lcdStatus->isRefresh);
        break;
        case 1:
            if(lcdStatus->controlCurrentPage == 1)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                {
                    if(lcdStatus->gearSelect < 2)
                        lcdStatus->gearSelect++;
                }
                LCD_Display_Control(lcdStatus, sw);
            }
            else if(lcdStatus->controlCurrentPage == 2)
            {
                if(lcdStatus->currentKeyType != KEY_TYPE_NONE)
                {
                    if(lcdStatus->capacitySelect < 1)
                        lcdStatus->capacitySelect++;
                }
                LCD_Display_Control(lcdStatus, sw);
            }
        break;
        default:;
        break;
    }
}
/*****************************************************************************
 Function    : LCD_Display_Logo
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LCD_Display_Logo(ClockTypeDef *clock)
{
    LCD_Refresh_Screen();
    LCD_Display_One_Hanzi(1, 2, 20, &FONT_Chinese[64][16], 16); 
    LCD_Display_One_Hanzi(1, 2, 44, &FONT_Chinese[66][16], 16);
    LCD_Display_One_Hanzi(1, 2, 68, &FONT_Chinese[68][16], 16);
    LCD_Display_One_Hanzi(1, 2, 92, &FONT_Chinese[70][16], 16);

    
    if((clock->y/10) < 10)
        LCD_Display_One_Hanzi(0,4,36 , &FONT_Number[Font_Number_Array(clock->y/10)][16],16);
    else    
        LCD_Display_One_Hanzi(0,4,36 , &FONT_Number[1][16],16);
    LCD_Display_One_Hanzi(0,4,44 , &FONT_Number[Font_Number_Array(clock->y%10)][16],16);    //年
    LCD_Display_One_Hanzi(0,4,52 , &FONT_Characters[2][16],16);
    
    LCD_Display_One_Hanzi(0,4,60 , &FONT_Number[Font_Number_Array(clock->m/10)][16],16); 
    LCD_Display_One_Hanzi(0,4,68 , &FONT_Number[Font_Number_Array(clock->m%10)][16],16);    //月
    LCD_Display_One_Hanzi(0,4,76 , &FONT_Characters[2][16],16);
    
    LCD_Display_One_Hanzi(0,4,84 , &FONT_Number[Font_Number_Array(clock->d/10)][16],16);   
    LCD_Display_One_Hanzi(0,4,92 , &FONT_Number[Font_Number_Array(clock->d%10)][16],16);    //日

    LCD_Display_One_Hanzi(0,6,36 , &FONT_Number[Font_Number_Array(clock->h/10)][16],16);  
    LCD_Display_One_Hanzi(0,6,44 , &FONT_Number[Font_Number_Array(clock->h%10)][16],16);    //时
    LCD_Display_One_Hanzi(0,6,52 , &FONT_Characters[1][16],16);
    
    LCD_Display_One_Hanzi(0,6,60 , &FONT_Number[Font_Number_Array(clock->min/10)][16],16);    
    LCD_Display_One_Hanzi(0,6,68 , &FONT_Number[Font_Number_Array(clock->min%10)][16],16);  //分
    LCD_Display_One_Hanzi(0,6,76 , &FONT_Characters[1][16],16);
    
    LCD_Display_One_Hanzi(0,6,84 , &FONT_Number[Font_Number_Array(clock->s/10)][16],16);    
    LCD_Display_One_Hanzi(0,6,92 , &FONT_Number[Font_Number_Array(clock->s%10)][16],16);    //秒
}
/************************ZXDQ *****END OF FILE****/

