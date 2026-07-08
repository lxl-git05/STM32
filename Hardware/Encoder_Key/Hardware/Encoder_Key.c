#include "Encoder_Key.h"

int16_t Encoder_Count = 0;   // 编码器计数值

void Encoder_Init(void)
{
    // PB0/PB1 的 GPIO 和 NVIC 配置已在 gpio.c 的 MX_GPIO_Init() 中完成
}

int16_t Encoder_Get(void)
{
    int16_t temp = Encoder_Count;
    Encoder_Count = 0;
    return temp;
}

// HAL_GPIO_EXTI_Callback 由 stm32f1xx_it.c 中的 EXTI0/EXTI1_IRQHandler
// 调用 HAL_GPIO_EXTI_IRQHandler，再由后者调用此回调
// 此函数是弱定义的，会覆盖 HAL 库中的空实现
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)
    {
        // 去抖动：再次确认 PB0 仍为低电平
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            // 读取 PB1 电平判断方向
            // 上升沿触发时：PB1高→正转++，PB1低→反转--
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
            {
                Encoder_Count++;
            }
        }
    }
    else if (GPIO_Pin == GPIO_PIN_1)
    {
        // 去抖动：再次确认 PB1 仍为低电平
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
        {
            // 读取 PB0 电平判断方向
            // 上升沿触发时：PB0低→正转++，PB0高→反转--
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
            {
                Encoder_Count--;
            }
        }
    }
}
