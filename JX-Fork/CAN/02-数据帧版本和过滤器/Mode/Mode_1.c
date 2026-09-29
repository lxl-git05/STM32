#include "AllHeader.h"

// 1. CAN环回测试
// 描述: 自发自收

// 发送
MyCAN_Tx_Msg TxMsg [] =
{
    {{0x555, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x000, 0x12345678, CAN_ID_EXT, CAN_RTR_DATA , 4, DISABLE},{0xAA, 0xBB, 0xCC, 0xDD}} ,
		{{0x666, 0x00000000, CAN_ID_STD, CAN_RTR_REMOTE , 0, DISABLE},{0x00, 0x00, 0x00, 0x00}} ,
		{{0x000, 0x0789ABCD, CAN_ID_EXT, CAN_RTR_REMOTE , 0, DISABLE},{0x00, 0x00, 0x00, 0x00}} ,
};

uint8_t pTxMsgArray = 0;

// 接收
MyCAN_Rx_Msg RxMsg ;

void Mode_1_Setup(void) 
{
	// 本工程默认是环回模式,配置正常模式就解除当前注释即可
	
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
	
	MyCAN_Init();
	
}
void Mode_1_Loop(void)
{
	OLED_Printf(0, 0, OLED_6X8, "===Mode_1===");
	
	// 发送
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		MyCAN_Transmit(&TxMsg[pTxMsgArray]);
		
		pTxMsgArray ++;
		if (pTxMsgArray >= sizeof(TxMsg) / sizeof(MyCAN_Tx_Msg))
		{
			pTxMsgArray = 0;
		}
	}
	
	// 接收
	if (MyCAN_ReceiveFlag())
	{
		MyCAN_Receive(&RxMsg);
	}
	
	// 展示
//	bool isTxStd = TxMsg[pTxMsgArray].TxMessage.IDE == CAN_ID_STD ? true : false ;
	bool isRxStd = RxMsg.RxMessage.IDE == CAN_ID_STD ? true : false ;
	
//	int TxId = isTxStd ? TxMsg[pTxMsgArray].TxMessage.StdId : TxMsg[pTxMsgArray].TxMessage.ExtId ;
	int RxId = isRxStd ? RxMsg.RxMessage.StdId : RxMsg.RxMessage.ExtId ;
	
//	OLED_Printf(0, 15, OLED_6X8, "TxID:%s %x",isTxStd ? "Std" : "Ext" , TxId);	// 其实有点干扰
	OLED_Printf(0, 25, OLED_6X8, "RxID:%s %x",isRxStd ? "Std" : "Ext" , RxId);
	OLED_Printf(0, 35, OLED_6X8, "Leng:%x",TxMsg[pTxMsgArray].TxMessage.DLC);
	OLED_Printf(0, 45, OLED_6X8, "Data:%02x %02x %02x %02x",
		RxMsg.Data[0],RxMsg.Data[1] ,RxMsg.Data[2] ,RxMsg.Data[3] );
	OLED_Printf(0, 15, OLED_6X8, "Type:%s",RxMsg.RxMessage.RTR == CAN_RTR_DATA ? "Data" : "Remote");
}
void Mode_1_Tick(void) {}
void Mode_1_Exit(void) {}
	
