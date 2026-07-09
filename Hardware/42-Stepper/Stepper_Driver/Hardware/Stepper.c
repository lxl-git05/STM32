#include "Stepper.h"

__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[CMD_LEN] = {0};
__IO uint8_t rxCount = 0;
extern DMA_HandleTypeDef hdma_usart1_rx ;

// 1. 初始化
void Stepper_Init(void)
{
	
	SCB_DisableDCache();	// 一定要记得这个，否则缓存更新会使得数组数据不更新

//		__HAL_UART_CLEAR_IDLEFLAG(&huart1); 											 	// 清除IDLE标志
//		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 						 		// 使能串UART1 IDLE中断
//		HAL_UART_Receive_DMA(&huart1, (uint8_t *)rxCmd, CMD_LEN); 	// 开启DMA接收模式
	
	// RX_DMA:简化为一行
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)rxCmd, CMD_LEN);
	
	// 等待初始化完成
	HAL_Delay(500) ;
		
}

// 串口RX_DMA处理,放在stm32h7xx_it.c的相应串口位置
void Stepper_Rx_DMA_Cplt(void)
{
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清除IDLE标志

		HAL_UART_DMAStop(&huart1); // 停止DMA，为了重新设置DMA发送多少数据

		rxCount = CMD_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		 
		rxFrameFlag = true; // 置位一帧命令接收完毕标志位
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)rxCmd, CMD_LEN);
	}
}


