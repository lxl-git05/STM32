#include "Initial.h"
#include "App_Task.h"

void Hardware_Init(void)
{
    /* Initialize user device drivers here, after CubeMX peripheral init.
       Do not call MX_xxx_Init() or create RTOS objects here. */
}

void Task_Init(void)
{
    App_Task_Init();    // 主控任务初始化
}
