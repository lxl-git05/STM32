#include "AllHeader.h"

// 1. CAN环回测试
// 描述: 自发自收

uint8_t KeyNum;
uint32_t TxID = 0x555;
uint8_t TxLength = 4;
uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44};

uint32_t RxID;
uint8_t RxLength;
uint8_t RxData[8];

void Mode_1_Setup(void) 
{
	MyCAN_Init();
//	hcan1.Init.Mode = CAN_MODE_LOOPBACK ;	// 如果想要使用环回模式测试硬件,取消这里的注释即可
}
void Mode_1_Loop(void)
{
	OLED_Printf(0, 0, OLED_6X8, "===Mode_1===");
	
	// 发送
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		TxData[0] ++;
		TxData[1] ++;
		TxData[2] ++;
		TxData[3] ++;
		
		MyCAN_Transmit(TxID, TxLength, TxData);
	}
	
	// 接收
	if (MyCAN_ReceiveFlag())
	{
		MyCAN_Receive(&RxID, &RxLength, RxData);
	}
	
	// 展示
	OLED_Printf(0, 15, OLED_6X8, "TxID:%x",TxID);
	OLED_Printf(0, 25, OLED_6X8, "RxID:%x",RxID);
	OLED_Printf(0, 35, OLED_6X8, "Leng:%x",TxLength);
	OLED_Printf(0, 45, OLED_6X8, "Data:%02x %02x %02x %02x",RxData[0],RxData[1] ,RxData[2] ,RxData[3] );
}
void Mode_1_Tick(void) {}
void Mode_1_Exit(void) {}
	
