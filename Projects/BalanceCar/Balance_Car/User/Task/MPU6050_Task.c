#include "MPU6050_Task.h"
#include "MPU6050.h"
#include <stddef.h>
#include "Allheader.h"

TaskHandle_t mpu6050TaskHandle = NULL;
static volatile uint8_t s_ready = 0U;
static volatile uint32_t s_failCount = 0U;
static volatile TickType_t s_lastGoodTick = 0U;

static void MPU6050_Task(void *argument)
{
    (void)argument;
    /* vTaskDelay(10 ms) must advance at least one RTOS tick. */
    if (pdMS_TO_TICKS(10U) == 0U ||
        pdMS_TO_TICKS(MPU6050_TASK_PERIOD_MS) == 0U)
    {
        ++s_failCount;
        vTaskDelete(NULL);
        return;
    }
    /* Must run after MX_I2C1_Init() and after FreeRTOS starts. */
    for (;;)
    {
        if (MPU6050_Mahony_Init(MPU6050_TASK_AUTO_CALIB) == HAL_OK)
            break;
        ++s_failCount;
        vTaskDelay(pdMS_TO_TICKS(1000U));  /* Retry absent/disconnected sensor. */
    }

    const TickType_t period = pdMS_TO_TICKS(MPU6050_TASK_PERIOD_MS);
    TickType_t lastWake = xTaskGetTickCount();
    TickType_t lastGood = lastWake;
    uint8_t consecutiveFailures = 0U;
    s_lastGoodTick = lastWake;

    for (;;)
    {
        vTaskDelayUntil(&lastWake, period);
        TickType_t now = xTaskGetTickCount();
        float dt = (float)(now - lastGood) / (float)configTICK_RATE_HZ;

        /* After a long outage, do not integrate a large stale interval. */
        if (dt > 0.100f) dt = 0.020f;

        if (MPU6050_Mahony_Update_Tick(dt) == HAL_OK)
        {
            consecutiveFailures = 0U;
            lastGood = now;
            s_lastGoodTick = xTaskGetTickCount();
            s_ready = 1U;
        }
        else
        {
            ++s_failCount;
            if (++consecutiveFailures >= 3U)
            {
                (void)MPU6050_I2C_Recover();
                consecutiveFailures = 0U;
            }
            /* Keep the previous good attitude; freshness exposes the fault. */
        }
		
		// MPU6050打印数据
//        if (MPU6050_GetSnapshot(&imu) == HAL_OK)
//        {
//            Serial_printf(&Serial1, "%.2f\n", imu.yaw_abs);
//        }
//        else if (xTaskGetTickCount() - lastMpuDiagnostic >= pdMS_TO_TICKS(1000U))
//        {
//            lastMpuDiagnostic = xTaskGetTickCount();
//            Serial_printf(&Serial1,
//                          "MPU invalid: ready=%u fail=%lu ageTicks=%lu I2Cerr=0x%08lX\r\n",
//                          (unsigned int)MPU6050_Task_IsReady(),
//                          (unsigned long)MPU6050_Task_GetFailCount(),
//                          (unsigned long)MPU6050_Task_GetDataAgeTicks(),
//                          (unsigned long)HAL_I2C_GetError(&hi2c1));
//        }
    }
}

BaseType_t MPU6050_Task_Create(void)
{
    if (mpu6050TaskHandle != NULL) return pdPASS;
    return xTaskCreate(MPU6050_Task,
                       "MPU6050",
                       MPU6050_TASK_STACK_WORDS,
                       NULL,
                       MPU6050_TASK_PRIORITY,
                       &mpu6050TaskHandle);
}

uint8_t MPU6050_Task_IsReady(void)
{
    return s_ready;
}

uint32_t MPU6050_Task_GetFailCount(void)
{
    return s_failCount;
}

TickType_t MPU6050_Task_GetDataAgeTicks(void)
{
    if (s_ready == 0U) return portMAX_DELAY;
    return xTaskGetTickCount() - s_lastGoodTick;
}
