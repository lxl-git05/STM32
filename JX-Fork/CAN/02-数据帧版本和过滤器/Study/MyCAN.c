#include "MyCAN.h"

// 过滤器配置: 全部过滤,只使用FIFO0接收
// 全阻塞式发送和轮训式接收
void MyCAN_Init(void)
{
	/* MX_CAN1_Init() has already configured the clock, GPIO and bit timing. */
	CAN_FilterTypeDef Filter = {0};	// 0号过滤器
	Filter.FilterBank = 0;
	Filter.FilterIdHigh = 0x0000;
	Filter.FilterIdLow = 0x0000;
	Filter.FilterMaskIdHigh = 0x0000;
	Filter.FilterMaskIdLow = 0x0000;
	Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	Filter.FilterActivation = ENABLE;
	Filter.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&hcan1, &Filter) != HAL_OK) Error_Handler();
	if (HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_LISTENING)
	{
		if (HAL_CAN_Start(&hcan1) != HAL_OK) Error_Handler();
	}
	
	
}

// 阻塞式发送
void MyCAN_Transmit(MyCAN_Tx_Msg *Tx_Msg)
{
	uint32_t TransmitMailbox;
	uint32_t Timeout;

	if (Tx_Msg == NULL || Tx_Msg->Data == NULL)
		return;

	if (Tx_Msg->TxMessage.DLC > 8U)
		return;

	if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0U)
		return;

	if (HAL_CAN_AddTxMessage(
			&hcan1,
			&Tx_Msg->TxMessage,
			Tx_Msg->Data,
			&TransmitMailbox) != HAL_OK)
	{
		return;
	}

	Timeout = HAL_GetTick();

	while (HAL_CAN_IsTxMessagePending(&hcan1, TransmitMailbox) != 0U)
	{
		if ((uint32_t)(HAL_GetTick() - Timeout) >= 100U)
		{
			HAL_CAN_AbortTxRequest(&hcan1, TransmitMailbox);
			return;
		}
	}
}

// 判断状态
uint8_t MyCAN_ReceiveFlag(void)
{
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0U)
	{
		return 1;
	}
	return 0;
}

// 轮训式接收
HAL_StatusTypeDef MyCAN_Receive(MyCAN_Rx_Msg *Rx_Msg)
{
	if (Rx_Msg == NULL || Rx_Msg->Data == NULL)
		return HAL_ERROR;

	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0U)
		return HAL_BUSY;

	if (HAL_CAN_GetRxMessage(
			&hcan1,
			CAN_RX_FIFO0,
			&Rx_Msg->RxMessage,
			Rx_Msg->Data) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (Rx_Msg->RxMessage.DLC > 8U)
		return HAL_ERROR;

	return HAL_OK;
}

