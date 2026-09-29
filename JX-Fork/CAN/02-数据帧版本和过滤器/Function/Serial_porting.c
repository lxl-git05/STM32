#include "Serial_porting.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

Serial_Typedef Serial1;
Serial_Typedef Serial2;

static void Serial_StartReceive(Serial_Typedef *pSerial)
{
    if (HAL_UARTEx_ReceiveToIdle_DMA(pSerial->huart, pSerial->rxBuf,
                                    Serial_RX_BUF_SIZE) != HAL_OK)
        Error_Handler();
    __HAL_DMA_DISABLE_IT(pSerial->huart->hdmarx, DMA_IT_HT);
}

void Serial_Init(void)
{
    Serial1.Instance = USART1;
    Serial1.huart = &huart1;
    Serial2.Instance = USART2;
    Serial2.huart = &huart2;
    Serial_Agreement_ABC_Init();
    Serial_Agreement_HEX_Init();

    Serial_StartReceive(&Serial1);
    Serial_StartReceive(&Serial2);
}

void Serial_printf(Serial_Typedef *pSerial, const char *fmt, ...)
{
    char buffer[256];
    va_list args;
    int len;
    if (pSerial == NULL || fmt == NULL) return;
    va_start(args, fmt);
    len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    if (len <= 0) return;
    if (len >= (int)sizeof(buffer)) len = sizeof(buffer) - 1;
    Serial_SendBytes(pSerial, (uint8_t *)buffer, (uint16_t)len);
}

int16_t Serial_GetHexData(Serial_Typedef *pSerial, uint8_t index)
{
    if (pSerial == NULL || index >= pSerial->HEX_Data.len) return 0;
    return pSerial->HEX_Data.data[index];
}

uint8_t Serial_GetHexLen(Serial_Typedef *pSerial)
{
    return pSerial == NULL ? 0 : pSerial->HEX_Data.len;
}

uint8_t Serial_GetNewPackageFlag_HEX(Serial_Typedef *pSerial)
{
    if (pSerial == NULL || !pSerial->HEX_Data.frame_valid) return 0;
    pSerial->HEX_Data.frame_valid = false;
    return 1;
}

int Serial_GetError_HEX(Serial_Typedef *pSerial)
{
    return pSerial == NULL ? Serial_Err_HEX_Head : pSerial->HEX_Data.err;
}

uint8_t Serial_GetNewPackageFlag_ABC(Serial_Typedef *pSerial)
{
    if (pSerial == NULL || !pSerial->ABC_Data.Serial_New_Package_Flag) return 0;
    pSerial->ABC_Data.Serial_New_Package_Flag = false;
    return 1;
}

int Serial_GetError_ABC(Serial_Typedef *pSerial)
{
    return pSerial == NULL ? Serial_Err_ABC_Head : pSerial->ABC_Data.err;
}

bool Serial_SetFloatData(Serial_Typedef *pSerial, char *KeyWord, char *cmd, float *Data)
{
    if (pSerial == NULL || KeyWord == NULL || cmd == NULL || Data == NULL ||
        !Serial_Check_Str(pSerial, KeyWord)) return false;
    return sscanf(pSerial->ABC_Data.Serial_New_Package_ABC, cmd, Data) == 1;
}

bool Serial_SetIntData(Serial_Typedef *pSerial, char *KeyWord, char *cmd, int *Data)
{
    if (pSerial == NULL || KeyWord == NULL || cmd == NULL || Data == NULL ||
        !Serial_Check_Str(pSerial, KeyWord)) return false;
    return sscanf(pSerial->ABC_Data.Serial_New_Package_ABC, cmd, Data) == 1;
}

bool Serial_Check_Str(Serial_Typedef *pSerial, char *KeyWord)
{
    return pSerial != NULL && KeyWord != NULL &&
           strstr(pSerial->ABC_Data.Serial_New_Package_ABC, KeyWord) != NULL;
}

bool Serial_CheckCmd(Serial_Typedef *pSerial, char *cmd)
{
    return pSerial != NULL && cmd != NULL &&
           strcmp(pSerial->ABC_Data.Serial_New_Package_ABC, cmd) == 0;
}

void Serial_send_string(Serial_Typedef *pSerial, char *str)
{
    if (str != NULL) Serial_SendBytes(pSerial, (uint8_t *)str, (uint16_t)strlen(str));
}

void Serial_SendBytes(Serial_Typedef *pSerial, uint8_t *buf, uint16_t len)
{
    if (pSerial == NULL || pSerial->huart == NULL || buf == NULL || len == 0) return;
    HAL_UART_Transmit(pSerial->huart, buf, len, 1000);
}

