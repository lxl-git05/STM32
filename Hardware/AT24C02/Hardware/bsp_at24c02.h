#ifndef __BSP_AT24C02_H__
#define __BSP_AT24C02_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
/* AT24C02 7-bit设备地址 = 0x50，左移1位 + 读写位 = 0xA0(写) / 0xA1(读) */
#define AT24C02_ADDR_WRITE    0xA0
#define AT24C02_ADDR_READ     0xA1

/* Exported functions --------------------------------------------------------*/
void     AT24C02_Init(void);
uint8_t  AT24C02_WriteByte(uint8_t WordAddress, uint8_t Data);
uint8_t  AT24C02_ReadByte(uint8_t WordAddress);

#endif /* __BSP_AT24C02_H__ */
