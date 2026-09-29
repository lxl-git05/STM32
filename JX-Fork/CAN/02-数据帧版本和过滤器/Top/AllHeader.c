#include "AllHeader.h"
#include "tim.h"
void Initial_ALL(void)
{
    OLED_Init();
    Serial_Init();
}
void Initial_Timer(void)
{
    if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK) Error_Handler();
    if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK) Error_Handler();
}
