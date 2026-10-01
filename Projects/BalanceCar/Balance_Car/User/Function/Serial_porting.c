
#include "Serial_porting.h"
#include "usart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


// 全局串口对象
Serial_Typedef Serial1;


// ==========================================
// 1. 内部辅助函数
// ==========================================

// 计算TX缓冲区的剩余空间
// 调用前必须进入临界区
static uint16_t Serial_TxFree(
    Serial_Typedef *pSerial
)
{
    uint16_t used;

    if (pSerial->txHead >= pSerial->txTail)
    {
        used = pSerial->txHead
             - pSerial->txTail;
    }
    else
    {
        used = Serial_TX_BUF_SIZE
             - pSerial->txTail
             + pSerial->txHead;
    }

    // 保留1字节，用于区分空和满
    return Serial_TX_BUF_SIZE - 1U - used;
}


// 向环形缓冲区写入1字节
// 调用前必须确保有足够空间，并进入临界区
static void Serial_TxWriteByte(
    Serial_Typedef *pSerial,
    uint8_t data
)
{
    pSerial->txBuf[pSerial->txHead] = data;

    pSerial->txHead++;

    if (pSerial->txHead >= Serial_TX_BUF_SIZE)
    {
        pSerial->txHead = 0;
    }
}


// 启动一段DMA发送
// 调用前必须进入临界区
static HAL_StatusTypeDef Serial_TxKick(
    Serial_Typedef *pSerial
)
{
    uint16_t len;
    HAL_StatusTypeDef status;

    // DMA正在发送
    if (pSerial->txDmaLen != 0)
    {
        return HAL_OK;
    }

    // 缓冲区为空
    if (pSerial->txHead == pSerial->txTail)
    {
        return HAL_OK;
    }

    // 检查DMA句柄
    if (pSerial->huart->hdmatx == NULL)
    {
        return HAL_ERROR;
    }

    // 计算连续可发送的数据长度
    if (pSerial->txHead > pSerial->txTail)
    {
        len = pSerial->txHead
            - pSerial->txTail;
    }
    else
    {
        len = Serial_TX_BUF_SIZE
            - pSerial->txTail;
    }

    // 启动DMA
    status = HAL_UART_Transmit_DMA(
        pSerial->huart,
        &pSerial->txBuf[pSerial->txTail],
        len
    );

    if (status == HAL_OK)
    {
        // 记录当前DMA发送长度
        pSerial->txDmaLen = len;
    }

    return status;
}


// ==========================================
// 2. RX DMA启动
// ==========================================

static HAL_StatusTypeDef Serial_StartReceive(
    Serial_Typedef *pSerial
)
{
    if (pSerial == NULL ||
        pSerial->huart == NULL ||
        pSerial->huart->hdmarx == NULL)
    {
        return HAL_ERROR;
    }

    if (HAL_UARTEx_ReceiveToIdle_DMA(
        pSerial->huart,
        pSerial->rxBuf,
        Serial_RX_BUF_SIZE
    ) != HAL_OK)
    {
        pSerial->HEX_Data.err =
            Serial_Err_RX_Start;

        return HAL_ERROR;
    }

    // 关闭DMA半传输中断
    __HAL_DMA_DISABLE_IT(
        pSerial->huart->hdmarx,
        DMA_IT_HT
    );

    return HAL_OK;
}


// ==========================================
// 3. 串口初始化
// ==========================================

HAL_StatusTypeDef Serial_Init(void)
{
    // 此函数只在初始化阶段调用一次
    memset(&Serial1, 0, sizeof(Serial1));

    Serial1.Instance = USART1;
    Serial1.huart = &huart1;

    // 初始化协议
    Serial_Agreement_ABC_Init();
    Serial_Agreement_HEX_Init();

    // 启动接收DMA
    return Serial_StartReceive(&Serial1);
}


// ==========================================
// 4. TX DMA发送
// ==========================================

// 把数据写入环形缓冲区
HAL_StatusTypeDef Serial_SendBytes(
    Serial_Typedef *pSerial,
    const uint8_t *buf,
    uint16_t len
)
{
    uint16_t first;
    HAL_StatusTypeDef status;
    uint32_t primask;

    if (pSerial == NULL ||
        pSerial->huart == NULL ||
        pSerial->huart->hdmatx == NULL ||
        buf == NULL ||
        len == 0)
    {
        return HAL_ERROR;
    }

    // 保存中断状态并进入临界区
    primask = __get_PRIMASK();
    __disable_irq();

    // 检查剩余空间
    if (len > Serial_TxFree(pSerial))
    {
        pSerial->txOverflow++;

        __set_PRIMASK(primask);

        return HAL_BUSY;
    }

    // 计算第一段连续可写入长度
    first = Serial_TX_BUF_SIZE
          - pSerial->txHead;

    if (first > len)
    {
        first = len;
    }

    // 第一段复制到环形缓冲区末尾
    memcpy(
        &pSerial->txBuf[pSerial->txHead],
        buf,
        first
    );

    pSerial->txHead += first;

    if (pSerial->txHead >= Serial_TX_BUF_SIZE)
    {
        pSerial->txHead = 0;
    }

    // 如果跨越缓冲区末尾
    if (len > first)
    {
        memcpy(
            pSerial->txBuf,
            buf + first,
            len - first
        );

        pSerial->txHead = len - first;
    }

    // DMA空闲时，立即启动发送
    status = Serial_TxKick(pSerial);

    // 恢复原先的中断状态
    __set_PRIMASK(primask);

    return status;
}


