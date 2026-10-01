#ifndef __MPU6050_TASK_H
#define __MPU6050_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* For STM32F103: 128U words = 512 bytes. Tune using stack watermark. */
#define MPU6050_TASK_STACK_WORDS    128U
#define MPU6050_TASK_PERIOD_MS      20U

/* 1: auto-calibrate at boot (stationary, level). 0: use stored bias. */
#define MPU6050_TASK_AUTO_CALIB     1U

extern TaskHandle_t mpu6050TaskHandle;

BaseType_t MPU6050_Task_Create(void);
uint8_t MPU6050_Task_IsReady(void);
uint32_t MPU6050_Task_GetFailCount(void);
/* portMAX_DELAY if not initialized; use to reject stale attitude for motors. */
TickType_t MPU6050_Task_GetDataAgeTicks(void);

#ifdef __cplusplus
}
#endif
#endif
