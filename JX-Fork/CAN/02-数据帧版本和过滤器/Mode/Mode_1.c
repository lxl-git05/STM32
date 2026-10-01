#include "AllHeader.h"

//#define List_16 1	// 16位列表: 目标是0x234,0x345,0x456
//#define Mask_16 1
//#define List_32 1
#define Mask_32 1

// 1. CAN环回测试
// 描述: 自发自收

// 发送
MyCAN_Tx_Msg TxMsg [] =
{
#ifdef List_16
    {{0x123, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x234, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x345, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x456, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x567, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
		{{0x678, 0x00000000, CAN_ID_STD, CAN_RTR_DATA , 4, DISABLE},{0x11, 0x22, 0x33, 0x44}} ,
#endif
#ifdef Mask_16
		{0x100, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x101, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x1FE, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x1FF, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		
		{0x200, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x201, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x2FE, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x2FF, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		
		{0x310, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x311, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x31E, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x31F, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		
		{0x320, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x321, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x32E, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
		{0x32F, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4, DISABLE, {0x11, 0x22, 0x33, 0x44}},
#endif
#ifdef List_32
	{0x123, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x234, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x345, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x456, 0x00000000, CAN_ID_STD, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	
	{0x000, 0x12345678, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x0789ABCD, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
#endif
#ifdef Mask_32
	{0x000, 0x12345600, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x12345601, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x123456FE, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x123456FF, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	
	{0x000, 0x0789AB00, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x0789AB01, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x0789ABFE, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
	{0x000, 0x0789ABFF, CAN_ID_EXT, CAN_RTR_DATA,   4,DISABLE, {0x11, 0x22, 0x33, 0x44}},
#endif
		// 各种格式
//		{{0x000, 0x12345678, CAN_ID_EXT, CAN_RTR_DATA , 4, DISABLE},{0xAA, 0xBB, 0xCC, 0xDD}} ,
//		{{0x666, 0x00000000, CAN_ID_STD, CAN_RTR_REMOTE , 0, DISABLE},{0x00, 0x00, 0x00, 0x00}} ,
//		{{0x000, 0x0789ABCD, CAN_ID_EXT, CAN_RTR_REMOTE , 0, DISABLE},{0x00, 0x00, 0x00, 0x00}} ,
};

uint8_t pTxMsgArray = 0;

// 接收
MyCAN_Rx_Msg RxMsg ;

void Mode_1_Setup(void) 
{
	// 本工程默认是环回模式,配置正常模式就解除当前注释即可
	/*
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
	*/
	CAN_FilterTypeDef Filter = {0};	// 0号过滤器
	Filter.FilterBank = 0;
	Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	Filter.FilterActivation = ENABLE;
	Filter.SlaveStartFilterBank = 14;
	#ifdef List_16
	Filter.FilterIdHigh = 0x234 << 5 | 0x00 ;	// (RTR = 0 , IDE = 0)
	Filter.FilterIdLow = 0x345 << 5 ;
	Filter.FilterMaskIdHigh = 0x567 << 5 ;
	Filter.FilterMaskIdLow = 0x000 << 5;
	Filter.FilterScale = CAN_FILTERSCALE_16BIT;		// 16bit
	Filter.FilterMode = CAN_FILTERMODE_IDLIST;		// LIST
	MyCAN_Filter_Init(&Filter) ;
	#elif Mask_16
	Filter.FilterIdHigh = 0x200 << 5 ;				// (RTR = 0 , IDE = 0)	// 高位是MASK
	Filter.FilterMaskIdHigh = 0x700 << 5 | 0x18 ;// (RTR = 0是需要过滤入的 , IDE = 0是需要过滤入的)	// 低位是ID
	
	Filter.FilterIdLow = 0x320 << 5 ;					// 过滤ID为 011 0010 xxxx, 所以是0x7F0 << 5 | (RTR << 5 | IDE << 4)
	Filter.FilterMaskIdLow = (0x7F0 << 5) | 0x18 ;
	
	Filter.FilterScale = CAN_FILTERSCALE_16BIT;		// 16bit
	Filter.FilterMode = CAN_FILTERMODE_IDMASK;		// MASK
	MyCAN_Filter_Init(&Filter) ;
	#elif List_32
	// 11 + 18 + RTR + IDE + 0
	uint32_t data_1 = 0x123 << 21 | 0x0 ;		// IDE = 0 RTR = 0 0
	Filter.FilterIdHigh = data_1 >> 16 ;				
	Filter.FilterIdLow = data_1 & 0x0000FFFF ;	// 低16位		
	
	uint32_t data_2 = 0x12345678u << 3 | 0x4 ; 	// IDE = 1 RTR = 0 0
	Filter.FilterMaskIdHigh = data_2 >> 16 ;
	Filter.FilterMaskIdLow = data_2 ;
	
	Filter.FilterScale = CAN_FILTERSCALE_32BIT;		// 32bit
	Filter.FilterMode = CAN_FILTERMODE_IDLIST;		// LIST
	MyCAN_Filter_Init(&Filter) ;
	#elif Mask_32
	// 11 + 18 + RTR + IDE + 0
	uint32_t data_1 = 0x12345600u << 3 | 0x4;		// IDE = 1 RTR = 0 0
	Filter.FilterIdHigh = data_1 >> 16 ;				
	Filter.FilterIdLow = data_1 ;	// 低16位,会截断		
	
	uint32_t data_2 = 0x1FFFFF00u << 3 | 0x6; 	// IDE = 1 RTR = 0 0	(注意!!!过滤器是需要关注IDE和RTR的)
	Filter.FilterMaskIdHigh = data_2 >> 16 ;
	Filter.FilterMaskIdLow = data_2 ;
	
	Filter.FilterScale = CAN_FILTERSCALE_32BIT;		// 32bit
	Filter.FilterMode = CAN_FILTERMODE_IDMASK;		// MASK
	MyCAN_Filter_Init(&Filter) ;
	#else
	MyCAN_Init();		// 全通
	#endif
	
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
	
