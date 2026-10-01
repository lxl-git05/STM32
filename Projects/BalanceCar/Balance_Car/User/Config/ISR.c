#include "ISR.h"
#include "Serial_porting.h"

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

/* ReceiveToIdle DMA dispatch is separate from ordinary RxCplt. */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    if (huart->Instance == USART1)
    {
        Serial_RxEventCallback(huart, size);
    }
    else if (huart->Instance == USART2)
    {
        /* ESP32 raw receive entry reserved; not initialized. */
    }
    else if (huart->Instance == USART3)
    {
        /* Motor raw receive entry reserved; not initialized. */
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* USART1 DMA TX: advance the ring buffer and start pending data. */
    if (huart->Instance == USART1)
    {
        Serial_TxCpltCallback(huart);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Serial_ErrorCallback(huart);
    }
    /* USART2/3 have no user receive initialization or protocol yet. */
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* No CAN receive consumer exists yet. Before enabling FIFO0 notification,
       implement bounded FIFO servicing or mask notification until a task
       drains the FIFO. A pending FIFO must not be left retriggering the IRQ. */
    (void)hcan;
}
