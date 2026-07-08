/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*** ����USART����ö�� ***/

// !!!!!!!!!!!�ǵÿ�!!!!!!!!!!!!!!!!
#define USART1_SET
//#define USART2_SET
//#define USART3_SET
// !!!!!!!!!!!�ǵÿ�!!!!!!!!!!!!!!!!

UART_HandleTypeDef* Current_USART_Handle;		  /* ��ǰĳ��USART�ľ�� */
Current_USART_Indx Current_USART_Printf_Indx;	/* ��ǰĳ��USART������ */

/* 
 * ��飺���õ�ǰʹ�õ�USART
 * ������indx - Ҫ���õ�USART����
 * ������������ǣ�USARTx_IDX������x���Դ�1~3
 * ʹ�þ�����������Ҫ�������printf����ǰ�棬ָ������һ�����ڣ�
 * 		Set_Current_USART(USART1_IDX);
 * 		printf("���Ǵ���1\r\n");
 */
void Set_Current_USART(Current_USART_Indx indx)
{
	switch(indx)
  {
		#ifdef USART1_SET
    case USART1_IDX:
    Current_USART_Handle = &huart1;
    Current_USART_Printf_Indx = USART1_IDX;
    break;
		#endif
		#ifdef USART2_SET
    case USART2_IDX:
    Current_USART_Handle = &huart2;
    Current_USART_Printf_Indx = USART2_IDX;
    break;
		#endif
		#ifdef USART3_SET
    case USART3_IDX:
    Current_USART_Handle = &huart3;
    Current_USART_Printf_Indx = USART3_IDX;
    break;
		#endif
    default:
    Current_USART_Handle = NULL;
    Current_USART_Printf_Indx = USART_NONE;
    break;
  }
}

/* 
 * ��飺�ض���fputc���������ڽ��ַ��������ǰ���õ�USART
 * ������
 * ch - Ҫ���͵��ַ�
 * f  - �ļ�ָ�루�ڴ�ʵ����δʹ�ã�
 * ����ֵ�����͵��ַ�����EOF�������
 */

int fputc(int ch, FILE *f)
{
  if(Current_USART_Handle == NULL)
	{			/* �����ǰû������USART������򷵻�EOF��ʾ���� */
    return EOF;
  }
  /* ���ݵ�ǰ���õ�USART�����ѡ���Ӧ��USART���跢���ַ� */
	#ifdef USART1_SET
  if(Current_USART_Handle == &huart1)
	{		
		while ((USART1->SR & 0X40) == 0); 		/* �ȴ�USART1������ɣ�Ȼ�����ַ� */
		USART1->DR = (uint8_t)ch; 				/* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
  }
	#endif
	#ifdef USART2_SET
  else if(Current_USART_Handle == &huart2)
	{
		while ((USART2->SR & 0X40) == 0); 		/* �ȴ�USART2������ɣ�Ȼ�����ַ� */
		USART2->DR = (uint8_t)ch; 				/* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
  }
	#endif
	#ifdef USART3_SET
  else if(Current_USART_Handle == &huart3)
	{
		while ((USART3->ISR & 0X40) == 0); 		/* �ȴ�USART3������ɣ�Ȼ�����ַ� */
		USART3->TDR = (uint8_t)ch; 				/* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
  }
	#endif
  return ch;								/* ���ط��͵��ַ� */
}


/* USER CODE END 1 */
