# Template-F1

# 1. 项目基础说明

+ 核心架构
  + 使用Mymain隔离复杂HAL代码，实现主函数简化
  + 使用Allheader万能头，实现函数方便引用
  + 使用ISR回调，实现中断回调集中一个库处理，方便中断全局概览
  + 使用Mode架构，实现模式解耦切换
    + Mode的中断默认都使用20ms更新，需要其他参数的需要自己配置

+ 核心硬件
  + OLED
  + Key
  + 串口Serial调试
  + 时基定时器选择Systick（暂时）

# 2. 项目硬件配置

## 2-1 基础配置

| 引脚号 |  Label   |           备注           |
| :----: | :------: | :----------------------: |
|  PC13  |   LED0   |         板载 LED         |
|  PB12  |   KEY0   |        板载 按键0        |
|  PB13  |   KEY1   | 外置 按键2 上拉（按键1） |
|     |           |                                |
|  PB8  | OLED_SCL |         软件OLED         |
|  PB9  | OLED_SDA |         软件OLED         |
|  PA9  | USART1_TX | 串口调试发送（115200） |
|  PA10  | USART1_RX | Serial1 接收（DMA + 空闲检测） |
|  PA2  | USART2_TX | Serial2 发送（115200） |
|  PA3  | USART2_RX | Serial2 接收（DMA + 空闲检测） |
|  |           |                                |
|        |           |                                |
|        |           |                                |

## 2-2 定时器

|   定时器    | 更新中断 |
| :---------: | :------: |
|   Systick   |   1ms    |
| Systick分频 |   20ms   |
# 3. 模板行为

- Core/Src/main.c 完成 CubeMX 的 GPIO、DMA、USART1、USART2 初始化后进入 Mymain()。
- Top/Mode/Tools/Function/Hardware 分别承载主循环、中断入口、模式、工具、串口接口及设备驱动，不使用 MySystem。
- SysTick 每 1ms 调用 MyISR_SysTick() 扫描按键，每累计 20 次分派当前模式的 Mode_x_Tick()。Initial_Timer() 在 OLED 和串口初始化后开启这些应用回调，不增加定时器外设。
- KEY0（PB12）单击切换模式；KEY1（PB13）在 Mode2 单击使 num *= 10、双击使 num /= 10、长按使 num = 0。Mode2 OLED 显示当前 num，20ms 回调通过 Serial1 打印整数加换行。无 KEY2。
- OLED 使用 PB8/PB9 的 CubeMX GPIO 输出配置，按键使用 PB12/PB13 的 CubeMX 上拉输入配置；驱动只通过 HAL GPIO API 操作引脚。
- Serial1 和 Serial2 使用 CubeMX 生成的 huart1/huart2 与 DMA；通过 HAL_UARTEx_ReceiveToIdle_DMA 接收，在 MyISR.c 中集中处理 HAL 接收和错误回调。Serial_porting.h 保留与 F407 模板相同的双串口打印、字节发送、ABC/HEX 解析及查询 API。
- USART1/2 和 DMA1 Channel4/5/6/7 的 IRQ 函数由 CubeMX 的 stm32f1xx_it.c 唯一定义，不在 MyISR 中重复定义。
- Keil 工程中的应用文件按 Top、Mode、Tools、Function、Hardware 分组。重新生成 CubeMX 代码后，应确认 Keil 项目仍包含这些组及对应头文件搜索路径。

# 4. 构建

在 MDK-ARM/Template_F103C8T6.uvprojx 中选择 Template_F103C8T6 目标并构建。当前 Keil ARMCC 5 构建记录：0 Error(s), 0 Warning(s)。详情见 模板工程构建报告.md。
