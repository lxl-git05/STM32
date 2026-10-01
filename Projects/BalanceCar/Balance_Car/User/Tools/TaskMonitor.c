#include "TaskMonitor.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial_porting.h"
#include <string.h>

typedef struct
{
    TaskHandle_t handle;
    UBaseType_t taskNumber;
    uint32_t previousRun;
} TaskMonitor_Record;

static TaskMonitor_Record previousTasks[TASK_MONITOR_CAPACITY];
static uint32_t previousTotal;
static uint32_t lastCycles, clockUs, remainder, cyclesPerUs;

void TaskMonitor_ClockInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    lastCycles = DWT->CYCCNT;
    cyclesPerUs = SystemCoreClock / 1000000U;
    clockUs = 0U;
    remainder = 0U;
}

uint32_t TaskMonitor_ClockUs(void)
{
    uint32_t mask = __get_PRIMASK();
    uint32_t now, elapsed, result;
    __disable_irq();
    now = DWT->CYCCNT;
    elapsed = now - lastCycles;
    lastCycles = now;
    if (cyclesPerUs != 0U)
    {
        clockUs += elapsed / cyclesPerUs;
        remainder += elapsed % cyclesPerUs;
        clockUs += remainder / cyclesPerUs;
        remainder %= cyclesPerUs;
    }
    result = clockUs;
    __set_PRIMASK(mask);
    return result;
}

static uint32_t TaskMonitor_Collect(const TaskStatus_t *tasks, UBaseType_t count,
                             uint32_t totalTime, uint32_t *reports)
{
    uint32_t window = totalTime - previousTotal;
    for (UBaseType_t i = 0U; i < count; i++)
    {
        uint32_t previousRun = 0U;
        for (UBaseType_t j = 0U; j < TASK_MONITOR_CAPACITY; j++)
        {
            if (previousTasks[j].handle == tasks[i].xHandle &&
                previousTasks[j].taskNumber == tasks[i].xTaskNumber)
            {
                previousRun = previousTasks[j].previousRun;
                break;
            }
        }
        reports[i] = window != 0U ? (uint32_t)(((uint64_t)
            (tasks[i].ulRunTimeCounter - previousRun) * 10000U) / window) : 0U;
    }
    /* Replace the previous snapshot; deleted task handles do not occupy slots. */
    memset(previousTasks, 0, sizeof(previousTasks));
    for (UBaseType_t i = 0U; i < count && i < TASK_MONITOR_CAPACITY; i++)
    {
        previousTasks[i].handle = tasks[i].xHandle;
        previousTasks[i].taskNumber = tasks[i].xTaskNumber;
        previousTasks[i].previousRun = tasks[i].ulRunTimeCounter;
    }
    previousTotal = totalTime;
    return window;
}

void TaskMonitor_Print(void)
{
    static TaskStatus_t snapshot[TASK_MONITOR_CAPACITY];
    static uint32_t windowCpu[TASK_MONITOR_CAPACITY];
    static const char *const states[] = {"Running", "Ready", "Blocked", "Suspended", "Deleted"};
    UBaseType_t count;
    UBaseType_t i;
    uint32_t totalTime, windowUs;

    vTaskSuspendAll();
    count = uxTaskGetSystemState(snapshot,
                                TASK_MONITOR_CAPACITY, &totalTime);
    windowUs = count != 0U ? TaskMonitor_Collect(snapshot, count,
                                               totalTime, windowCpu) : 0U;
    xTaskResumeAll();

    Serial_printf(&Serial1, "\r\n=== FreeRTOS memory / task snapshot ===\r\n");
    Serial_printf(&Serial1, "Tick=%lu  Heap total=%lu B  Free=%lu B  MinFree=%lu B\r\n",
                  (unsigned long)xTaskGetTickCount(), (unsigned long)configTOTAL_HEAP_SIZE,
                  (unsigned long)xPortGetFreeHeapSize(), (unsigned long)xPortGetMinimumEverFreeHeapSize());
    if (count == 0U)
    {
        Serial_printf(&Serial1, "Snapshot failed: task count exceeds %u slots.\r\n",
                      (unsigned int)TASK_MONITOR_CAPACITY);
        return;
    }
    Serial_printf(&Serial1, "Tasks=%lu TotalRunTime=%lu us Window=%lu ms\r\n",
                  (unsigned long)count, (unsigned long)totalTime,
                  (unsigned long)(windowUs / 1000U));
    Serial_printf(&Serial1, "%-16s %-10s %4s %12s %7s %7s %11s\r\n",
                  "Name", "State", "Prio", "RunTime(us)", "CPUwin%", "CPUall%", "StackMin(B)");
    for (i = 0U; i < count; i++)
    {
        const TaskStatus_t *info = &snapshot[i];
        uint32_t cpuWindow = windowCpu[i];
        uint32_t cpuTotal = totalTime != 0U ? (uint32_t)(
            ((uint64_t)info->ulRunTimeCounter * 10000U) / totalTime) : 0U;
        Serial_printf(&Serial1, "%-16s %-10s %4lu %12lu %4lu.%02lu %4lu.%02lu %11lu\r\n",
                      info->pcTaskName, (unsigned int)info->eCurrentState < 5U ? states[info->eCurrentState] : "Invalid",
                      (unsigned long)info->uxCurrentPriority,
                      (unsigned long)info->ulRunTimeCounter,
                      (unsigned long)(cpuWindow / 100U), (unsigned long)(cpuWindow % 100U),
                      (unsigned long)(cpuTotal / 100U), (unsigned long)(cpuTotal % 100U),
                      (unsigned long)(info->usStackHighWaterMark * sizeof(StackType_t)));
        /* Pace the report so asynchronous UART DMA can drain its ring buffer. */
        vTaskDelay(pdMS_TO_TICKS(10U));
    }
    Serial_printf(&Serial1, "=== End ===\r\n");
}
