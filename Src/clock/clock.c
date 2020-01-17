/**
  ******************************************************************************
  * File Name          : clock.c
  * Description        : This file contains all functions about clock.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "clock.h"
#include "i2c_soft.h"


/*****************************************************************************
 Function    : RX8025_Read
 Description : Read data from RX8025.
 Input       : addr, the address of data
               length, the length of data
               buff, the pointer of data
 Output      : None
 Return      : None
 *****************************************************************************/   
void RX8025_Read(uint8_t addr, uint8_t length, uint8_t *buff)
{  
    uint8_t i;  
    I2C_Send_Start();
    I2C_Wirte_Byte(0x64);  
    I2C_Wirte_Byte(addr);  
    I2C_Send_Start();  
    I2C_Wirte_Byte(0x65);  
    for(i=0;i<length-1;i++)   
        buff[i] = I2C_Read_Byte(1);      
    buff[i] = I2C_Read_Byte(0); 
    I2C_Send_Stop();  
} 
/*****************************************************************************
 Function    : RX8025_Wirte
 Description : Wirte data to RX8025.
 Input       : addr, the address of data
               buff, the pointer of data
               len, the length of data
 Output      : None
 Return      : None
 *****************************************************************************/  
void RX8025_Wirte(uint8_t addr, uint8_t *buff, uint8_t len)
{  
    uint8_t i;    
    I2C_Send_Start();   
    for(i=0; i<len; i++)  
    {  
        I2C_Wirte_Byte(0X64);  
        I2C_Wirte_Byte(addr);   
        I2C_Wirte_Byte(buff[i]);
        addr++;
    }  
    I2C_Send_Stop();  
}  
/*****************************************************************************
 Function    : Clock_Get_Date_And_Time
 Description : Read date and time from RX8025.
 Input       : The pointer of clock structure.
 Output      : None
 Return      : None
 *****************************************************************************/  
void Clock_Get_Date_And_Time(ClockTypeDef *clock)
{
    uint8_t temp;  
    RX8025_Read(6, 1, &temp);                            
    clock->y   = ((temp>>4)*10) + (temp & 0x0f);     //年

    RX8025_Read(5, 1, &temp);                        
    clock->m   = ((temp>>4)*10) + (temp & 0x0f);     //月
           
    RX8025_Read(4, 1, &temp);
    clock->d   = ((temp>>4)*10) + (temp & 0x0f);     //日

    RX8025_Read(2, 1, &temp);                        
    clock->h   = ((temp>>4)*10) + (temp & 0x0f);     //时

    RX8025_Read(1, 1, &temp);                         
    clock->min = ((temp>>4)*10) + (temp & 0x0f);     //分
         
    RX8025_Read(0, 1, &temp);                         
    clock->s   = ((temp>>4)*10) + (temp & 0x0f);     //秒
}
/*****************************************************************************
 Function    : Clock_Set_Date_And_Time
 Description : Write date and time to RX8025.
 Input       : The pointer of clock structure.
 Output      : None
 Return      : None
 *****************************************************************************/ 
void Clock_Set_Date_And_Time(ClockTypeDef *clock)
{    
    uint8_t temp;
    temp = (( clock->y/10)<<4) | ( clock->y%10);
    RX8025_Wirte(6, &temp, 1);

    temp = (( clock->m/10)<<4) | ( clock->m%10);
    RX8025_Wirte(5, &temp, 1);

    temp = (( clock->d/10)<<4) | ( clock->d%10);
    RX8025_Wirte(4, &temp, 1);

    temp = (( clock->h/10)<<4) | ( clock->h%10);
    RX8025_Wirte(2, &temp, 1);

    temp = (( clock->min/10)<<4) | ( clock->min%10);
    RX8025_Wirte(1, &temp, 1);

    temp = (( clock->s/10)<<4) | ( clock->s%10);
    RX8025_Wirte(0, &temp, 1);
    
    temp = 0x20;
    RX8025_Wirte(0x0E, &temp, 1);//24小时制
}

/************************ZXDQ *****END OF FILE****/

