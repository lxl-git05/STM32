#ifndef __BSP_AT24C02_H__
#define __BSP_AT24C02_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* AT24C02 I2C Address */
#define AT24C02_ADDRESS_READ    0xA0
#define AT24C02_ADDRESS_WRITE   0xA1

/* I2C Pin Define - AT24C02 专用IO控制宏 */
#define AT24C02_SCL(x)   do { if (x) HAL_GPIO_WritePin(AT_SCL_GPIO_Port, AT_SCL_Pin, GPIO_PIN_SET); \
                              else   HAL_GPIO_WritePin(AT_SCL_GPIO_Port, AT_SCL_Pin, GPIO_PIN_RESET); \
                            } while(0)
#define AT24C02_SDA(x)   do { if (x) HAL_GPIO_WritePin(AT_SDA_GPIO_Port, AT_SDA_Pin, GPIO_PIN_SET); \
                              else   HAL_GPIO_WritePin(AT_SDA_GPIO_Port, AT_SDA_Pin, GPIO_PIN_RESET); \
                            } while(0)
#define AT24C02_SDA_GET()   HAL_GPIO_ReadPin(AT_SDA_GPIO_Port, AT_SDA_Pin)

/* SDA方向控制 - AT24C02使用软件I2C，SDA在读取时需要切换方向 */
/* PB15 = SDA, 使用寄存器直接切换GPIO方向 */
#define AT24C02_SDA_OUT()   do { AT_SDA_GPIO_Port->CRH = (AT_SDA_GPIO_Port->CRH & 0xFFFFFFF0) | 0x00000003; } while(0)  // 输出模式(50MHz)
#define AT24C02_SDA_IN()    do { AT_SDA_GPIO_Port->CRH = (AT_SDA_GPIO_Port->CRH & 0xFFFFFFF0) | 0x00000004; } while(0)  // 输入模式(浮动)

/* Delay function */
void delay_us(uint16_t us);

/* I2C Bus Operations */
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Ack(unsigned char ack);
unsigned char I2C_WaitAck(void);
void Send_Byte(uint8_t dat);
unsigned char Read_Byte(void);

/* AT24C02 Operations */
void AT24C02_WriteByte(unsigned char WordAddress, unsigned char Data);
unsigned char AT24C02_ReadByte(unsigned char WordAddress);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_AT24C02_H__ */
