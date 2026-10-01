#include "Allheader.h"
#include "main.h"

TaskHandle_t keyTaskHandle = NULL;

void Key_Task_Init(void)
{
    if (keyTaskHandle == NULL)
    {
        if (xTaskCreate(Key_Task, "Key_Task", 48U, NULL,Key_Task_Priority, &keyTaskHandle) != pdPASS)
        {
            Error_Handler();
        }
    }
}

void Key_Task(void *argument)
{
    (void)argument;

	TickType_t lastWake = xTaskGetTickCount();
	
    while(1)
    {
        Key_Tick();
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(20U));
    }
}
