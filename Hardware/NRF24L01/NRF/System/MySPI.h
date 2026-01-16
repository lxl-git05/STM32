#ifndef __MYSPI_H
#define __MYSPI_H

#include "main.h"

//#define SPI_ON
//#define SPI_Software_Mode_Enable	// 注释该语句表示开启硬件SPI

#ifdef SPI_ON
// SPI初始化
void MySPI_Init(void);
	
// 开启SPI通信
void MySPI_Start(void);

// 结束SPI通信
void MySPI_Stop(void);

// 交换一个字节,主机发送ByteSend,接收返回值,本函数实现的是模式0功能,改为模式1,2,3自行研究
uint8_t MySPI_SwapByte(uint8_t ByteSend);
#endif

#endif
