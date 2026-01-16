#include "NRF24L01.h"
#include "NRF24L01_Define.h"

// 有效载荷字节数
#define NRF24L01_TX_PACKET_WIDTH 4
#define NRF24L01_RX_PACKET_WIDTH 4

// 收发地址
uint8_t NRF24L01_TxAddress[5] = {0x11 , 0x22 , 0x33 , 0x44 , 0x55} ;
uint8_t NRF24L01_RxAddress[5] = {0x11 , 0x22 , 0x33 , 0x44 , 0x55} ;

// 全局数组
uint8_t NRF_TxPacket[NRF24L01_TX_PACKET_WIDTH] ;
uint8_t NRF_RxPacket[NRF24L01_RX_PACKET_WIDTH] ;


// ============= 引脚配置 =============
void NRF_W_CE(uint8_t BitValue)
{
	HAL_GPIO_WritePin(NRF_CE_GPIO_Port , NRF_CE_Pin , (GPIO_PinState)BitValue) ;
}

void NRF_W_CSN(uint8_t BitValue)
{
	HAL_GPIO_WritePin(NRF_CSN_GPIO_Port , NRF_CSN_Pin , (GPIO_PinState)BitValue) ;
}

void NRF_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(NRF_SCK_GPIO_Port , NRF_SCK_Pin , (GPIO_PinState)BitValue) ;
}

void NRF_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(NRF_MOSI_GPIO_Port , NRF_MOSI_Pin , (GPIO_PinState)BitValue) ;
}

uint8_t NRF_R_MISO(void)
{
	return HAL_GPIO_ReadPin(NRF_MISO_GPIO_Port , NRF_MISO_Pin) ;
}

// ============= 初始化 =============
void NRF24L01_Init(void)
{
	// 引脚:Output的为推挽输出 , input的MISO为上拉输入
	//#define NRF_CE_Pin GPIO_PIN_0
	//#define NRF_CE_GPIO_Port GPIOA
	//#define NRF_CSN_Pin GPIO_PIN_1
	//#define NRF_CSN_GPIO_Port GPIOA
	//#define NRF_SCK_Pin GPIO_PIN_2
	//#define NRF_SCK_GPIO_Port GPIOA
	//#define NRF_MOSI_Pin GPIO_PIN_3
	//#define NRF_MOSI_GPIO_Port GPIOA
	//#define NRF_MISO_Pin GPIO_PIN_4
	//#define NRF_MISO_GPIO_Port GPIOA
	// 初始状态
	NRF_W_CE(0)  ;	// 默认退出收发模式
	NRF_W_CSN(1) ;	// 默认高电平,也就是不选中从机
	NRF_W_SCK(0) ;	// 时钟默认初始为低电平,符合SPI模式0
	NRF_W_MOSI(0);
}

// ============= 通信协议 =============
uint8_t NRF_SPI_SwapByte(uint8_t ByteSend)	// 前提:事先开启了从机
{
	for(int i = 0 ; i < 8 ; i ++)
	{
		// SPI移出数据
		if (ByteSend & 0x80)
		{
			NRF_W_MOSI(1) ;
		}
		else
		{
			NRF_W_MOSI(0) ;
		}
		ByteSend <<= 1 ;			// 高位丢弃
		
		// 时钟上升沿
		NRF_W_SCK(1) ;
		
		// SPI移入数据
		if (NRF_R_MISO())
		{
			ByteSend |= 0x01 ;	// 低位移入,真实模拟移位寄存器的工作方式
		}
		
		// 时钟下降沿
		NRF_W_SCK(0) ;
	}
	// 返回数据
	return ByteSend  ;
}

// ============ 指令实现 ============
// 写指令
void NRF_WriteReg(uint8_t RedAddress , uint8_t Data)
{
	NRF_W_CSN(0) ;
	NRF_SPI_SwapByte(NRF24L01_W_REGISTER | RedAddress) ;		// 写指令的格式是0x001AAAAAA,A为任意,所以需要 | 0x20
	NRF_SPI_SwapByte(Data) ;
	NRF_W_CSN(1) ;
}
// 读数据
uint8_t NRF_ReadReg(uint8_t RedAddress) 
{
	uint8_t Data = 0x00;
	
	NRF_W_CSN(0) ;
	NRF_SPI_SwapByte(NRF24L01_R_REGISTER | RedAddress) ;
	Data = NRF_SPI_SwapByte(NRF24L01_NOP) ;
	NRF_W_CSN(1) ;
	
	return Data ;
}

// 写多指令
void NRF_WriteRegs(uint8_t RedAddress , uint8_t *DataArr , uint8_t Count)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_W_REGISTER | RedAddress) ;		// 写指令的格式是0x001AAAAAA,A为任意,所以需要 | 0x20
	
	for(int i = 0 ; i < Count ; i ++)
	{
		NRF_SPI_SwapByte(DataArr[i]) ;
	}
	
	NRF_W_CSN(1) ;
}
// 读多数据
void NRF_ReadRegs(uint8_t RedAddress , uint8_t *DataArr , uint8_t Count)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_R_REGISTER | RedAddress) ;
	for (int i = 0 ; i < Count ; i ++)
	{
		DataArr[i] = NRF_SPI_SwapByte(NRF24L01_NOP) ;
	}
	
	NRF_W_CSN(1) ;
}