// 在必要时重新尝试启动DMA
HAL_StatusTypeDef Serial_TxService(
    Serial_Typedef *pSerial
)
{
    HAL_StatusTypeDef status;
    uint32_t primask;

    if (pSerial == NULL ||
        pSerial->huart == NULL)
    {
        return HAL_ERROR;
    }

    primask = __get_PRIMASK();
    __disable_irq();

    status = Serial_TxKick(pSerial);

    __set_PRIMASK(primask);

    return status;
}


// DMA发送完成回调
void Serial_TxCpltCallback(
    UART_HandleTypeDef *huart
)
{
    Serial_Typedef *pSerial;
    uint32_t primask;

    if (huart != Serial1.huart)
    {
        return;
    }

    pSerial = &Serial1;

    primask = __get_PRIMASK();
    __disable_irq();

    // 更新已发送数据的位置
    if (pSerial->txDmaLen != 0)
    {
        pSerial->txTail += pSerial->txDmaLen;

        if (pSerial->txTail >= Serial_TX_BUF_SIZE)
        {
            pSerial->txTail -= Serial_TX_BUF_SIZE;
        }

        // 标记当前DMA传输已完成
        pSerial->txDmaLen = 0;
    }

    // 继续发送缓冲区里的剩余数据
    (void)Serial_TxKick(pSerial);

    __set_PRIMASK(primask);
}


// ==========================================
// 5. printf封装
// ==========================================

void Serial_printf(
    Serial_Typedef *pSerial,
    const char *fmt,
    ...
)
{
    char buffer[256];
    va_list args;
    int len;

    if (pSerial == NULL || fmt == NULL)
    {
        return;
    }

    va_start(args, fmt);

    len = vsnprintf(
        buffer,
        sizeof(buffer),
        fmt,
        args
    );

    va_end(args);

    if (len <= 0)
    {
        return;
    }

    // 防止格式化字符串超出buffer
    if (len >= (int)sizeof(buffer))
    {
        len = sizeof(buffer) - 1;
    }

    // 数据复制到TX环形缓冲区
    (void)Serial_SendBytes(
        pSerial,
        (const uint8_t *)buffer,
        (uint16_t)len
    );
}


// 发送字符串
void Serial_send_string(
    Serial_Typedef *pSerial,
    char *str
)
{
    if (str == NULL)
    {
        return;
    }

    (void)Serial_SendBytes(
        pSerial,
        (const uint8_t *)str,
        (uint16_t)strlen(str)
    );
}


// ==========================================
// 6. HEX协议发送
// ==========================================

void Serial_Send_HEX_Package(
    Serial_Typedef *pSerial,
    uint16_t *data,
    uint8_t count
)
{
    uint16_t needed;
    uint16_t i;
    uint32_t primask;

    if (pSerial == NULL ||
        pSerial->huart == NULL ||
        pSerial->huart->hdmatx == NULL ||
        (count != 0 && data == NULL))
    {
        return;
    }

    // 2字节帧头 + 1字节数量
    // 每个数据占3字节 + 2字节帧尾
    needed = (uint16_t)(5U + 3U * count);

    primask = __get_PRIMASK();
    __disable_irq();

    // 保证一整帧都有足够空间
    if (needed > Serial_TxFree(pSerial))
    {
        pSerial->txOverflow++;

        __set_PRIMASK(primask);

        return;
    }

    // 帧头
    Serial_TxWriteByte(
        pSerial,
        Serial_Agreement_HEX.head1
    );

    Serial_TxWriteByte(
        pSerial,
        Serial_Agreement_HEX.head2
    );

    // 数据数量
    Serial_TxWriteByte(pSerial, count);

    // 数据区域
    for (i = 0; i < count; i++)
    {
        uint8_t high;
        uint8_t low;
        uint8_t check;

        high = (uint8_t)(data[i] >> 8);
        low = (uint8_t)data[i];

        check = high ^ low;

        Serial_TxWriteByte(pSerial, high);
        Serial_TxWriteByte(pSerial, low);
        Serial_TxWriteByte(pSerial, check);
    }

    // 帧尾
    Serial_TxWriteByte(
        pSerial,
        Serial_Agreement_HEX.end1
    );

    Serial_TxWriteByte(
        pSerial,
        Serial_Agreement_HEX.end2
    );

    // 启动DMA
    (void)Serial_TxKick(pSerial);

    __set_PRIMASK(primask);
}


