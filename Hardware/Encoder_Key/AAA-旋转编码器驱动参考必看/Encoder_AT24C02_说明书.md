# 旋转编码器参数编辑系统 & AT24C02 持久化 — 说明书

> **适用芯片**: STM32F103C8T6（Cortex-M3）  
> **开发环境**: Keil MDK / CubeMX  
> **最后更新**: 2026-07-13

---

## 目录

1. [系统总览](#1-系统总览)
2. [Encoder_Key — 旋转编码器驱动](#2-encoder_key--旋转编码器驱动)
3. [ParamEdit — 参数编辑器](#3-paramedit--参数编辑器)
4. [AT24C02 — EEPROM 持久化](#4-at24c02--eeprom-持久化)
5. [ParamEdit ↔ AT24C02 联动](#5-paramedit--at24c02-联动)
6. [Key — 按键驱动](#6-key--按键驱动)
7. [OLED — 显示驱动](#7-oled--显示驱动)
8. [跨芯片移植指南](#8-跨芯片移植指南)
9. [完整示例代码](#9-完整示例代码)
10. [常见问题 & 注意事项](#10-常见问题--注意事项)

---

## 1. 系统总览

```
┌──────────────────────────────────────────────────┐
│                    应用层 (Mymain.c)               │
│  Param_Init / Param_Register / Param_Loop         │
├──────────────────────────────────────────────────┤
│  ParamEdit (参数编辑器)    Param_AT24C02 (持久化)   │
│  管理参数表+UI交互          EEPROM读写+数据恢复      │
├──────────────────────────────────────────────────┤
│  Encoder_Key    Key         OLED      at24c02_mgr │
│  (旋转编码器)   (按键状态机)  (显示)    (参数映射)    │
├──────────────────────────────────────────────────┤
│                         bsp_at24c02 (I2C驱动)      │
├──────────────────────────────────────────────────┤
│           STM32F103C8T6 硬件层 (HAL)              │
│  PB0/PB1(EXTI) PB12/PB13(GPIO) PB8/PB9(I2C) ...  │
└──────────────────────────────────────────────────┘
```

### 硬件引脚分配总表

| 引脚 | 功能 | 外设 | 连接目标 |
|------|------|------|----------|
| **PB0** | 编码器 A 相 | EXTI0, 下降沿中断 | 旋转编码器 S1 |
| **PB1** | 编码器 B 相 | EXTI1, 下降沿中断 | 旋转编码器 S2 |
| **PB11** | 编码器按键 | GPIO Input, 上拉 | 旋转编码器 SW（下压） |
| **PB12** | 按键 KEY1 | GPIO Input, 上拉 | 外部轻触按键 |
| **PB13** | 按键 KEY2 | GPIO Input, 上拉 | 外部轻触按键 |
| **PB8** | OLED SCL | GPIO Output（软件I2C） | OLED 时钟线 |
| **PB9** | OLED SDA | GPIO Output（软件I2C） | OLED 数据线 |

---

## 2. Encoder_Key — 旋转编码器驱动

### 2.1 文件位置

| 文件 | 路径 |
|------|------|
| 头文件 | `Hardware/Encoder_Key.h` |
| 源文件 | `Hardware/Encoder_Key.c` |

### 2.2 硬件资源配置

```
引脚: PB0 (EXTI0, 下降沿) — 编码器 A 相
      PB1 (EXTI1, 下降沿) — 编码器 B 相
MCU:  STM32F103C8T6 @ 72MHz
```

CubeMX 中 PB0/PB1 需配置为 **GPIO_EXTI0 / GPIO_EXTI1**，下降沿触发，内部上拉。

**中断优先级**：EXTI0 和 EXTI1 的中断优先级需相同（同一中断向量组），默认即可。

### 2.3 API

```c
void    Encoder_Init(void);   // 初始化（GPIO/NVIC已在CubeMX生成，无需额外操作）
int16_t Encoder_Get(void);    // 读取旋转增量，读取后自动清零
```

- **返回值**: 本次调用前累积的旋转脉冲数，CW 为正，CCW 为负
- **清零机制**: 每次读取后内部计数器归零，不会被"重复累加"
- **计数范围**: ±32767（int16_t）

### 2.4 工作原理

使用外部中断（EXTI）在 PB0/PB1 的下降沿触发，通过读取另一相电平判断方向：

```
PB0 下降沿 + PB1=低 → CW  → Count++
PB1 下降沿 + PB0=低 → CCW → Count--
```

ISR 内含软件去抖：中断触发后再次读取引脚电平确认仍为低。

### 2.5 配置修改

无特殊宏定义。方向逻辑如需反转，在 `HAL_GPIO_EXTI_Callback()` 中将 `++` 和 `--` 互换即可。

---

## 3. ParamEdit — 参数编辑器

### 3.1 文件位置

| 文件 | 路径 |
|------|------|
| 头文件 | `Hardware/ParamEdit.h` |
| 源文件 | `Hardware/ParamEdit.c` |

### 3.2 可配置宏

所有宏定义在 `ParamEdit.h` 头部，**改这里就能改变行为**：

```c
// ================ AT24C02 联动开关 ================
#define PARAM_USE_AT24C02       1     // 1=启用, 0=完全解耦

// ================ 按键映射 ================
#define PARAM_KEY_ENTER_EXIT    KEY_1 // 进入/退出参数编辑（长按）
#define PARAM_KEY_NEXT          KEY_1 // 下一个参数（单击）
#define PARAM_KEY_PREV          KEY_1 // 上一个参数（双击）
#define PARAM_KEY_SAVE          KEY_3 // 保存到 AT24C02（单击）
```

> **换键示例**: 想让 KEY2 负责"下一个参数"？改 `#define PARAM_KEY_NEXT KEY_2` 即可。

### 3.3 参数类型

```c
typedef enum {
    PARAM_INT8,     // int8_t   (-128 ~ 127)
    PARAM_INT16,    // int16_t  (-32768 ~ 32767)
    PARAM_INT32,    // int32_t  (-2^31 ~ 2^31-1)
    PARAM_FLOAT     // float
} ParamType;
```

### 3.4 API

```c
void Param_Init(void);
void Param_Clear(void);
void Param_Register(const char *name, void *var, float step, ParamType type);
void Param_Loop(void);
int  Param_IsActive(void);
```

| 函数 | 说明 |
|------|------|
| `Param_Init()` | 初始化参数表（清空所有已注册参数） |
| `Param_Clear()` | 清空参数表，之后可重新 `Param_Register()` 注册新参数 |
| `Param_Register(name, &var, step, type)` | 注册一个参数：名称、变量指针、步长、类型 |
| `Param_Loop()` | 主循环调用：状态机（进入/退出编辑 + 编码器 + 按键 + OLED刷新） |
| `Param_IsActive()` | 返回 1=正在编辑模式, 0=正常模式 |

### 3.5 交互逻辑

```
正常模式 ──(KEY1 长按 1s)──→ 编辑模式
                               │
  旋转编码器 ← 修改当前参数值   │
  KEY1 单击  ← 下一个参数      │  OLED 实时刷新
  KEY1 双击  ← 上一个参数      │  参数到头自动回绕
  KEY3 单击  ← 保存到 AT24C02  │
                               │
编辑模式 ──(KEY1 长按 1s)──→ 正常模式
```

### 3.6 OLED 布局

```
┌──────────────────────┐  128×64, 6×8 字体
│=== Param Edit ===    │  第0行(标题)
│>Speed:*100 st:10     │  第1行(当前参数, *表示未保存)
│ Kp: 1.20   st:0.1    │  第2行
│ Ki: 0.05   st:0.01   │  第3行
│ Mode: 0    st:1      │  第4行
│LONG=Exit  KEY3=Save  │  底部提示
└──────────────────────┘
```

- 最多同时显示 `OLED_MAX_SHOW` 个参数（默认 4 个，从第1行起）
- `>` 标记当前选中参数
- `*` 标记 AT 参数已修改但未保存（仅 `PARAM_USE_AT24C02=1` 时）
- 底部提示行随 AT 开关自动变化

---

## 4. AT24C02 — EEPROM 持久化

### 4.1 三层架构

```
Param_AT24C02.c    ← 业务层：定义"哪些变量需要持久化"
      │
at24c02_manager.c  ← 管理层：参数注册、地址分配、读写管理
      │
bsp_at24c02.c      ← 驱动层：I2C 字节读写（软/硬件 I2C 切换）
```

### 4.2 文件位置

| 层级 | 文件 | 路径 |
|------|------|------|
| 驱动层 | `bsp_at24c02.h/.c` | `Hardware/` |
| 管理层 | `at24c02_manager.h/.c` | `Hardware/` |
| 业务层 | `Param_AT24C02.h/.c` | `Hardware/` |

### 4.3 硬件资源配置

```
引脚: PB14 (AT_SCL), PB15 (AT_SDA)
设备: AT24C02 (256 字节 EEPROM)
地址: 0xA0 (写), 0xA1 (读)
```

> **PB14/PB15 在 CubeMX 中需标注 User Label**: `AT_SCL` / `AT_SDA`，否则 `bsp_at24c02.h` 中的宏 `AT_SCL_Pin` / `AT_SDA_Pin` 找不到定义。

### 4.4 可配置宏（bsp_at24c02.h）

```c
#define AT24C02_USE_SW_I2C    1    // 1=软件I2C(GPIO模拟), 0=硬件I2C(使用I2C2外设)
```

**选 1（软件 I2C）**：使用 PB14/PB15 做 GPIO 模拟，不占用硬件 I2C 外设，适合任意 GPIO。  
**选 0（硬件 I2C）**：使用 STM32 的 I2C2 外设 (`hi2c2`)，需在 CubeMX 中使能 I2C2 并配置 PB10/PB11（注意与编码器按键 PB11 冲突！建议用 PB10/PB14 或其他）。

### 4.5 注册宏（at24c02_manager.h）

```c
#define AT_PARAM_I8(_ptr, _def)    // 注册 int8_t  参数
#define AT_PARAM_I16(_ptr, _def)   // 注册 int16_t 参数
#define AT_PARAM_I32(_ptr, _def)   // 注册 int32_t 参数
#define AT_PARAM_F(_ptr, _def)     // 注册 float   参数
```

### 4.6 API（Param_AT24C02.h — 业务层最常用）

```c
void    Param_AT24C02_Init(void);                     // 初始化：注册所有AT参数 + 从EEPROM恢复
void    Param_AT24C02_SaveAll(void);                  // 批量保存所有AT参数
void    Param_AT24C02_EraseAll(void);                 // 擦除整个AT24C02（填0xFF，约1.3秒）
uint8_t Param_AT24C02_Write(void *varPtr);            // 写单个变量到EEPROM，返回1=成功
uint8_t Param_AT24C02_Read(void *varPtr);             // 从EEPROM读单个变量，返回1=成功
```

### 4.7 API（at24c02_manager.h — 管理层，更底层）

```c
void    AT_Manager_Register(const AT_ParamItem *item);  // 注册单个参数项
void    AT_Manager_Init(void);                           // 从EEPROM恢复所有已注册参数
uint8_t AT_Manager_Write(void *varPtr);                  // 写单个变量
uint8_t AT_Manager_Read(void *varPtr);                   // 读单个变量
void    AT_Manager_SaveAll(void);                        // 批量写
void    AT_Manager_LoadAll(void);                        // 批量读
uint8_t AT_Manager_GetCount(void);                       // 参数个数
uint8_t AT_Manager_GetATAddr(void *varPtr);              // 通过指针反查EEPROM地址
```

### 4.8 EEPROM 地址分配

地址**按注册顺序自动分配**，不需要手动指定：

```c
// Param_AT24C02.c 中的注册表
static const AT_ParamItem s_AT_Params[] = {
    { AT_PARAM_I8(&g_mode,    0)    },   // addr=0x00, size=1
    { AT_PARAM_I32(&g_motorSpeed, 0) },  // addr=0x01, size=4
    { AT_PARAM_F(&g_pidKp,    1.0f) },   // addr=0x05, size=4
    { AT_PARAM_F(&g_pidKi,    0.0f) },   // addr=0x09, size=4
    { AT_PARAM_F(&g_pidKd,    0.0f) },   // addr=0x0D, size=4
};
// 总计使用 17 字节 (0x00~0x10)，AT24C02 共 256 字节可用
```

### 4.9 首次上电 / 空白 EEPROM 处理

新 AT24C02 所有字节为 `0xFF`。Manager 检测到数据全 `0xFF` 时自动使用注册宏中指定的**默认值**。例如 `AT_PARAM_F(&g_pidKp, 1.0f)` 在空白芯片上会赋值 `g_pidKp = 1.0f`。

---

## 5. ParamEdit ↔ AT24C02 联动

### 5.1 联动开关

```c
// ParamEdit.h 第 10 行
#define PARAM_USE_AT24C02    1    // 1=联动, 0=独立运行
```

设为 `0` 后编译产物**完全不包含 AT24C02 代码**，ParamEdit 退化回纯内存参数编辑器。

### 5.2 联动机制

两个系统通过**变量指针**建立起联系：

1. `Param_AT24C02_Init()` 调用 `AT_Manager_Register()` 注册 AT 参数（记录每个变量的 EEPROM 地址）
2. `Param_Register("Speed", &g_motorSpeed, ...)` 用 `AT_Manager_GetATAddr(&g_motorSpeed)` 查询该变量是否在 AT 表中
3. 命中 → `is_at=1`，并调用 `AT_Manager_Read()` 加载 EEPROM 存储值
4. 未命中 → `is_at=0`，不做任何 AT 相关操作

### 5.3 脏标记机制

| 操作 | 效果 |
|------|------|
| 旋转编码器修改 AT 参数 | `dirty=1`，OLED 显示 `*` |
| 旋转编码器修改非 AT 参数 | 无影响，不显示 `*` |
| 按下 KEY3（编码器按键）| 写回 EEPROM，`dirty=0`，`*` 消失 |
| 断电重启 | EEPROM 中的值被 `Param_AT24C02_Init()` 恢复，`dirty` 初始为 0 |

### 5.4 时序要求

```c
// Mymain.c 中的正确顺序
Param_AT24C02_Init();   // ① 先恢复 EEPROM 值到全局变量
Param_Init();            // ② 再初始化参数编辑器
Param_Register(...);     // ③ 注册参数（AT参数自动补加载）
```

如果 `Param_AT24C02_Init()` 调用在 `Param_Register()` 之后，也不会有问题——`Param_Register()` 内部会尝试调用 `AT_Manager_Read()`，如果 AT 尚未初始化（`s_count==0`），则 `AT_Manager_GetATAddr()` 返回 0xFF，`is_at` 标记为 0。后续 `Param_AT24C02_Init()` 加载的值会覆盖变量，但 ParamEdit 中不会显示 `*`。为避免这种不一致，**建议始终按上述顺序调用**。

---

## 6. Key — 按键驱动

### 6.1 文件位置

| 文件 | 路径 |
|------|------|
| 头文件 | `Hardware/Key.h` |
| 源文件 | `Hardware/Key.c` |

### 6.2 宏定义

```c
#define KEY_COUNT       4       // 按键数量（当前 KEY1~KEY3 已使用）
#define KEY_1           0
#define KEY_2           1
#define KEY_3           2       // 编码器按键
#define KEY_4           3       // 预留

#define KEY_HOLD        0x01    // 按住
#define KEY_DOWN        0x02    // 按下瞬间
#define KEY_UP          0x04    // 释放瞬间
#define KEY_SINGLE      0x08    // 单击
#define KEY_DOUBLE      0x10    // 双击
#define KEY_LONG        0x20    // 长按（1秒）
#define KEY_REPEAT      0x40    // 长按连发（每100ms）
```

### 6.3 可调参数（Key.c 内部）

```c
#define KEY_TIME_DOUBLE   200    // 双击判定窗口 (ms)，改小可加快单击响应
#define KEY_TIME_LONG     1000   // 长按判定阈值 (ms)
#define KEY_TIME_REPEAT   100    // 长按连发间隔 (ms)
```

### 6.4 API

```c
uint8_t Key_Check(uint8_t n, uint8_t Flag);   // 检查按键标志，非HOLD标志读取后自动清除
void    Key_Tick(void);                        // 每1ms调用一次（需在SysTick中断中）
```

**使用示例**：
```c
if (Key_Check(KEY_1, KEY_SINGLE)) { /* KEY1 单击 */ }
if (Key_Check(KEY_1, KEY_LONG))   { /* KEY1 长按 */ }
```

### 6.5 添加新按键

两步：

1. **CubeMX**: 将目标 GPIO 设为 Input + Pull-up，加 User Label（如 `KEY3`）
2. **Key.c `Key_GetState()`**: 添加分支：
```c
else if (n == KEY_3) {
    if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == 0)
        return KEY_PRESSED;
}
```

### 6.6 调用要求

`Key_Tick()` **必须在 1ms 定时中断中调用**：
```c
void HAL_SYSTICK_Callback(void) {
    Key_Tick();           // ← 必须
    LED_Flash_Mode_Tick();
}
```

---

## 7. OLED — 显示驱动

### 7.1 文件位置

| 文件 | 路径 |
|------|------|
| OLED 驱动 | `Hardware/OLED.h/.c` |
| 字体数据 | `Hardware/OLED_Data.h/.c` |

### 7.2 硬件资源

```
引脚: PB8 (OLED_SCL), PB9 (OLED_SDA)
设备: 0.96" OLED, SSD1306 控制器, 128×64 像素
I2C:  软件 GPIO 模拟（不占用硬件 I2C 外设）
地址: 0x78 (0x3C << 1)
```

### 7.3 常用 API

```c
void OLED_Clear(void);
void OLED_Update(void);                                      // 刷新显示缓冲区到屏幕
void OLED_Printf(uint8_t x, uint8_t y, uint8_t font,         // printf 风格输出
                 const char *fmt, ...);
void OLED_ShowString(uint8_t x, uint8_t y,                   // 显示字符串
                     const char *str, uint8_t font);
void OLED_ShowSignedNum(uint8_t x, uint8_t y,                // 显示有符号数
                        int32_t num, uint8_t len, uint8_t font);
```

- `font`: `OLED_6X8`（21字符/行，8行）或 `OLED_8X16`（16字符/行，4行）
- `OLED_Printf()` 支持标准 printf 格式符（`%d`, `%.2f`, `%s` 等）

---

## 8. 跨芯片移植指南

### 8.1 移植清单

移植到其他 MCU（如 TI MSPM0、ESP32、GD32 等），需修改以下内容：

| 模块 | 需修改的文件 | 修改内容 |
|------|------------|----------|
| **Encoder_Key** | `Encoder_Key.c` | GPIO 读取函数 → 目标芯片 HAL（如 `GPIO_readPin()`） |
| **Key** | `Key.c` | `HAL_GPIO_ReadPin()` → 目标芯片 GPIO API |
| **OLED** | `OLED.c` | 软件 I2C 的 SCL/SDA 操作宏 → 目标芯片 GPIO API；微秒延时 → 目标芯片定时器 |
| **bsp_at24c02** | `bsp_at24c02.c` | 同上，I2C 函数全部重写或使用目标芯片硬件 I2C |
| **SysTick** | 中断配置 | `Key_Tick()` 需改为目标芯片的 1ms 定时器回调 |

### 8.2 HAL 抽象层建议

如果想保持移植性，可以创建一个 `hal_gpio.h` 抽象层，将芯片相关的 GPIO 操作封装为统一宏：

```c
// 示例：STM32 版本
#define HAL_GPIO_READ(port, pin)    HAL_GPIO_ReadPin(port, pin)
#define HAL_GPIO_WRITE(port, pin, v) HAL_GPIO_WritePin(port, pin, v)

// 示例：MSPM0 版本
#define HAL_GPIO_READ(port, pin)    DL_GPIO_readPins(port, pin)
#define HAL_GPIO_WRITE(port, pin, v) DL_GPIO_setPins(port, pin)
```

### 8.3 不需要移植的模块

以下模块**完全与硬件无关**，可直接复用：

| 模块 | 说明 |
|------|------|
| `ParamEdit.c` | 纯逻辑：参数表管理 + 状态机 + OLED 格式化输出 |
| `at24c02_manager.c` | 纯逻辑：参数注册、地址分配、字节组包/解包 |
| `Param_AT24C02.c` | 纯业务：参数表定义、调用 Manager API |

### 8.4 MSPM0 移植示例

TI MSPM0G3507 移植要点：

```c
// 1. 编码器 — 使用 GPIO 中断（MSPM0 的 INPUTMUX + GPIOA）
//    在 ISR 中调用类似逻辑，替换 HAL_GPIO_ReadPin 为 DL_GPIO_readPins

// 2. 按键 — 替换 Key_GetState() 中的引脚读取

// 3. OLED / AT24C02 I2C — MSPM0 有硬件 I2C，建议用硬件模式：
//    将 bsp_at24c02.h 中 AT24C02_USE_SW_I2C 改为 0
//    重写硬件 I2C 的实现（替换 HAL_I2C_Mem_Write/Read）

// 4. SysTick — MSPM0 用 SysTick 或通用定时器，1ms 中断中调用 Key_Tick()

// 5. 其余代码（ParamEdit、at24c02_manager、Param_AT24C02）直接复用，无需修改
```

---

## 9. 完整示例代码

### 9.1 最小化初始化（不使用 AT24C02）

```c
#include "ParamEdit.h"
#include "Encoder_Key.h"

int32_t g_speed = 0;
float   g_kp    = 1.0f;
int8_t  g_mode  = 0;

void Mymain(void)
{
    Initial_ALL();      // 初始化 OLED、串口等
    __enable_irq();
    Encoder_Init();

    Param_Init();
    Param_Register("Speed", &g_speed, 10.0f, PARAM_INT32);
    Param_Register("Kp",    &g_kp,    0.1f,  PARAM_FLOAT);
    Param_Register("Mode",  &g_mode,  1.0f,  PARAM_INT8);

    while (1)
    {
        Param_Loop();   // 长按 KEY1 进入编辑，再长按退出

        if (!Param_IsActive())
        {
            // 非编辑模式：你的业务代码
            // g_speed, g_kp, g_mode 的值已通过编码器实时更新
        }
    }
}
```

### 9.2 完整初始化（含 AT24C02 持久化）

```c
#include "ParamEdit.h"
#include "Encoder_Key.h"
#include "Param_AT24C02.h"

// ===== 全局变量（与 Param_AT24C02.c 中的 s_AT_Params 对应）=====
int32_t  g_motorSpeed = 0;
float    g_pidKp      = 1.0f;
float    g_pidKi      = 0.0f;
float    g_pidKd      = 0.0f;
int8_t   g_mode       = 0;
int8_t   g_mode2      = 0;    // 非AT参数，不持久化

void Mymain(void)
{
    Initial_ALL();
    __enable_irq();

    Encoder_Init();

    // ① AT24C02 初始化：将 EEPROM 值恢复到全局变量
    Param_AT24C02_Init();

    // ② 注册参数表（AT参数自动从EEPROM加载值）
    Param_Init();
    Param_Register("Speed", &g_motorSpeed, 10.0f, PARAM_INT32);  // AT参数
    Param_Register("Kp",    &g_pidKp,      0.1f,  PARAM_FLOAT);  // AT参数
    Param_Register("Ki",    &g_pidKi,      0.01f, PARAM_FLOAT);  // AT参数
    Param_Register("Kd",    &g_pidKd,      0.01f, PARAM_FLOAT);  // AT参数
    Param_Register("Mode",  &g_mode,       1.0f,  PARAM_INT8);   // AT参数
    Param_Register("Mode2", &g_mode2,      1.0f,  PARAM_INT8);   // 非AT参数

    while (1)
    {
        Param_Loop();

        if (!Param_IsActive())
        {
            // 非编辑模式的业务逻辑
            // 参数值在编辑模式中被实时修改，退出后即可使用新值
        }
    }
}
```

### 9.3 AT24C02 参数表定义

在 `Param_AT24C02.c` 中定义需要持久化的变量：

```c
#include "Param_AT24C02.h"

// ===== 业务全局变量（extern 自 Mymain.c）=====
extern int32_t  g_motorSpeed;
extern float    g_pidKp;
extern float    g_pidKi;
extern float    g_pidKd;
extern int8_t   g_mode;

// ===== AT24C02 参数注册表（地址自动分配）=====
static const AT_ParamItem s_AT_Params[] = {
    { AT_PARAM_I8(&g_mode,       0)    },   // EEPROM addr 0x00, 1byte
    { AT_PARAM_I32(&g_motorSpeed, 0)   },   // EEPROM addr 0x01, 4bytes
    { AT_PARAM_F(&g_pidKp,       1.0f) },   // EEPROM addr 0x05, 4bytes
    { AT_PARAM_F(&g_pidKi,       0.0f) },   // EEPROM addr 0x09, 4bytes
    { AT_PARAM_F(&g_pidKd,       0.0f) },   // EEPROM addr 0x0D, 4bytes
};

void Param_AT24C02_Init(void)
{
    for (int i = 0; i < sizeof(s_AT_Params)/sizeof(s_AT_Params[0]); i++)
        AT_Manager_Register(&s_AT_Params[i]);
    AT_Manager_Init();  // 从 EEPROM 恢复值（空白则用默认值）
}
```

### 9.4 动态参数表切换

```c
// 使用 Param_Clear() 在不同模式下切换参数集

enum { MODE_PID, MODE_PWM } currentMode = MODE_PID;

void SwitchToMode(int mode)
{
    Param_Clear();   // 清空旧参数

    if (mode == MODE_PID)
    {
        Param_Register("Kp", &g_kp, 0.1f, PARAM_FLOAT);
        Param_Register("Ki", &g_ki, 0.01f, PARAM_FLOAT);
        Param_Register("Kd", &g_kd, 0.01f, PARAM_FLOAT);
    }
    else if (mode == MODE_PWM)
    {
        Param_Register("Freq",  &g_pwmFreq,  100.0f, PARAM_INT32);
        Param_Register("Duty",  &g_pwmDuty,  1.0f,   PARAM_INT8);
    }

    currentMode = mode;
}
```

### 9.5 手动读写 AT24C02（不使用 ParamEdit 联动）

```c
// 直接通过 Param_AT24C02 API 读写 EEPROM

// 保存单个变量
Param_AT24C02_Write(&g_motorSpeed);

// 读取单个变量
Param_AT24C02_Read(&g_motorSpeed);

// 批量保存所有AT参数
Param_AT24C02_SaveAll();

// 擦除整个 EEPROM（恢复出厂设置）
Param_AT24C02_EraseAll();  // 耗时约 1.3 秒
```

---

## 10. 常见问题 & 注意事项

### 10.1 编码器

| 问题 | 解答 |
|------|------|
| 旋转方向反了？ | 在 `Encoder_Key.c` 的 `HAL_GPIO_EXTI_Callback()` 中将 `++` 和 `--` 互换 |
| 旋转不灵敏/跳数？ | 检查外部中断触发沿（应为下降沿），检查编码器供电和上拉电阻 |
| 参数值跳变？ | `Param_Loop()` 进入编辑模式时自动调用 `Encoder_Get()` 丢弃积攒值 |

### 10.2 AT24C02

| 问题 | 解答 |
|------|------|
| 读写失败/卡死？ | 检查 PB14/PB15 是否焊接正确，I2C 地址是否为 0xA0 |
| 软件 I2C 时序不准？ | 调整 `AT24C02_DELAY_US(n)` 中的循环次数 `n*8`（与主频成正比） |
| 参数值不恢复？ | 检查 `Param_AT24C02_Init()` 是否在 `Param_Register()` 之前调用 |
| 硬件 I2C 冲突？ | PB11 被编码器按键占用，硬件 I2C2 默认使用 PB10/PB11；改用 PB10/PB14 或其他引脚 |
| EEPROM 写次数寿命？ | AT24C02 约 100 万次擦写寿命，仅 KEY3 按下时写入，正常使用足够 |

### 10.3 ParamEdit

| 问题 | 解答 |
|------|------|
| OLED 无显示？ | 检查 `Key_Tick()` 是否在 SysTick 中调用（缺少则按键状态机不工作） |
| 参数超出范围？ | `Param_Add()` 不做范围限制，int8_t 溢出自绕回；如需限幅，在业务代码中处理 |
| 超过 30 个参数？ | 改 `MAX_PARAM` 宏，注意 RAM 占用（每个参数约 20 字节） |
| 如何隐藏非 AT 参数的 `*`？ | `is_at=0` 的参数自动不显示 `*`，无需额外处理 |

### 10.4 依赖关系

```c
// Mymain.h 中需要的 include（已默认包含）
#include "OLED.h"          // OLED 驱动
#include "Key.h"           // 按键驱动
#include "ParamEdit.h"     // 参数编辑器

// AT24C02 相关（按需）
#include "bsp_at24c02.h"        // 底层 I2C
#include "at24c02_manager.h"    // 管理层
#include "Param_AT24C02.h"      // 业务层
```

### 10.5 内存占用估算

| 模块 | 大致 RAM | 大致 Flash |
|------|----------|------------|
| Encoder_Key | ~2 B | ~200 B |
| ParamEdit | param数×20 B + ~100 B | ~1.5 KB |
| Key | ~50 B | ~500 B |
| OLED | 1 KB (显示缓冲) | ~3 KB |
| at24c02_manager | param数×16 B + ~50 B | ~1 KB |
| bsp_at24c02 (软件I2C) | ~10 B | ~800 B |
| **合计** | **约 1.5 KB RAM** | **约 7 KB Flash** |

STM32F103C8T6 有 20 KB RAM / 64 KB Flash，绰绰有余。

---

## 附录 A：文件结构速查

```
Hardware/Encoder_Key/
├── Hardware/
│   ├── Encoder_Key.h / .c      ★ 编码器驱动
│   ├── ParamEdit.h / .c        ★ 参数编辑器
│   ├── Key.h / .c              ★ 按键驱动
│   ├── OLED.h / .c / OLED_Data.h / .c  显示驱动
│   ├── bsp_at24c02.h / .c           AT24C02 底层驱动
│   ├── at24c02_manager.h / .c       AT24C02 管理层
│   └── Param_AT24C02.h / .c    ★ AT24C02 业务层（定义持久化参数表）
├── Top/
│   ├── Mymain.h / .c           ★ 用户入口
│   ├── Initial.h / .c              系统初始化
│   └── Menu_Key.h / .c             [已废弃]
├── Tools/
│   ├── Key_Check.h / .c            [已废弃]
│   ├── Timer_Counter.h / .c        微秒计时
│   └── LED_Flash.h / .c           LED 闪烁
└── AAA-旋转编码器驱动参考必看/
    ├── Encoder_AT24C02_说明书.md    ← 你正在读的文件
    └── ...
```

★ = 开发者最常接触的文件

---

## 附录 B：按键宏速查

| 宏 | 默认值 | 触发方式 | 作用 |
|----|--------|----------|------|
| `PARAM_KEY_ENTER_EXIT` | `KEY_1` | 长按(1s) | 进入/退出参数编辑模式 |
| `PARAM_KEY_NEXT` | `KEY_1` | 单击 | 选择下一个参数 |
| `PARAM_KEY_PREV` | `KEY_1` | 双击 | 选择上一个参数 |
| `PARAM_KEY_SAVE` | `KEY_3` | 单击 | 保存当前 AT 参数到 EEPROM |
| `PARAM_USE_AT24C02` | `1` | 编译时 | AT24C02 联动开关 |
