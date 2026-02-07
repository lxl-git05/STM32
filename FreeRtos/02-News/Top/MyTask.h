#ifndef __MYTASK_H
#define __MYTASK_H

// ========================= 库声明 =========================

// 系统库
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Stdbool.h"
#include "Stdio.h"
#include "usart.h"
// 外设库
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
// 工具库
#include "Timer_Counter.h"

// ========================= 函数 =========================
void Initial_ALL(void) ;

void MyTask_default(void);

void MyTask_01(void);

void MyTask_02(void);

void MyTask_03(void);

#endif
