#ifndef USER_UI_TASK_H
#define USER_UI_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t uiTaskHandle;

void UI_Task_Init(void) ;
void UI_Task(void *argument) ;

#endif /* USER_UI_TASK_H */
