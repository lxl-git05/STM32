#include "bsp_at24c02.h"

/**
 * @brief AT24C02 初始化（硬件I2C2已在MX_I2C2_Init中初始化，此处为空操作）
 */
void AT24C02_Init(void)
{
    /* I2C2硬件已在main.c的MX_I2C2_Init()中初始化 */
}

/**
 * @brief 向AT24C02指定地址写入一个字节
 * @param WordAddress AT24C02内存地址(0~255)
 * @param Data 要写入的数据
 * @return 0=失败, 1=成功
 */
uint8_t AT24C02_WriteByte(uint8_t WordAddress, uint8_t Data)
{
    HAL_StatusTypeDef status;

    /* 写入数据，AT24C02内存地址为1字节 */
    status = HAL_I2C_Mem_Write(&hi2c2, AT24C02_ADDR_WRITE, WordAddress,
                                I2C_MEMADD_SIZE_8BIT, &Data, 1, 100);

    if (status != HAL_OK)
        return 0;

    /* 等待内部写周期完成（AT24C02数据手册典型5ms） */
    HAL_Delay(5);

    return 1;
}

/**
 * @brief 从AT24C02指定地址读取一个字节
 * @param WordAddress AT24C02内存地址(0~255)
 * @return 读取到的数据
 */
uint8_t AT24C02_ReadByte(uint8_t WordAddress)
{
    uint8_t Data = 0;

    HAL_I2C_Mem_Read(&hi2c2, AT24C02_ADDR_READ, WordAddress,
                     I2C_MEMADD_SIZE_8BIT, &Data, 1, 100);

    return Data;
}
