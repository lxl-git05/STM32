#include "MyISR.h"
#include "AllHeader.h"

void MyISR_TIM6_1ms(void)
{
    Key_Tick();
}

void MyISR_TIM7_20ms(void)
{
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
