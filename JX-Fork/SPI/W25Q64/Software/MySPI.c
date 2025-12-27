#include "MySPI.h"

// =================== 初始化 ===================
void MySPI_Init(void)
{
	// 软件SPI在HAL已经进行了初始化:
	//#define SPI_SS_Pin GPIO_PIN_4
	//#define SPI_SS_GPIO_Port GPIOA
	//#define SPI_CLK_Pin GPIO_PIN_5
	//#define SPI_CLK_GPIO_Port GPIOA
	//#define SPI_DO_Pin GPIO_PIN_6
	//#define SPI_DO_GPIO_Port GPIOA
	//#define SPI_DI_Pin GPIO_PIN_7
	//#define SPI_DI_GPIO_Port GPIOA
	// 记得SCK作为时钟先是0(从而满足模式0的第一个沿为上升沿)
	// SS最开始为1,表示不选中(因为是低电平有效)
}

// =================== 引脚操作 ===================

// 引脚操作:SS写入
void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_SS_GPIO_Port , SPI_SS_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:SCK写入
void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_CLK_GPIO_Port , SPI_CLK_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:MOSI(DI)写入
void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_DI_GPIO_Port , SPI_DI_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:MISO(DO)读取
uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(SPI_DO_GPIO_Port , SPI_DO_Pin) ;
}



// =================== 模拟时序 ===================
// 开启SPI通信
void MySPI_Start(void)
{
	MySPI_W_SS(0) ;
}
// 结束SPI通信
void MySPI_Stop(void)
{
	MySPI_W_SS(1) ;
}

// 交换一个字节,主机发送ByteSend,接收返回值,本函数实现的是模式0功能,改为模式1,2,3自行研究
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0x00 ;
	// 在for循环之前有SS下降沿,所以从机早就准备好了输出数据,并且存入到了移位寄存器,所以for_loop一开始就需要输出数据
	for (int i = 0 ; i < 8 ; i ++)
	{
		MySPI_W_MOSI( ByteSend & (0x80 >> i) ) ;	// 传输最高位给从机
		MySPI_W_SCK(1) ;	// 发出上升沿,此时从机发送从高位,主机下一步去读取
		if (MySPI_R_MISO() == 1) { ByteReceive |= (0x80 >> i) ; }
		MySPI_W_SCK(0) ;	// 下降沿,主机从机输出下一位数据到移位寄存器
	}
	
	return ByteReceive ;
}
