#include "Allheader.h"
#include "main.h"

TaskHandle_t debugTaskHandle = NULL ;

void Debug_Task(void *argument)
{
    (void)argument;
	
	TickType_t lastWake = xTaskGetTickCount();
	
    while(1)
    {	
		if (Key_Check(KEY_0, KEY_LONG))
		{
			TaskMonitor_Print();
		}
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(20U));
    }
}

void Debug_Task_Init(void)
{
    if (debugTaskHandle == NULL)
    {
        if (xTaskCreate(Debug_Task, "Debug_Task", 512U, NULL,Debug_Task_Priority, &debugTaskHandle) != pdPASS)
        {
            Error_Handler();
        }
    }
}
