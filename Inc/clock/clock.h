/**
  ******************************************************************************
  * File Name          : clock.h
  * Description        : This file contains all functions about clock.
  ******************************************************************************
  * @attention
  
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __clock_H
#define __clock_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct 
{
    uint16_t y;            
    uint16_t m;           
    uint16_t d;         
    uint16_t h;            
    uint16_t min;          
    uint16_t s;          
} ClockTypeDef;   


void Clock_Get_Date_And_Time(ClockTypeDef *clock);
void Clock_Set_Date_And_Time(ClockTypeDef *clock);


#ifdef __cplusplus
}
#endif

#endif

/************************ZXDQ *****END OF FILE****/
  

