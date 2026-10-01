#ifndef USER_APP_TASK_H
#define USER_APP_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t appTaskHandle;

void App_Task_Init(void);
void App_Task(void *argument);

#endif /* USER_APP_TASK_H */
