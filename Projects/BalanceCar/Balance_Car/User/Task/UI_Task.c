#include "Allheader.h"
#include "main.h"

TaskHandle_t uiTaskHandle = NULL ;

static int OLED_Num = 0 ;

void UI_Task(void *argument)
{
    (void)argument;
	
    while(1)
    {	
		// OLED展示
		OLED_Clear() ;
		OLED_Printf(0,0,OLED_6X8 , "Num = %d",OLED_Num ++) ;
		
		// 电量展示
		int percent = Battery_GetValue();
        if (percent >= 0)
            OLED_Printf(0,16,OLED_6X8 , "Battery: %d%%", percent) ;
		
		// OLED刷新
		OLED_Update() ;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void UI_Task_Init(void)
{
    if (uiTaskHandle == NULL)
    {
        if (xTaskCreate(UI_Task, "UI_Task", 128U, NULL,UI_Task_Priority, &uiTaskHandle) != pdPASS)
        {
            Error_Handler();
        }
    }
}