// 写TX的FIFO指令
void NRF_WriteTxPayload(uint8_t *DataArr , uint8_t Count)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_W_TX_PAYLOAD) ;		// 写指令的格式是0x001AAAAAA,A为任意,所以需要 | 0x20
	
	for(int i = 0 ; i < Count ; i ++)
	{
		NRF_SPI_SwapByte(DataArr[i]) ;
	}
	
	NRF_W_CSN(1) ;
}
// 读RX的FIFO指令
void NRF_ReadRxPayload(uint8_t *DataArr , uint8_t Count)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_R_RX_PAYLOAD) ;
	for (int i = 0 ; i < Count ; i ++)
	{
		DataArr[i] = NRF_SPI_SwapByte(NRF24L01_NOP) ;
	}
	
	NRF_W_CSN(1) ;
}

// 清空发送FIFO
void NRF_FlushTx(void)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_FLUSH_TX) ;
	
	NRF_W_CSN(1) ;
}


// 清空接收FIFO
void NRF_FlushRx(void)
{
	NRF_W_CSN(0) ;
	
	NRF_SPI_SwapByte(NRF24L01_FLUSH_RX) ;
	
	NRF_W_CSN(1) ;
}

// 快速读状态寄存器
uint8_t NRF_ReadStatus(void)
{
	uint8_t Status = 0 ;
	
	NRF_W_CSN(0) ;
	
	Status = NRF_SPI_SwapByte(NRF24L01_NOP) ;
	
	NRF_W_CSN(1) ;
	
	return Status ; 
}

// ================ 功能函数 ================
// 掉电模式
void NRF_PowerDown(void)
{
	uint8_t Config ;
	
	NRF_W_CE(0) ;
	
	Config = NRF_ReadReg(NRF24L01_CONFIG);
	
	Config &= ~0x02 ;
	
	NRF_WriteReg(NRF24L01_CONFIG , Config) ;
}

// 待机模式1
void NRF_StandbyI(void)
{
	uint8_t Config ;
	
	NRF_W_CE(0) ;
	
	Config = NRF_ReadReg(NRF24L01_CONFIG);
	
	Config |= 0x02 ;	// 把PowerUp置1即可
	
	NRF_WriteReg(NRF24L01_CONFIG , Config) ;
}

// 接收模式
void NRF_RxMode(void)
{
	uint8_t Config ;
	
	NRF_W_CE(0) ;
	
	Config = NRF_ReadReg(NRF24L01_CONFIG);
	
	Config |= 0x03 ;	// 把后两位都置1
	
	NRF_WriteReg(NRF24L01_CONFIG , Config) ;
	
	NRF_W_CE(1) ;
}

// 发送模式
void NRF_TxMode(void)
{
	uint8_t Config ;
	
	NRF_W_CE(0) ;
	
	Config = NRF_ReadReg(NRF24L01_CONFIG);
	
	Config |= 0x02 ;	// 把Up置1
	Config &= ~0x01 ;	// 把PRIM_RX置0
	
	NRF_WriteReg(NRF24L01_CONFIG , Config) ;
	
	NRF_W_CE(1) ;
}

// 初始化函数
void NRF_Init(void)
{
	NRF24L01_Init() ;
	
	/*以下配置通信双方必须保持一致，否则无法进行通信*/
	NRF_WriteReg(NRF24L01_CONFIG, 0x08);		//配置寄存器，不屏蔽中断，使能CRC，CRC为1字节，PWR_UP = 0，PRIM_RX = 0
	NRF_WriteReg(NRF24L01_EN_AA, 0x3F);		//使能自动应答，开启接收通道0~通道5的自动应答
	NRF_WriteReg(NRF24L01_EN_RXADDR, 0x01);	//使能接收通道，只开启接收通道0
	NRF_WriteReg(NRF24L01_SETUP_AW, 0x03);		//设置地址宽度，地址宽度为5字节
	NRF_WriteReg(NRF24L01_SETUP_RETR, 0x03);	//设置自动重传，间隔250us，重传3次
	NRF_WriteReg(NRF24L01_RF_CH, 0x02);		//射频通道，频率为(2400 + 2)MHz = 2.402GHz
	NRF_WriteReg(NRF24L01_RF_SETUP, 0x0E);		//射频设置，通信速率为2Mbps，发射功率为0dBm
	
	/*接收通道0的数据包宽度，设置为宏定义NRF24L01_RX_PACKET_WIDTH指定的值*/
	NRF_WriteReg(NRF24L01_RX_PW_P0, NRF24L01_RX_PACKET_WIDTH);
	
	/*接收通道0地址，设置为全局数组NRF24L01_RxAddress指定的地址，地址宽度固定为5字节*/
	NRF_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);
	
	/*清空Tx FIFO的所有数据*/
	NRF_FlushTx();
	
	/*清空Rx FIFO的所有数据*/
	NRF_FlushRx();
	
	/*给状态寄存器的位4（MAX_RT）、位5（TX_DS）和位6（RX_DR）写1，清标志位*/
	NRF_WriteReg(NRF24L01_STATUS, 0x70);
	
	/*初始化配置完成，芯片默认进入接收模式*/
	NRF_RxMode();
}

