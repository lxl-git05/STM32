/**
 ******************************************************************************
 * @file    at24c02_manager.h
 * @brief   AT24C02 EEPROM 参数存储管理框架
 *
 * @details
 * 本框架用于将用户已存在的全局变量自动存储到 AT24C02 EEPROM，实现掉电保存
 * 主要用于电赛等场景下的参数脱机保存与调参
 *
 ===============================================================================
 *                            使用说明
 ===============================================================================
 *
 * 1. 头文件包含
 *    #include "at24c02_manager.h"
 *
 * 2. 定义全局变量（在你自己的文件中）
 *    int16_t kp = 100;
 *    int16_t ki = 10;
 *    float target = 50.0f;
 *    uint8_t mode = 1;
 *
 * 3. 注册参数（在 param.c 或 main.c 中）
 *    AT24C02_REG(kp,     AT24C02_I16);  // 注册已存在的全局变量
 *    AT24C02_REG(ki,     AT24C02_I16);
 *    AT24C02_REG(target, AT24C02_F);
 *    AT24C02_REG(mode,   AT24C02_U8);
 *
 * 4. 定义注册表
 *    AT24C02_Item_t* at24c02_table[] = {
 *        &kp_item,
 *        &ki_item,
 *        &target_item,
 *        &mode_item,
 *    };
 *    uint8_t at24c02_table_size = 4;
 *
 * 5. 初始化（在MX_GPIO_Init之后调用一次）
 *    AT24C02_Manager_Init();  // 自动分配地址
 *    AT24C02_LoadAll();      // 从EEPROM加载参数，失败使用默认值
 *
 * 6. 使用变量（像普通变量一样使用即可）
 *    kp = 150;                 // 直接赋值
 *    OLED_ShowNum(0, 0, kp, 5); // 显示
 *
 * 7. 保存参数（调参时调用）
 *    AT24C02_SaveOne(&kp);     // 保存单个参数到EEPROM
 *    AT24C02_SaveAll();         // 保存所有参数到EEPROM
 *
 * 8. 恢复默认值
 *    AT24C02_ResetToDefault(&target); // 单个恢复
 *    AT24C02_ResetAllToDefault();     // 全部恢复
 *
 ===============================================================================
 *                            支持的数据类型
 ===============================================================================
 * AT24C02_I8    -> int8_t     (1字节, -128 ~ 127)
 * AT24C02_U8    -> uint8_t    (1字节, 0 ~ 255)
 * AT24C02_I16   -> int16_t    (2字节, -32768 ~ 32767)
 * AT24C02_U16   -> uint16_t   (2字节, 0 ~ 65535)
 * AT24C02_I32   -> int32_t    (4字节)
 * AT24C02_U32   -> uint32_t   (4字节)
 * AT24C02_F     -> float      (4字节)
 *
 ===============================================================================
 *                            工作原理
 ===============================================================================
 * 1. 注册阶段：AT24C02_REG 宏生成描述符 _name_item
 * 2. 初始化阶段：Manager_Init 按声明顺序自动分配存储地址
 * 3. 加载阶段：LoadAll 从 EEPROM 读取数据填充变量，失败则保持默认值
 * 4. 保存阶段：SaveOne/SaveAll 将变量值写入 EEPROM 对应地址
 *
 * 地址自动分配示例：
 *   AT24C02_REG(a, AT24C02_U8);   // 地址 0x00 (1字节)
 *   AT24C02_REG(b, AT24C02_F);    // 地址 0x01 (4字节)
 *   AT24C02_REG(c, AT24C02_I16);  // 地址 0x05 (2字节)
 *
 ******************************************************************************
 */

#ifndef __AT24C02_MANAGER_H__
#define __AT24C02_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_at24c02.h"
#include <stdint.h>
#include <stdbool.h>

/*==============================================================================
 * 数据类型定义
 *============================================================================*/

/**
 * @brief AT24C02 支持的数据类型枚举
 */
typedef enum {
    AT24C02_I8  = 1,    /**< int8_t   - 1字节 */
    AT24C02_U8  = 2,    /**< uint8_t  - 1字节 */
    AT24C02_I16 = 3,    /**< int16_t  - 2字节 */
    AT24C02_U16 = 4,    /**< uint16_t - 2字节 */
    AT24C02_I32 = 5,    /**< int32_t  - 4字节 */
    AT24C02_U32 = 6,    /**< uint32_t - 4字节 */
    AT24C02_F   = 7,    /**< float    - 4字节 */
} AT24C02_TypeDef;

