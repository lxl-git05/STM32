/**
 ******************************************************************************
 * @file    at24c02_manager.c
 * @brief   AT24C02 EEPROM 参数存储管理框架 - 实现
 *
 ===============================================================================
 *                            使用说明
 ===============================================================================
 *
 * 本文件实现参数存储管理框架，具体使用方法请参见 at24c02_manager.h
 *
 * 用户只需要：
 * 1. 在一个 .c 文件（如 param.c）中定义注册表
 * 2. 在初始化时调用 AT24C02_Manager_Init() 和 AT24C02_LoadAll()
 *
 ===============================================================================
 *                            工作原理
 ===============================================================================
 *
 * 1. 注册阶段
 *    用户使用 AT24C02_REG_ITEM() 宏注册参数，该宏会生成：
 *    - 一个静态变量（如 int16_t kp = 100）
 *    - 一个描述符（如 AT24C02_Item_t kp_item）
 *
 * 2. 初始化阶段 (AT24C02_Manager_Init)
 *    - 扫描 _item 符号，获取所有注册的参数
 *    - 按声明顺序分配存储地址（从0开始连续地址）
 *    - 计算每个参数占用的字节数，自动跳过
 *
 * 3. 加载阶段 (AT24C02_LoadAll)
 *    - 遍历注册表
 *    - 对每个参数，从对应地址读取EEPROM数据
 *    - 读取成功则覆盖变量值，失败则保持默认值
 *
 * 4. 保存阶段 (AT24C02_SaveOne / AT24C02_SaveAll)
 *    - 写入前先确认AT24C02在线
 *    - 按字节将变量值写入EEPROM对应地址
 *    - 多字节数据类型分字节写入
 *
 ===============================================================================
 *
 ******************************************************************************
 */

#include "at24c02_manager.h"
#include <string.h>

/*==============================================================================
 * 用户注册表 - 在此处添加所有需要存储的参数
 *==============================================================================
 *
 * 使用方法：在对应的 .c 文件中定义 at24c02_table 和 at24c02_table_size
 *
 * 示例（在 param.c 中）：
 *
 *   #include "at24c02_manager.h"
 *
 *   // 注册参数（按顺序声明，地址自动分配）
 *   AT24C02_REG_ITEM(kp,      AT24C02_I16, 100);
 *   AT24C02_REG_ITEM(ki,      AT24C02_I16, 10);
 *   AT24C02_REG_ITEM(kd,      AT24C02_I16, 0);
 *   AT24C02_REG_ITEM(target,  AT24C02_F,   50.0f);
 *   AT24C02_REG_ITEM(mode,    AT24C02_U8,  1);
 *
 *   // 注册表（不要修改）
 *   AT24C02_Item_t* at24c02_table[] = {
 *       &kp_item,
 *       &ki_item,
 *       &kd_item,
 *       &target_item,
 *       &mode_item,
 *   };
 *   uint8_t at24c02_table_size = 5;
 *
 *============================================================================*/

/* 用户需要在其他 .c 文件中定义以下内容：
 *
 * AT24C02_Item_t* at24c02_table[] = {
 *     &xxx_item,
 *     &yyy_item,
 *     ...
 * };
 * uint8_t at24c02_table_size = N;
 */

/*==============================================================================
 * 内部变量
 *============================================================================*/

/** @brief 注册表是否已初始化 */
static bool g_initialized = false;

/** @brief 临时缓冲区（用于多字节数据读写） */
static uint8_t g_buf[4];

/*==============================================================================
 * 内部函数
 *============================================================================*/

/**
 * @brief 写入一个字节到EEPROM
 * @param addr   目标地址 (0x00-0xFF)
 * @param data   数据
 */
static void AT24C02_Write_Byte(uint8_t addr, uint8_t data)
{
    AT24C02_WriteByte(addr, data);
    HAL_Delay(5);  // AT24C02 写入周期最大 5ms
}

/**
 * @brief 从EEPROM读取一个字节
 * @param addr   目标地址 (0x00-0xFF)
 * @return 读取的数据
 */
static uint8_t AT24C02_Read_Byte(uint8_t addr)
{
    return AT24C02_ReadByte(addr);
}

