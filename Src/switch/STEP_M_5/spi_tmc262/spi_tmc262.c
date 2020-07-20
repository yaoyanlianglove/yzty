/**
  ******************************************************************************
  * File Name          : spi_tmc262.c
  * Description        : This file contains all functions about tmc262.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "spi_tmc262.h"
#include "gpio.h"
/*****************************************************************************
 Function    : SPI_TMC262_GPIO_Init
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void SPI_TMC262_GPIO_Init(void)
{
    TMC262_CS_HIGH;
    TMC262_SCK_HIGH;
}
/*****************************************************************************
 Function    : Delay_TMC262
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void Delay_TMC262(uint16_t nDelay)
{
	uint16_t i;
	for(i = 0; i < nDelay; i++);    
}
/*****************************************************************************
 Function    : TMC262_Read_Write
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
uint32_t TMC262_Read_Write(uint32_t data)
{
    int i;
	uint32_t temp=0;
    uint32_t read_data = 0;
    TMC262_CS_LOW;
    Delay_TMC262(5);
    for(i = 0; i < 20; i++)
    {   
        temp = data;
		temp >>= (19 - i);	  
        if(temp & 0x01) // 获取位                       
            TMC262_MOSI_HIGH;             
        else
            TMC262_MOSI_LOW;
        TMC262_SCK_LOW;
        Delay_TMC262(10);
        if(TMC262_MISO_READ_DATA == 0x01)  
            read_data |= 0x01;
        if(i < 19)
		    read_data <<= 1;
        Delay_TMC262(10);
        TMC262_SCK_HIGH;
        Delay_TMC262(20);
    }
	TMC262_CS_HIGH;
    return(read_data);
}
/************************ZXDQ *****END OF FILE****/

