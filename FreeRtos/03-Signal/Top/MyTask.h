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
#include "tim.h"
// 队列相关头文件
#include "cmsis_os2.h"
#include "queue.h"
// 信号量相关头文件
#include "semphr.h"
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

// TIM2定时器
void tim2_cb(TIM_HandleTypeDef *htim) ;

#endif
