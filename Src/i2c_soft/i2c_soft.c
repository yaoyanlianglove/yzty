/**
  ******************************************************************************
  * File Name          : i2c_soft.c
  * Description        : This file provides functions to implement I2C using 
                         GPIO.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "i2c_soft.h"

/*****************************************************************************
 Function    : Delay_I2C
 Description : The cycle of I2C.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Delay_I2C(uint16_t nDelay)
{
    uint16_t i;
    for(i = 0; i < nDelay; i++);
}
/*****************************************************************************
 Function    : I2C_SDA_Out
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_SDA_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin   = I2C_SDA_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*****************************************************************************
 Function    : I2C_SDA_In
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_SDA_In(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin   = I2C_SDA_Pin;
    GPIO_InitStruct.Mode  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*****************************************************************************
 Function    : I2C_Send_Start
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_Send_Start(void)
{   
    I2C_SDA_Out();
    SDA_HIGH;
    Delay_I2C(I2C_Delay_Time);   
    SCL_HIGH;
    Delay_I2C(I2C_Delay_Time);    
    SDA_LOW;
    Delay_I2C(I2C_Delay_Time);
    SCL_LOW;
    Delay_I2C(I2C_Delay_Time);  
}
/*****************************************************************************
 Function    : I2C_Send_Stop
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_Send_Stop(void)
{ 
    I2C_SDA_Out();
    SDA_LOW;
    Delay_I2C(I2C_Delay_Time);
    SCL_HIGH;
    Delay_I2C(I2C_Delay_Time);
    SDA_HIGH;
    Delay_I2C(I2C_Delay_Time);
}
/*****************************************************************************
 Function    : I2C_Wait_Ack
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t I2C_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    I2C_SDA_In(); 
    SDA_HIGH;
    Delay_I2C(I2C_Delay_Time);
    SCL_HIGH;
    Delay_I2C(I2C_Delay_Time);
    while(SDA_STATUS)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            I2C_Send_Stop();
            return 1;
        }
    }
    SCL_LOW;
    return 0;
}
/*****************************************************************************
 Function    : I2C_NAck
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_NAck(void)
{
    SCL_LOW;
    I2C_SDA_Out();
    SDA_HIGH;
    Delay_I2C(I2C_Delay_Time);
    SCL_HIGH;
    Delay_I2C(I2C_Delay_Time);
    SCL_LOW;
}
/*****************************************************************************
 Function    : I2C_Ack
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void I2C_Ack(void)
{
    SCL_LOW;
    I2C_SDA_Out();
    SDA_LOW;
    Delay_I2C(I2C_Delay_Time);    
    SCL_HIGH;
    Delay_I2C(I2C_Delay_Time);
    SCL_LOW;
}
/*****************************************************************************
 Function    : I2C_Wirte_Byte
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint8_t I2C_Wirte_Byte(uint8_t ndata)
{
    uint8_t i;
    I2C_SDA_Out();
    SCL_LOW;
    for(i=0; i<8; i++)
    {
        if((ndata<<i)&0x80) 
        {
            SDA_HIGH;
        }
        else
        {                 
            SDA_LOW;
        }
        Delay_I2C(I2C_Delay_Time);
        SCL_HIGH;
        Delay_I2C(I2C_Delay_Time);
        SCL_LOW;
        Delay_I2C(I2C_Delay_Time);
    }   
    i = I2C_Wait_Ack();
    return i;
}
/*****************************************************************************
 Function    : I2C_Read_Byte
 Description : 
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
uint8_t I2C_Read_Byte(uint8_t ack)
{
    uint8_t result=0;
    uint8_t i;
    I2C_SDA_In();
    for(i=0; i<8; i++)
    {
        SCL_LOW;
        Delay_I2C(I2C_Delay_Time);
        SCL_HIGH;
        Delay_I2C(I2C_Delay_Time);
        result = result<<1;
        if(SDA_STATUS == 1) 
            result = result + 1;
        Delay_I2C(I2C_Delay_Time);
    }
    if (!ack)
        I2C_NAck();
    else
        I2C_Ack(); 
    return(result);
}

/************************ZXDQ *****END OF FILE****/

