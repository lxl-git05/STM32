#include "AllHeader.h"
#include "Initial.h"
/*
	这里不仅是硬件、任务、通信的初始化地点,更是通信初始化的库,所以通信结构都在这里进行初始化
*/

void Hardware_Init(void)
{
    /* Initialize user device drivers here, after CubeMX peripheral init.
       Do not call MX_xxx_Init() or create RTOS objects here. */
    Timer_Counter_Init();
    OLED_Init();
    Battery_Init();
    if (Serial_Init() != HAL_OK)
    {
        Error_Handler();
    }
}

void Task_Init(void)
{
    if (MPU6050_Task_Create() != pdPASS)
    {
        Error_Handler();
    }
    App_Task_Init();    // 主控任务初始化
	UI_Task_Init() ;	// OLED任务
	Key_Task_Init();	// 按键任务
	Debug_Task_Init();	// Debug任务,其实主要是打印
}

void Msg_Init(void)
{
    /* Create shared queues, semaphores, mutexes, event flags and timers here.
       Check every creation result before Task_Init() can use it. */
	
}
