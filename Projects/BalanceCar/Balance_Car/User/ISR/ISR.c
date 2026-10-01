#include "ISR.h"

/* HAL_TIM_PeriodElapsedCallback remains in main.c because CubeMX generates
   the TIM4 HAL timebase there. Its USER CODE section forwards here. */
void ISR_TIM_Handler(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        /* HAL_IncTick() is handled by main.c. No user processing here. */
        return;
    }

    /* Add short user timer dispatch when timer interrupts are enabled. */
}

/* UART IT and UART DMA completion share these HAL callbacks.
   Do not replace the HAL-owned DMA callback pointers. */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* PC communication: dispatch to a short Hardware handler. */
    }
    else if (huart->Instance == USART2)
    {
        /* ESP32 communication: dispatch to a short Hardware handler. */
    }
    else if (huart->Instance == USART3)
    {
        /* Motor communication: dispatch to a short Hardware handler. */
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Add port-specific completion dispatch when a driver needs it. */
    (void)huart;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Save error state or notify a task; perform recovery in task context. */
    (void)huart;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* No CAN receive consumer exists yet. Before enabling FIFO0 notification,
       implement bounded FIFO servicing or mask notification until a task
       drains the FIFO. A pending FIFO must not be left retriggering the IRQ. */
    (void)hcan;
}