void Serial_Send_HEX_Package(Serial_Typedef *pSerial, uint16_t *data, uint8_t count)
{
    static uint8_t txBuf[3 + 255 * 3 + 2];
    uint16_t pos = 0;
    uint8_t i;
    if (pSerial == NULL || (count != 0 && data == NULL)) return;
    txBuf[pos++] = Serial_Agreement_HEX.head1;
    txBuf[pos++] = Serial_Agreement_HEX.head2;
    txBuf[pos++] = count;
    for (i = 0; i < count; ++i) {
        uint8_t high = (uint8_t)(data[i] >> 8);
        uint8_t low = (uint8_t)data[i];
        txBuf[pos++] = high;
        txBuf[pos++] = low;
        txBuf[pos++] = high ^ low;
    }
    txBuf[pos++] = Serial_Agreement_HEX.end1;
    txBuf[pos++] = Serial_Agreement_HEX.end2;
    Serial_SendBytes(pSerial, txBuf, pos);
}

void Serial_Clear_ABC(Serial_Typedef *pSerial)
{
    if (pSerial == NULL) return;
    memset(pSerial->ABC_Data.Serial_New_Package_ABC, 0, Serial_ABC_BUF_SIZE);
    pSerial->ABC_Data.Serial_New_Package_Flag = false;
    pSerial->ABC_Data.err = Serial_Err_None;
}

void Serial_PrintDebug(Serial_Typedef *pSerial)
{
    if (pSerial == NULL) return;
    Serial_printf(pSerial, "=== Serial Debug ===\r\n  Instance: 0x%08lX\r\n==================\r\n",
                  (unsigned long)pSerial->Instance);
}

static void Serial_ParseABC(Serial_Typedef *pSerial, uint16_t size)
{
    uint16_t payload;
    if (size < 3 || pSerial->rxBuf[size - 2] != Serial_Agreement_ABC.end1 ||
        pSerial->rxBuf[size - 1] != Serial_Agreement_ABC.end2) {
        pSerial->ABC_Data.err = Serial_Err_ABC_Tail;
        return;
    }
    payload = size - 3;
    if (payload >= Serial_ABC_BUF_SIZE) {
        pSerial->ABC_Data.err = Serial_Err_ABC_Tail;
        return;
    }
    memcpy(pSerial->ABC_Data.Serial_New_Package_ABC, pSerial->rxBuf + 1, payload);
    pSerial->ABC_Data.Serial_New_Package_ABC[payload] = '\0';
    pSerial->ABC_Data.err = Serial_Err_None;
    pSerial->ABC_Data.Serial_New_Package_Flag = true;
}

static void Serial_ParseHEX(Serial_Typedef *pSerial, uint16_t size)
{
    uint8_t count;
    uint16_t needed;
    uint16_t i;
    if (size < 5 || pSerial->rxBuf[1] != Serial_Agreement_HEX.head2) {
        pSerial->HEX_Data.err = Serial_Err_HEX_Head;
        return;
    }
    count = pSerial->rxBuf[2];
    needed = (uint16_t)(5U + 3U * count);
    if (size != needed || pSerial->rxBuf[needed - 2] != Serial_Agreement_HEX.end1 ||
        pSerial->rxBuf[needed - 1] != Serial_Agreement_HEX.end2) {
        pSerial->HEX_Data.err = Serial_Err_HEX_Tail;
        return;
    }
    for (i = 0; i < count; ++i) {
        uint8_t high = pSerial->rxBuf[3U + 3U * i];
        uint8_t low = pSerial->rxBuf[4U + 3U * i];
        uint8_t check = pSerial->rxBuf[5U + 3U * i];
        if ((uint8_t)(high ^ low) == check)
            pSerial->HEX_Data.data[i] = (int16_t)((high << 8) | low);
    }
    pSerial->HEX_Data.len = count;
    pSerial->HEX_Data.err = Serial_Err_None;
    pSerial->HEX_Data.frame_valid = true;
}

void Serial_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    Serial_Typedef *pSerial = NULL;
    if (huart == &huart1) pSerial = &Serial1;
    if (huart == &huart2) pSerial = &Serial2;
    if (pSerial == NULL) return;

    pSerial->rxLen = size;
    if (size >= 1 && size <= Serial_RX_BUF_SIZE) {
        if (pSerial->rxBuf[0] == Serial_Agreement_ABC.head) {
            Serial_ParseABC(pSerial, size);
        } else if (pSerial->rxBuf[0] == Serial_Agreement_HEX.head1) {
            Serial_ParseHEX(pSerial, size);
        } else {
            pSerial->ABC_Data.err = Serial_Err_ABC_Head;
        }
    }
    Serial_StartReceive(pSerial);
}

void Serial_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* HAL keeps RX running for recoverable noise errors. */
    if (huart->RxState != HAL_UART_STATE_READY) return;
    if (huart == &huart1) Serial_StartReceive(&Serial1);
    if (huart == &huart2) Serial_StartReceive(&Serial2);
}