// ==========================================
// 7. HEX协议数据读取
// ==========================================

int16_t Serial_GetHexData(
    Serial_Typedef *pSerial,
    uint8_t index
)
{
    if (pSerial == NULL ||
        index >= pSerial->HEX_Data.len)
    {
        return 0;
    }

    return pSerial->HEX_Data.data[index];
}


uint8_t Serial_GetHexLen(
    Serial_Typedef *pSerial
)
{
    return pSerial == NULL
        ? 0
        : pSerial->HEX_Data.len;
}


uint8_t Serial_GetNewPackageFlag_HEX(
    Serial_Typedef *pSerial
)
{
    if (pSerial == NULL ||
        !pSerial->HEX_Data.frame_valid)
    {
        return 0;
    }

    pSerial->HEX_Data.frame_valid = false;

    return 1;
}


int Serial_GetError_HEX(
    Serial_Typedef *pSerial
)
{
    return pSerial == NULL
        ? Serial_Err_HEX_Head
        : pSerial->HEX_Data.err;
}


// ==========================================
// 8. ABC协议数据读取
// ==========================================

uint8_t Serial_GetNewPackageFlag_ABC(
    Serial_Typedef *pSerial
)
{
    if (pSerial == NULL ||
        !pSerial->ABC_Data.Serial_New_Package_Flag)
    {
        return 0;
    }

    pSerial->ABC_Data.Serial_New_Package_Flag =
        false;

    return 1;
}


int Serial_GetError_ABC(
    Serial_Typedef *pSerial
)
{
    return pSerial == NULL
        ? Serial_Err_ABC_Head
        : pSerial->ABC_Data.err;
}


bool Serial_SetFloatData(
    Serial_Typedef *pSerial,
    char *KeyWord,
    char *cmd,
    float *Data
)
{
    if (pSerial == NULL ||
        KeyWord == NULL ||
        cmd == NULL ||
        Data == NULL ||
        !Serial_Check_Str(pSerial, KeyWord))
    {
        return false;
    }

    return sscanf(
        pSerial->ABC_Data.Serial_New_Package_ABC,
        cmd,
        Data
    ) == 1;
}


bool Serial_SetIntData(
    Serial_Typedef *pSerial,
    char *KeyWord,
    char *cmd,
    int *Data
)
{
    if (pSerial == NULL ||
        KeyWord == NULL ||
        cmd == NULL ||
        Data == NULL ||
        !Serial_Check_Str(pSerial, KeyWord))
    {
        return false;
    }

    return sscanf(
        pSerial->ABC_Data.Serial_New_Package_ABC,
        cmd,
        Data
    ) == 1;
}


bool Serial_Check_Str(
    Serial_Typedef *pSerial,
    char *KeyWord
)
{
    return pSerial != NULL &&
           KeyWord != NULL &&
           strstr(
               pSerial->ABC_Data.Serial_New_Package_ABC,
               KeyWord
           ) != NULL;
}


bool Serial_CheckCmd(
    Serial_Typedef *pSerial,
    char *cmd
)
{
    return pSerial != NULL &&
           cmd != NULL &&
           strcmp(
               pSerial->ABC_Data.Serial_New_Package_ABC,
               cmd
           ) == 0;
}


// 清除ABC接收数据
void Serial_Clear_ABC(
    Serial_Typedef *pSerial
)
{
    if (pSerial == NULL)
    {
        return;
    }

    memset(
        pSerial->ABC_Data.Serial_New_Package_ABC,
        0,
        Serial_ABC_BUF_SIZE
    );

    pSerial->ABC_Data.Serial_New_Package_Flag =
        false;

    pSerial->ABC_Data.err = Serial_Err_None;
}


// ==========================================
// 9. 调试
// ==========================================

void Serial_PrintDebug(
    Serial_Typedef *pSerial
)
{
    if (pSerial == NULL)
    {
        return;
    }

    Serial_printf(
        pSerial,
        "=== Serial Debug ===\r\n"
        "Instance: 0x%08lX\r\n"
        "TX Overflow: %lu\r\n"
        "====================\r\n",
        (unsigned long)(uintptr_t)pSerial->Instance,
        (unsigned long)pSerial->txOverflow
    );
}