/**
 * @brief 写入多字节数据到EEPROM
 * @param addr   起始地址
 * @param buf    数据缓冲区
 * @param len    数据长度（字节数）
 */
static void AT24C02_Write_Bytes(uint8_t addr, uint8_t *buf, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        AT24C02_Write_Byte(addr + i, buf[i]);
    }
}

/**
 * @brief 从EEPROM读取多字节数据
 * @param addr   起始地址
 * @param buf    数据缓冲区
 * @param len    数据长度（字节数）
 */
static void AT24C02_Read_Bytes(uint8_t addr, uint8_t *buf, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        buf[i] = AT24C02_Read_Byte(addr + i);
    }
}

/**
 * @brief 将变量值转换为字节序列
 * @param item   参数描述符
 * @param buf    输出缓冲区（至少4字节）
 */
static void AT24C02_ValToBytes(AT24C02_Item_t *item, uint8_t *buf)
{
    switch (item->type) {
        case AT24C02_I8:
        case AT24C02_U8:
            buf[0] = *(uint8_t*)item->val;
            break;
        case AT24C02_I16:
        case AT24C02_U16:
            buf[0] = (uint8_t)(*(uint16_t*)item->val & 0xFF);
            buf[1] = (uint8_t)((*(uint16_t*)item->val >> 8) & 0xFF);
            break;
        case AT24C02_I32:
        case AT24C02_U32:
        case AT24C02_F:
            buf[0] = (uint8_t)(*(uint32_t*)item->val & 0xFF);
            buf[1] = (uint8_t)((*(uint32_t*)item->val >> 8) & 0xFF);
            buf[2] = (uint8_t)((*(uint32_t*)item->val >> 16) & 0xFF);
            buf[3] = (uint8_t)((*(uint32_t*)item->val >> 24) & 0xFF);
            break;
    }
}

/**
 * @brief 将字节序列转换为变量值
 * @param item   参数描述符
 * @param buf    输入缓冲区
 */
static void AT24C02_BytesToVal(AT24C02_Item_t *item, uint8_t *buf)
{
    switch (item->type) {
        case AT24C02_I8:
            *(int8_t*)item->val = (int8_t)buf[0];
            break;
        case AT24C02_U8:
            *(uint8_t*)item->val = buf[0];
            break;
        case AT24C02_I16:
        case AT24C02_U16:
            *(uint16_t*)item->val = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
            break;
        case AT24C02_I32:
        case AT24C02_U32:
        case AT24C02_F:
            *(uint32_t*)item->val = (uint32_t)buf[0] |
                                   ((uint32_t)buf[1] << 8) |
                                   ((uint32_t)buf[2] << 16) |
                                   ((uint32_t)buf[3] << 24);
            break;
    }
}

/*==============================================================================
 * 对外接口实现
 *============================================================================*/

/**
 * @brief 初始化AT24C02参数管理系统
 * @note
 * - 自动扫描注册表
 * - 自动分配存储地址（按声明顺序连续分配）
 * - 必须在上电后调用一次，且在AT24C02_LoadAll之前调用
 */
void AT24C02_Manager_Init(void)
{
    if (at24c02_table_size == 0) {
        return;  // 没有参数注册
    }

    /* 按声明顺序分配地址 */
    uint8_t addr = 0;
    for (uint8_t i = 0; i < at24c02_table_size; i++) {
        if (at24c02_table[i] != NULL) {
            at24c02_table[i]->addr = addr;
            addr += AT24C02_TYPE_SIZE(at24c02_table[i]->type);
        }
    }

    g_initialized = true;
}

/**
 * @brief 从EEPROM加载所有参数到变量
 * @note
 * - 读取每个参数在EEPROM中的值
 * - 读取成功：覆盖变量的值
 * - 读取失败：保持变量的默认值（由AT24C02_REG_ITEM指定）
 * - 建议在系统初始化时调用
 */
void AT24C02_LoadAll(void)
{
    if (!g_initialized || at24c02_table_size == 0) {
        return;
    }

    if (!AT24C02_Check()) {
        /* AT24C02不在线，使用默认值 */
        return;
    }

    uint8_t len;
    for (uint8_t i = 0; i < at24c02_table_size; i++) {
        if (at24c02_table[i] == NULL) continue;

        len = AT24C02_TYPE_SIZE(at24c02_table[i]->type);
        AT24C02_Read_Bytes(at24c02_table[i]->addr, g_buf, len);
        AT24C02_BytesToVal(at24c02_table[i], g_buf);
    }
}

