#ifndef USER_MSG_H
#define USER_MSG_H

#include "FreeRTOS.h"
#include "task.h"

// 1. 任务句柄
extern TaskHandle_t appTaskHandle;

void Msg_Init(void);

#endif /* USER_MSG_H */
