#ifndef __KEY_TASK_H
#define __KEY_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t keyTaskHandle;

void Key_Task_Init(void) ;
void Key_Task(void *argument) ;


#endif
