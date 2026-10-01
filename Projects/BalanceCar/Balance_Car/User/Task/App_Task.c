#include "Allheader.h"
#include "main.h"

TaskHandle_t appTaskHandle = NULL;

void App_Task(void *argument)
{
    (void)argument;
	
	TickType_t lastWake = xTaskGetTickCount();
	
    while(1)
    {
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(20U));
    }
}

void App_Task_Init(void)
{
    if (appTaskHandle == NULL)
    {
        if (xTaskCreate(App_Task, "App_Task", 128U, NULL,App_Task_Priority, &appTaskHandle) != pdPASS)
        {
            Error_Handler();
        }
    }
}
