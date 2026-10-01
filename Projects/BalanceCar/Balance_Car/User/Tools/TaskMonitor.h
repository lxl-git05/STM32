#ifndef USER_TASK_MONITOR_H
#define USER_TASK_MONITOR_H

#include <stdint.h>

#define TASK_MONITOR_CAPACITY 16U

void TaskMonitor_ClockInit(void);
uint32_t TaskMonitor_ClockUs(void);
/* Print native RTOS statistics through Serial1; call from one task only. */
void TaskMonitor_Print(void);

#endif
