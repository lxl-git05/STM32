#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "main.h"

// ============= 全局变量 =============

extern uint8_t NRF_TxPacket[] ;
extern uint8_t NRF_RxPacket[] ;

// ============= 初始化 =============

void NRF_Init(void);

void NRF_W_CSN(uint8_t BitValue) ;

// ============= 通信协议 ============
// 数据交换
uint8_t NRF_SPI_SwapByte(uint8_t ByteSend);	

// 写指令
void NRF_WriteReg(uint8_t RedAddress , uint8_t Data);

// 读数据
uint8_t NRF_ReadReg(uint8_t RedAddress)  ;

// 写多指令
void NRF_WriteRegs(uint8_t RedAddress , uint8_t *DataArr , uint8_t Count);

// 读多数据
void NRF_ReadRegs(uint8_t RedAddress , uint8_t *DataArr , uint8_t Count);

// 发送数据
uint8_t NRF24L01_Send(void);

// 接收数据
uint8_t NRF_Receive(void);

#endif
