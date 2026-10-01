#include "App_Task.h"
#include "Msg.h"
#include "main.h"

void App_Task_Init(void)
{
    if (appTaskHandle == NULL)
    {
        if (xTaskCreate(App_Task, "App_Task", 128U, NULL,24U, &appTaskHandle) != pdPASS)
        {
            Error_Handler();
        }
    }
}

void App_Task(void *argument)
{
    (void)argument;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}