// 发送数据
uint8_t NRF24L01_Send(void)
{
	uint8_t Status;
	uint8_t SendFlag;
	uint32_t Timeout;
	
	/*发送地址，设置为全局数组NRF24L01_TxAddress指定的地址，地址宽度固定为5字节*/
	NRF_WriteRegs(NRF24L01_TX_ADDR, NRF24L01_TxAddress, 5);
	
	/*接收通道0地址，此处必须也设置为发送地址，用于接收应答*/
	NRF_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_TxAddress, 5);
	
	/*写发送有效载荷，写入全局数组NRF24L01_TxPacket指定的数据，数据宽度为NRF24L01_TX_PACKET_WIDTH*/
	NRF_WriteTxPayload(NRF_TxPacket, NRF24L01_TX_PACKET_WIDTH);
	
	/*发送的地址和有效载荷写入完成，进入发送模式，开始发送数据*/
	NRF_TxMode();
	
	/*指定超时时间，即循环读取状态寄存器的次数，具体值可以实测确定*/
	Timeout = 10000;
	
	/*循环读取状态寄存器*/
	while (1)
	{
		/*读取状态寄存器，保存至Status变量*/
		Status = NRF_ReadStatus();
		
		/*超时计次*/
		Timeout --;
		if (Timeout == 0)			//如果计次减至0
		{
			SendFlag = 4;			//发送超时，置标志位为4
			NRF_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}
		
		/*根据状态寄存器的值，判断发送状态*/
		if ((Status & 0x30) == 0x30)		//状态寄存器位4（MAX_RT）和位5（TX_DS）同时为1
		{
			SendFlag = 3;			//状态寄存器的值不合法，置标志位为3
			NRF_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}
		else if ((Status & 0x10) == 0x10)	//状态寄存器位4（MAX_RT）为1
		{
			SendFlag = 2;			//达到了最大重发次数仍未收到应答，置标志位为2
			NRF_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}
		else if ((Status & 0x20) == 0x20)	//状态寄存器位5（TX_DS）为1
		{
			SendFlag = 1;			//发送成功，无错误，置标志位为1
			break;					//跳出循环
		}
	}
	
	/*给状态寄存器的位4（MAX_RT）和位5（TX_DS）写1，清标志位*/
	NRF_WriteReg(NRF24L01_STATUS, 0x30);
	
	/*清空Tx FIFO的所有数据*/
	NRF_FlushTx();
	
	/*发送完成后，恢复接收通道0原来的地址*/
	/*如果发送地址和接收通道0地址设置相同，则可不执行这一句*/
	NRF_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);
	
	/*发送完成，芯片恢复为接收模式*/
	NRF_RxMode();
		
	/*返回发送标志位*/
	return SendFlag;
}

// NRF24L01接收数据包
uint8_t NRF_Receive(void)
{
	uint8_t Status, Config;
	uint8_t ReceiveFlag;
	
	/*读取状态寄存器，保存至Status变量*/
	Status = NRF_ReadStatus();
	
	/*读取配置寄存器，保存至Config变量*/
	Config = NRF_ReadReg(NRF24L01_CONFIG);
	
	/*根据配置寄存器和状态寄存器的值，判断接收状态*/
	if ((Config & 0x02) == 0x00)		//配置寄存器位1（PWR_UP）为0
	{
		ReceiveFlag = 3;				//设备仍处于掉电模式，置标志位为3
		NRF_Init();				//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
	}
	else if ((Status & 0x30) == 0x30)	//状态寄存器位4（MAX_RT）和位5（TX_DS）同时为1
	{
		ReceiveFlag = 2;				//状态寄存器的值不合法，置标志位为2
		NRF_Init();				//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
	}
	else if ((Status & 0x40) == 0x40)	//状态寄存器位6（RX_DR）为1
	{
		ReceiveFlag = 1;				//接收到数据，置标志位为1
		
		/*读接收有效载荷，存放在全局数组NRF24L01_RxPacket中，数据宽度为NRF24L01_RX_PACKET_WIDTH*/
		NRF_ReadRxPayload(NRF_RxPacket, NRF24L01_RX_PACKET_WIDTH);
		
		/*给状态寄存器的位6（RX_DR）写1，清标志位*/
		NRF_WriteReg(NRF24L01_STATUS, 0x40);

		/*清空Rx FIFO的所有数据*/
		NRF_FlushRx();
	}
	else
	{
		ReceiveFlag = 0;				//未接收到数据，置标志位为0
	}
	
	/*返回接收标志位*/
	return ReceiveFlag;
}

// 更新接收地址
void NRF_UpdateRxAddress(void)
{
	/*接收通道0地址，设置为全局数组NRF24L01_RxAddress指定的地址，地址宽度固定为5字节*/
	NRF_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);
}
