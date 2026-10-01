#ifndef USER_DEBUG_TASK_H
#define USER_DEBUG_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t debugTaskHandle;

void Debug_Task_Init(void) ;
void Debug_Task(void *argument) ;

#endif /* USER_DEBUG_TASK_H */