// ==========================================
// 10. ABC协议解析
// ==========================================

static void Serial_ParseABC(
    Serial_Typedef *pSerial,
    uint16_t size
)
{
    uint16_t payload;

    if (size < 3 ||
        pSerial->rxBuf[size - 2] !=
            Serial_Agreement_ABC.end1 ||
        pSerial->rxBuf[size - 1] !=
            Serial_Agreement_ABC.end2)
    {
        pSerial->ABC_Data.err =
            Serial_Err_ABC_Tail;

        return;
    }

    // 去掉1字节帧头和2字节帧尾
    payload = size - 3;

    if (payload >= Serial_ABC_BUF_SIZE)
    {
        pSerial->ABC_Data.err =
            Serial_Err_ABC_Tail;

        return;
    }

    memcpy(
        pSerial->ABC_Data.Serial_New_Package_ABC,
        pSerial->rxBuf + 1,
        payload
    );

    pSerial->ABC_Data.Serial_New_Package_ABC[payload] =
        '\0';

    pSerial->ABC_Data.err = Serial_Err_None;

    pSerial->ABC_Data.Serial_New_Package_Flag =
        true;
}


// ==========================================
// 11. HEX协议解析
// ==========================================

static void Serial_ParseHEX(
    Serial_Typedef *pSerial,
    uint16_t size
)
{
    uint8_t count;
    uint16_t needed;
    uint16_t i;

    if (size < 5 ||
        pSerial->rxBuf[1] !=
            Serial_Agreement_HEX.head2)
    {
        pSerial->HEX_Data.err =
            Serial_Err_HEX_Head;

        return;
    }

    count = pSerial->rxBuf[2];

    needed = (uint16_t)(5U + 3U * count);

    // 检查长度和帧尾
    if (size != needed ||
        pSerial->rxBuf[needed - 2] !=
            Serial_Agreement_HEX.end1 ||
        pSerial->rxBuf[needed - 1] !=
            Serial_Agreement_HEX.end2)
    {
        pSerial->HEX_Data.err =
            Serial_Err_HEX_Tail;

        return;
    }

    // 检查数据数组容量，防止越界
    if (count > sizeof(pSerial->HEX_Data.data) /
                sizeof(pSerial->HEX_Data.data[0]))
    {
        pSerial->HEX_Data.err =
            Serial_Err_HEX_Tail;

        return;
    }

    // 校验所有数据
    for (i = 0; i < count; i++)
    {
        uint8_t high;
        uint8_t low;
        uint8_t check;

        high = pSerial->rxBuf[3U + 3U * i];
        low = pSerial->rxBuf[4U + 3U * i];
        check = pSerial->rxBuf[5U + 3U * i];

        if ((uint8_t)(high ^ low) != check)
        {
            pSerial->HEX_Data.err =
                Serial_Err_HEX_Checksum;

            return;
        }
    }

    // 保存解析结果
    for (i = 0; i < count; i++)
    {
        uint8_t high;
        uint8_t low;

        high = pSerial->rxBuf[3U + 3U * i];
        low = pSerial->rxBuf[4U + 3U * i];

        pSerial->HEX_Data.data[i] =
            (int16_t)((high << 8) | low);
    }

    pSerial->HEX_Data.len = count;
    pSerial->HEX_Data.err = Serial_Err_None;
    pSerial->HEX_Data.frame_valid = true;
}


// ==========================================
// 12. RX事件回调
// ==========================================

void Serial_RxEventCallback(
    UART_HandleTypeDef *huart,
    uint16_t size
)
{
    Serial_Typedef *pSerial = NULL;

    if (huart == &huart1)
    {
        pSerial = &Serial1;
    }

    if (pSerial == NULL)
    {
        return;
    }

    pSerial->rxLen = size;

    if (size >= 1 && size <= Serial_RX_BUF_SIZE)
    {
        if (pSerial->rxBuf[0] ==
            Serial_Agreement_ABC.head)
        {
            Serial_ParseABC(pSerial, size);
        }
        else if (pSerial->rxBuf[0] ==
                 Serial_Agreement_HEX.head1)
        {
            Serial_ParseHEX(pSerial, size);
        }
        else
        {
            pSerial->ABC_Data.err =
                Serial_Err_ABC_Head;
        }
    }

    // 重新启动RX DMA
    (void)Serial_StartReceive(pSerial);
}


// ==========================================
// 13. UART错误回调
// ==========================================

void Serial_ErrorCallback(
    UART_HandleTypeDef *huart
)
{
    if (huart != &huart1)
    {
        return;
    }

    // 如果接收状态已经恢复为READY
    // 尝试重新启动DMA接收
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        (void)Serial_StartReceive(&Serial1);
    }
}
