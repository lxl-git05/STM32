#include "MyISR.h"
#include "AllHeader.h"

static volatile uint8_t isr_started;

void MyISR_Start(void)
{
    isr_started = 1;
}

void MyISR_SysTick(void)
{
    static uint8_t divide_20ms;
    if (!isr_started) return;
		// 1ms中断
    Key_Tick();
		// 20ms中断
    if (++divide_20ms < 20) return;
    divide_20ms = 0;
    switch (curr_mode)
    {
        case Mode_1: Mode_1_Tick(); break;
        case Mode_2: Mode_2_Tick(); break;
        case Mode_3: Mode_3_Tick(); break;
        case Mode_4: Mode_4_Tick(); break;
        case Mode_5: Mode_5_Tick(); break;
        case Mode_6: Mode_6_Tick(); break;
        default: break;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    Serial_RxEventCallback(huart, size);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    Serial_ErrorCallback(huart);
}
