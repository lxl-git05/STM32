#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h"
#include "MySPI.h"


// 初始化
void W25Q64_Init(void) ;

// 读取ID号:MID:厂商ID , DID:设备ID
void W25Q64_ReadID(uint8_t *MID , uint16_t *DID) ;

// 页编程
void W25Q64_PageProgram(uint32_t Address , uint8_t *DataArr , uint8_t Count);

// 页擦除(扇区擦除)
void W25Q64_SectorErase(uint32_t Address)	;

// 页读取:不像页编程(受RAM缓冲区制约),页读取是没有页数限制的
void W25Q64_ReadData(uint32_t Address , uint8_t *DataArr , uint32_t Count) ;
	
#endif
