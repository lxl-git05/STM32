
#ifndef __SERIAL_PORTING_H
#define __SERIAL_PORTING_H

#include "Serial_base.h"
#include <stdint.h>
#include <stdbool.h>

// DMA TX环形缓冲区大小
#define Serial_TX_BUF_SIZE 1024U

typedef struct
{
    // UART硬件
    USART_TypeDef *Instance;
    UART_HandleTypeDef *huart;

    // =========================
    // RX接收
    // =========================

    uint8_t rxBuf[Serial_RX_BUF_SIZE];
    uint16_t rxLen;

    Serial_ABC_Data_Typedef ABC_Data;
    Serial_HEX_Data_Typedef HEX_Data;

    // =========================
    // TX DMA发送
    // =========================

    uint8_t txBuf[Serial_TX_BUF_SIZE];

    // 下一个写入位置
    volatile uint16_t txHead;

    // 尚未发送的数据起始位置
    volatile uint16_t txTail;

    // 当前DMA正在发送的长度
    // 0表示DMA空闲
    volatile uint16_t txDmaLen;

    // 发送缓冲区空间不足的次数
    volatile uint32_t txOverflow;

} Serial_Typedef;


// =========================
// 全局串口对象
// =========================

extern Serial_Typedef Serial1;


// =========================
// 初始化
// =========================

HAL_StatusTypeDef Serial_Init(void);


// =========================
// TX发送接口
// =========================

void Serial_printf(
    Serial_Typedef *pSerial,
    const char *fmt,
    ...
);

HAL_StatusTypeDef Serial_SendBytes(
    Serial_Typedef *pSerial,
    const uint8_t *buf,
    uint16_t len
);

void Serial_send_string(
    Serial_Typedef *pSerial,
    char *str
);

void Serial_Send_HEX_Package(
    Serial_Typedef *pSerial,
    uint16_t *data,
    uint8_t count
);

// 尝试启动等待中的DMA发送
HAL_StatusTypeDef Serial_TxService(
    Serial_Typedef *pSerial
);


// =========================
// HEX协议接口
// =========================

int16_t Serial_GetHexData(
    Serial_Typedef *pSerial,
    uint8_t index
);

uint8_t Serial_GetHexLen(
    Serial_Typedef *pSerial
);

uint8_t Serial_GetNewPackageFlag_HEX(
    Serial_Typedef *pSerial
);

int Serial_GetError_HEX(
    Serial_Typedef *pSerial
);


// =========================
// ABC协议接口
// =========================

uint8_t Serial_GetNewPackageFlag_ABC(
    Serial_Typedef *pSerial
);

int Serial_GetError_ABC(
    Serial_Typedef *pSerial
);

bool Serial_SetFloatData(
    Serial_Typedef *pSerial,
    char *KeyWord,
    char *cmd,
    float *Data
);

bool Serial_SetIntData(
    Serial_Typedef *pSerial,
    char *KeyWord,
    char *cmd,
    int *Data
);

bool Serial_Check_Str(
    Serial_Typedef *pSerial,
    char *KeyWord
);

bool Serial_CheckCmd(
    Serial_Typedef *pSerial,
    char *cmd
);

void Serial_Clear_ABC(
    Serial_Typedef *pSerial
);


// =========================
// 调试
// =========================

void Serial_PrintDebug(
    Serial_Typedef *pSerial
);


// =========================
// ISR统一转发接口
// =========================

// DMA/空闲接收事件
void Serial_RxEventCallback(
    UART_HandleTypeDef *huart,
    uint16_t size
);

// UART错误事件
void Serial_ErrorCallback(
    UART_HandleTypeDef *huart
);

// DMA发送完成事件
void Serial_TxCpltCallback(
    UART_HandleTypeDef *huart
);

#endif
