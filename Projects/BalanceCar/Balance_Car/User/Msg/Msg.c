#include "Msg.h"

// 1. 任务句柄
TaskHandle_t appTaskHandle = NULL;

void Msg_Init(void)
{
    /* Create shared queues, semaphores, mutexes, event flags and timers here.
       Check every creation result before Task_Init() can use it.
       Task_Init() assigns thread handles used for thread notifications. */
}