/**
 * @brief 获取数据类型占用的字节数
 */
#define AT24C02_TYPE_SIZE(type) \
    ((type) == AT24C02_I8 || (type) == AT24C02_U8) ? 1 : \
    ((type) == AT24C02_I16 || (type) == AT24C02_U16) ? 2 : 4

/**
 * @brief 参数描述符结构体（内部使用）
 */
typedef struct {
    uint8_t type;       /**< 数据类型 @see AT24C02_TypeDef */
    uint8_t addr;       /**< 在AT24C02中的存储地址（自动分配） */
    void   *val;        /**< 指向变量的指针 */
    void   *def;         /**< 指向默认值（初始值） */
} AT24C02_Item_t;

/*==============================================================================
 * 宏定义 - 用户注册已存在的全局变量用
 *============================================================================*/

/**
 * @brief 注册一个已存在的全局变量到AT24C02管理表
 * @param _name  已存在的全局变量名
 * @param _type  数据类型 @see AT24C02_TypeDef
 *
 * @note
 * - 不会生成新变量，只生成描述符 _name_item
 * - 变量必须已经在其他地方定义
 * - 默认值取自变量的当前值
 * - 地址由 AT24C02_Manager_Init() 自动分配
 *
 * @code
 * // 假设已存在全局变量
 * int16_t kp;
 * float target;
 *
 * // 注册到管理表
 * AT24C02_REG(kp, AT24C02_I16);      // 描述符：kp_item
 * AT24C02_REG(target, AT24C02_F);    // 描述符：target_item
 *
 * // 注册表
 * AT24C02_Item_t* at24c02_table[] = {
 *     &kp_item,
 *     &target_item,
 * };
 * uint8_t at24c02_table_size = 2;
 * @endcode
 */
#define AT24C02_REG(_name, _type) \
    static AT24C02_Item_t _name##_item = { \
        .type = (_type), \
        .addr = 0, \
        .val  = &(_name), \
        .def  = &(_name), \
    }

/*==============================================================================
 * 外部声明 - 注册表（用户需要在对应的.c文件中定义一次）
 *============================================================================*/

/**
 * @brief 注册表数组（用户定义）
 * @note 用户需要在 .c 文件中定义并填充
 */
extern AT24C02_Item_t* at24c02_table[];

/**
 * @brief 注册表中参数项的数量
 */
extern uint8_t at24c02_table_size;

/*==============================================================================
 * 函数接口
 *============================================================================*/

/**
 * @brief 初始化AT24C02参数管理系统
 * @note
 * - 自动扫描注册表
 * - 自动分配存储地址（按声明顺序连续分配）
 * - 必须在上电后调用一次，且在 AT24C02_LoadAll 之前调用
 */
void AT24C02_Manager_Init(void);

/**
 * @brief 从EEPROM加载所有参数到变量
 * @note
 * - 读取每个参数在EEPROM中的值
 * - 读取成功：覆盖变量的值
 * - 读取失败：保持变量的当前值（作为默认值）
 * - 建议在系统初始化时调用
 */
void AT24C02_LoadAll(void);

/**
 * @brief 保存单个参数到EEPROM
 * @param val_ptr 变量的地址（使用&取地址）
 *
 * @code
 * kp = 150;                   // 修改变量
 * AT24C02_SaveOne(&kp);       // 保存到EEPROM
 * @endcode
 */
void AT24C02_SaveOne(void *val_ptr);

/**
 * @brief 保存所有参数到EEPROM
 * @note 遍历注册表，保存所有参数
 */
void AT24C02_SaveAll(void);

/**
 * @brief 恢复单个参数到默认值
 * @param val_ptr 变量的地址
 * @note 将变量恢复为初始化时的值，并将该值写入EEPROM
 */
void AT24C02_ResetToDefault(void *val_ptr);

/**
 * @brief 恢复所有参数到默认值
 * @note 将所有变量恢复为初始化时的值，并写入EEPROM
 */
void AT24C02_ResetAllToDefault(void);

/**
 * @brief 通过变量地址获取参数描述符
 * @param val_ptr 变量的地址
 * @return 参数描述符指针，NULL表示未找到
 */
AT24C02_Item_t* AT24C02_GetItem(void *val_ptr);

/**
 * @brief 检查EEPROM是否在线
 * @return true 在线，false 离线
 */
bool AT24C02_Check(void);

/**
 * @brief 格式化EEPROM（全部写0）
 * @note 会清除所有数据，谨慎使用！
 */
void AT24C02_Format(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT24C02_MANAGER_H__ */
