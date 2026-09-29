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
void MyCAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data)
{
	CAN_TxHeaderTypeDef TxMessage = {0};
	uint8_t TxData[8] = {0};
	uint32_t TransmitMailbox;
	uint32_t Timeout;

	if (ID > 0x7FFU || Length > 8U || (Length > 0U && Data == NULL)) return;
	if (HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_LISTENING) return;

	TxMessage.StdId = ID;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = Length;
	TxMessage.TransmitGlobalTime = DISABLE;
	/* HAL reads all eight bytes even when DLC is smaller. */
	for (uint8_t i = 0; i < Length; i ++)
	{
		TxData[i] = Data[i];
	}

	if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0U) return;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxMessage, TxData, &TransmitMailbox) != HAL_OK) return;

	/* Call from the main loop with the HAL time base running. */
	Timeout = HAL_GetTick();
	while (HAL_CAN_IsTxMessagePending(&hcan1, TransmitMailbox) != 0U)
	{
		if ((uint32_t)(HAL_GetTick() - Timeout) >= 100U)
		{
			(void)HAL_CAN_AbortTxRequest(&hcan1, TransmitMailbox);
			break;
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
void MyCAN_Receive(uint32_t *ID, uint8_t *Length, uint8_t *Data)
{
	CAN_RxHeaderTypeDef RxMessage;
	uint8_t RxData[8];

	if (ID == NULL || Length == NULL || Data == NULL) return;
	*Length = 0;
	if (MyCAN_ReceiveFlag() == 0U) return;
	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxMessage, RxData) != HAL_OK) return;

	if (RxMessage.IDE == CAN_ID_STD)
	{
		*ID = RxMessage.StdId;
	}
	else
	{
		*ID = RxMessage.ExtId;
	}

	if (RxMessage.RTR == CAN_RTR_DATA && RxMessage.DLC <= 8U)
	{
		*Length = (uint8_t)RxMessage.DLC;
		for (uint8_t i = 0; i < *Length; i ++)
		{
			Data[i] = RxData[i];
		}
	}
}

