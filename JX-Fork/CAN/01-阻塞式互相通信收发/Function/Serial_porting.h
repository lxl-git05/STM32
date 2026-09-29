#ifndef __SERIAL_PORTING_H
#define __SERIAL_PORTING_H

#include "Serial_base.h"

typedef struct {
    USART_TypeDef *Instance;
    UART_HandleTypeDef *huart;
    uint8_t rxBuf[Serial_RX_BUF_SIZE];
    uint16_t rxLen;
    Serial_ABC_Data_Typedef ABC_Data;
    Serial_HEX_Data_Typedef HEX_Data;
} Serial_Typedef;

#define Serial2_Enable 1

extern Serial_Typedef Serial1;
extern Serial_Typedef Serial2;

void Serial_Init(void);
void Serial_printf(Serial_Typedef *pSerial, const char *fmt, ...);

int16_t Serial_GetHexData(Serial_Typedef *pSerial, uint8_t index);
uint8_t Serial_GetHexLen(Serial_Typedef *pSerial);
uint8_t Serial_GetNewPackageFlag_HEX(Serial_Typedef *pSerial);
int Serial_GetError_HEX(Serial_Typedef *pSerial);

uint8_t Serial_GetNewPackageFlag_ABC(Serial_Typedef *pSerial);
int Serial_GetError_ABC(Serial_Typedef *pSerial);
bool Serial_SetFloatData(Serial_Typedef *pSerial, char *KeyWord, char *cmd, float *Data);
bool Serial_SetIntData(Serial_Typedef *pSerial, char *KeyWord, char *cmd, int *Data);
bool Serial_Check_Str(Serial_Typedef *pSerial, char *KeyWord);
bool Serial_CheckCmd(Serial_Typedef *pSerial, char *cmd);

void Serial_send_string(Serial_Typedef *pSerial, char *str);
void Serial_SendBytes(Serial_Typedef *pSerial, uint8_t *buf, uint16_t len);
void Serial_Send_HEX_Package(Serial_Typedef *pSerial, uint16_t *data, uint8_t count);

void Serial_Clear_ABC(Serial_Typedef *pSerial);
void Serial_PrintDebug(Serial_Typedef *pSerial);

/* Called by the unified interrupt layer. */
void Serial_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size);
void Serial_ErrorCallback(UART_HandleTypeDef *huart);

#endif
