/**
  ******************************************************************************
  * File Name          : debug.h
  * Description        : This file provides the code to redefine the printf 
                         function.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __debug_H
#define __debug_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "target_config.h"
#include "los_typedef.h"
#include <string.h>

#define DEBUG_OUT  1
#define DEBUG_UART  huart4
#ifdef DEBUG_OUT
    #define dprintf (VOID)printf
#else
    extern INT32 dprintf_none(const CHAR *format,...);
    #define dprintf (VOID)dprintf_none
#endif

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

