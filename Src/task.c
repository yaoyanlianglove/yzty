/**
  ******************************************************************************
  * File Name          : task.c
  * Description        : This file contains all functions about task.
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
#include "task.h"
#include "los_swtmr.h"
#include "debug.h"
#include "yzty.h"
#include "delay.h"

static UINT32 TASK_YZTY_INIT_ID;  
static UINT32 TASK_LCD_ID;
static UINT32 TASK_MAIN_ID;   

static UINT16 TIMER_1MS_ID; 


#define TASK_YZTY_INIT_PRIOR     2
#define TASK_LCD_PRIOR           20  
#define TASK_MAIN_PRIOR          3  

#define TASK_YZTY_INIT_SIZE      0x150
#define TASK_LCD_SIZE            0x200
#define TASK_MAIN_SIZE           0x150

/*****************************************************************************
 Function    : Task_Lcd
 Description : Lcd task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static UINT32 Task_Lcd(VOID)
{
    UINT32 res = LOS_OK;    
    dprintf("Task_Lcd Start.\r\n"); 
    while(1)
    { 
        YZTY_Lcd();
        res = LOS_TaskDelay(1000);
        if (res != LOS_OK)
        {
            dprintf("Delay Task Failed.\r\n");
            return LOS_NOK;
        }
    }   
}
/*****************************************************************************
 Function    : Task_Main
 Description : Main task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static UINT32 Task_Main(VOID)
{
    UINT32 res = LOS_OK; 
    dprintf("Task_Main Start.\r\n");    
    while(1)
    {
        YZTY_Refresh_Iwdg();                         //喂狗
        YZTY_Read_Telemetry();                       //读遥测
        YZTY_Read_Oil_Temp();                        //读温度
        YZTY_Read_Gear_Signal();                     //读档位
        YZTY_Protect_Judge();                        //保护判断
        YZTY_Lock_Judge();                           //闭锁判断
        YZTY_Device_Status_Judge();                  //设备状态
        YZTY_Read_Clock();                           //读系统时间
        YZTY_Storage();                              //存储
        YZTY_Hand_Judge();                           //手动按钮控制判断
        YZTY_Control_Judge();                        //控制判断
        YZTY_Reset_Judge();                          //重启判断
        YZTY_Heart_Judge();                          //心跳超时，远程模式切自动模式
        YZTY_Communication();                        //通信程序
        res = LOS_TaskDelay(1000); 
        if (res != LOS_OK)
        {
            dprintf("Delay Task Failed.\r\n");
            return LOS_NOK;
        }
    }   
}
/*****************************************************************************
 Function    : Timer_1ms_Callback
 Description : None
 Input       : None
 Output      : None
 Return      : None
*****************************************************************************/ 
static VOID Timer_1ms_Callback(UINT32 arg)//回调函数1ms
{
    YZTY_Time_Counter();
}
/*****************************************************************************
 Function    : Task_APP_Create
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static UINT32 Task_APP_Create(VOID)
{
    UINT32 res;
    TSK_INIT_PARAM_S stInitParam;

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_Main;
    stInitParam.usTaskPrio   = TASK_MAIN_PRIOR;
    stInitParam.pcName       = "Task_Main";
    stInitParam.uwStackSize  = TASK_MAIN_SIZE;

    res = LOS_TaskCreate(&TASK_MAIN_ID, &stInitParam);
    if (res != LOS_OK)
    {
        dprintf("TASK_MAIN create Failed!\r\n");
        return LOS_NOK;
    }
    return LOS_OK;
} 
/*****************************************************************************
 Function    : Task_YZTY_Init
 Description : YZTY init task.
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
static UINT32 Task_YZTY_Init(VOID)
{
    UINT32 res = LOS_OK;
    UINT32 initRes = 0;
    dprintf("Task YZTY Init Start.\r\n"); 
    initRes = YZTY_Init();   
    while(1)
    {
        if(initRes != 0)
        {
            YZTY_Refresh_Iwdg();                         //喂狗
            YZTY_Read_Telemetry();                       //读遥测
            YZTY_Read_Oil_Temp();                        //读温度
            YZTY_Read_Gear_Signal();                     //读档位
            YZTY_Protect_Judge();                        //保护判断
            YZTY_Lock_Judge();                           //闭锁判断
            YZTY_Read_Clock();                           //读系统时间
            YZTY_Device_Status_Judge();
            YZTY_Storage();
            YZTY_Communication();                        //通信程序
            YZTY_Reset_Judge();                          //重启判断
            res = LOS_TaskDelay(1000);
            if (res != LOS_OK)
            {
                dprintf("Delay Task Failed.\r\n");
                return LOS_NOK;
            }
        }
        else
        {
            res = Task_APP_Create();
            if (res != LOS_OK)
            {
                dprintf("Task_APP_Create Failed.\r\n");
            }
            else
            {
                LOS_TaskDelete(TASK_YZTY_INIT_ID);
            }
        }
    }   
}
/*****************************************************************************
 Function    : Task_Create
 Description : None
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/  
UINT32 Task_Create(VOID)
{
    UINT32 res;
    TSK_INIT_PARAM_S stInitParam;
    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_YZTY_Init;
    stInitParam.usTaskPrio   = TASK_YZTY_INIT_PRIOR;
    stInitParam.pcName       = "TASK_YZTY_INIT";
    stInitParam.uwStackSize  = TASK_YZTY_INIT_SIZE;


    res = LOS_SwtmrCreate(1, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)Timer_1ms_Callback, &TIMER_1MS_ID 
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
                , OS_SWTMR_ROUSES_ALLOW,
                OS_SWTMR_ALIGN_SENSITIVE 
#endif
                ,1);
    if(LOS_OK != res)
    {
        dprintf("Create Timer 1ms failed\r\n");
        return LOS_NOK;
    }

    res = LOS_SwtmrStart(TIMER_1MS_ID);//启动周期性软件定时器
    if(LOS_OK != res)
    {
        dprintf("Start Timer 1ms failed\r\n");
        return LOS_NOK;
    }
    dprintf("Timer 1ms Start\r\n");
    
    delay_ms(1000);

    res = LOS_TaskCreate(&TASK_YZTY_INIT_ID, &stInitParam);
    if (res != LOS_OK)
    {
        dprintf("TASK_YZTY_INIT_ create Failed!\r\n");
        return LOS_NOK;
    }

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_Lcd;
    stInitParam.usTaskPrio   = TASK_LCD_PRIOR;
    stInitParam.pcName       = "TASK_LCD";
    stInitParam.uwStackSize  = TASK_LCD_SIZE;


    res = LOS_TaskCreate(&TASK_LCD_ID, &stInitParam);
    if (res != LOS_OK)
    {
        dprintf("Task_Lcd create Failed!\r\n");
        return LOS_NOK;
    }
    return LOS_OK;
} 

/************************ZXDQ *****END OF FILE****/


