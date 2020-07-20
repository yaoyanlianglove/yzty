/**
  ******************************************************************************
  * File Name          : debug.c
  * Description        : This file provides the code to redefine the printf 
                         function.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
#include "debug.h"
#include "usart.h"

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART and Loop until the end of transmission */
#ifdef DEBUG_OUT
    HAL_UART_Transmit(&DEBUG_UART, (uint8_t *)&ch, 1, 0xFFFF);
#endif
    return ch;
}

int dprintf_none(const char *format,...)
{
    return 0;
}

/************************ZXDQ *****END OF FILE****/