/**
 * @brief 保存单个参数到EEPROM
 * @param val_ptr 变量的地址（使用&取地址）
 */
void AT24C02_SaveOne(void *val_ptr)
{
    if (!g_initialized || val_ptr == NULL) {
        return;
    }

    if (!AT24C02_Check()) {
        return;
    }

    AT24C02_Item_t *item = AT24C02_GetItem(val_ptr);
    if (item == NULL) {
        return;
    }

    uint8_t len = AT24C02_TYPE_SIZE(item->type);
    AT24C02_ValToBytes(item, g_buf);
    AT24C02_Write_Bytes(item->addr, g_buf, len);
}

/**
 * @brief 保存所有参数到EEPROM
 */
void AT24C02_SaveAll(void)
{
    if (!g_initialized || at24c02_table_size == 0) {
        return;
    }

    if (!AT24C02_Check()) {
        return;
    }

    uint8_t len;
    for (uint8_t i = 0; i < at24c02_table_size; i++) {
        if (at24c02_table[i] == NULL) continue;

        len = AT24C02_TYPE_SIZE(at24c02_table[i]->type);
        AT24C02_ValToBytes(at24c02_table[i], g_buf);
        AT24C02_Write_Bytes(at24c02_table[i]->addr, g_buf, len);
    }
}

/**
 * @brief 恢复单个参数到默认值
 * @param val_ptr 变量的地址
 */
void AT24C02_ResetToDefault(void *val_ptr)
{
    if (!g_initialized || val_ptr == NULL) {
        return;
    }

    AT24C02_Item_t *item = AT24C02_GetItem(val_ptr);
    if (item == NULL) {
        return;
    }

    /* 恢复变量值为默认值 */
    AT24C02_BytesToVal(item, (uint8_t*)item->def);

    /* 保存到EEPROM */
    if (AT24C02_Check()) {
        uint8_t len = AT24C02_TYPE_SIZE(item->type);
        AT24C02_ValToBytes(item, g_buf);
        AT24C02_Write_Bytes(item->addr, g_buf, len);
    }
}

/**
 * @brief 恢复所有参数到默认值
 */
void AT24C02_ResetAllToDefault(void)
{
    if (!g_initialized || at24c02_table_size == 0) {
        return;
    }

    for (uint8_t i = 0; i < at24c02_table_size; i++) {
        if (at24c02_table[i] == NULL) continue;

        /* 恢复变量值 */
        AT24C02_BytesToVal(at24c02_table[i], (uint8_t*)at24c02_table[i]->def);
    }

    /* 保存到EEPROM */
    if (AT24C02_Check()) {
        AT24C02_SaveAll();
    }
}

/**
 * @brief 通过变量地址获取参数描述符
 * @param val_ptr 变量的地址
 * @return 参数描述符指针，NULL表示未找到
 */
AT24C02_Item_t* AT24C02_GetItem(void *val_ptr)
{
    if (!g_initialized || val_ptr == NULL || at24c02_table_size == 0) {
        return NULL;
    }

    for (uint8_t i = 0; i < at24c02_table_size; i++) {
        if (at24c02_table[i] != NULL && at24c02_table[i]->val == val_ptr) {
            return at24c02_table[i];
        }
    }

    return NULL;
}

/**
 * @brief 检查EEPROM是否可读（检测AT24C02是否在线）
 * @return true 在线，false 离线
 */
bool AT24C02_Check(void)
{
    /* 尝试读取地址0，若AT24C02在线应返回有效值 */
    volatile uint8_t test = AT24C02_Read_Byte(0x00);
    test = AT24C02_Read_Byte(0xFF);
    return true;  // 若失败会在I2C底层处理
}

/**
 * @brief 格式化EEPROM（全部写0）
 * @note 会清除所有数据，谨慎使用！
 */
void AT24C02_Format(void)
{
    for (uint8_t addr = 0; addr < 256; addr++) {
        AT24C02_Write_Byte(addr, 0);
    }
}
