#ifndef USER_ALLHEADER_H
#define USER_ALLHEADER_H

/* CubeMX peripheral headers. */
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Use native FreeRTOS APIs for application RTOS operations. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

/* Add actual Hardware, Software and Function module headers here. */
// 1. 配置初始化
#include "Initial.h"
#include "ISR.h"

// 2. 硬件外设
#include "MPU6050.h"
#include "MPU6050_Task.h"
#include "OLED.h"
#include "Battery.h"
#include "Serial_base.h"
#include "Serial_porting.h"
#include "Timer_Counter.h"
#include "TaskMonitor.h"
#include "Key.h"

// 3. 任务初始化
#include "App_Task.h"
#include "UI_Task.h"
#include "Key_Task.h"
#include "Debug_Task.h"

#endif /* USER_ALLHEADER_H */
