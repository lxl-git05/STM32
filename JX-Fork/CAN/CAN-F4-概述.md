# CAN-F4 工程概述

## 1. 当前状态

2026-09-29 首次读取，采用快速概览。工程基于 STM32F407ZGTx、C 语言和 STM32 HAL，保留 CubeMX 配置与 Keil MDK 工程。当前是带 OLED、按键、双串口和模式调度的学习模板，尚未实现 CAN 通信。本次仅阅读与记录，未编译或烧录。

## 2. 工程地图

| 目录或文件 | 职责 |
|---|---|
| CAN-F4/Template_F407ZGT6.ioc | CubeMX 外设与时钟配置，固件包标记为 STM32Cube FW_F4 V1.28.3 |
| CAN-F4/MDK-ARM/Template_F407ZGT6.uvprojx | Keil 工程，目标 STM32F407ZGTx，uAC6=0 |
| CAN-F4/Core/ | main、GPIO、DMA、TIM、USART、异常处理及 HAL 时基 |
| CAN-F4/Top/ | Mymain 主循环、AllHeader 初始化与头文件汇总、MyISR 中断回调 |
| CAN-F4/Mode/ | Mode_G 全局管理，Mode_1 至 Mode_6 的 Setup/Loop/Tick/Exit |
| CAN-F4/Hardware/ | OLED、字库、按键、串口基础模块 |
| CAN-F4/Function/ | Serial_porting 串口 HAL 适配及报文解析 |
| CAN-F4/Tools/ | Timer_Counter 计时工具 |
| CAN-F4/Drivers/ | ST HAL 与 CMSIS 库；包含 CAN 驱动源码，但尚未接入应用 |
| CAN-F4/README-F4.md | 模板说明和引脚表 |

## 3. 核心执行流程

1. Core/Src/main.c：HAL_Init → SystemClock_Config → GPIO/DMA/TIM6/TIM7/USART1/USART2 初始化 → Mymain。
2. Top/Mymain.c：调用 Mode_G_Setup，再持续执行 OLED_Clear、Mode_G_Loop、模式处理、OLED_Update。
3. Mode/Mode_G.c：初始化 OLED、串口并启动 TIM6/TIM7 中断。curr_mode 初始为 Mode_Null，next_mode 初始为 Mode_1，因此正常启动后进入 Mode_1。KEY_0 单击请求切换模式。
4. 模式未改变时执行当前模式的 Loop；改变时先执行旧模式 Exit，再执行新模式 Setup，最后更新 curr_mode。这给不同实验提供独立入口。
5. Top/MyISR.c：TIM6 每 1 ms 调用 Key_Tick；TIM7 每 20 ms 调用当前模式 Tick；UART 接收事件和错误回调转交 Serial 模块。Tick 在中断上下文执行。

## 4. 当前功能与 CAN 接入位置

- Mode_1.c 注释已指定“CAN环回测试 / 自发自收”，但 Setup、Tick、Exit 为空，Loop 仅显示标题。
- Mode_2.c 是按键、计数、OLED 和串口演示；其 Tick 调用 Serial_printf，底层使用阻塞式 HAL_UART_Transmit。后续设计接收中断时应考虑这一现有行为。
- Core 下没有 can.c/can.h，main 没有 CAN 初始化，应用目录未发现 hcan 或 HAL_CAN 收发调用。
- stm32f4xx_hal_conf.h 中 HAL_CAN_MODULE_ENABLED 仍被注释，Keil 源文件列表也未包含 HAL CAN 驱动。
- 后续 CAN 实验可沿用 Mode_1 的生命周期组织：Setup 做实验初始化，Loop 处理实验与显示，Exit 做必要收尾；中断回调可沿用 MyISR 的集中组织方式。具体实现待后续学习任务确定。

## 5. 时钟与现有资源

实际配置为 HSE 8 MHz，SYSCLK/HCLK 168 MHz，APB1 42 MHz、APB2 84 MHz；对应 APB1 定时器时钟 84 MHz、APB2 定时器时钟 168 MHz。README 将 APB 总线时钟与定时器时钟混写，后续配置时应核对 main.c 与 .ioc。

| 资源 | 当前配置 |
|---|---|
| HAL 时基 | TIM13 |
| TIM6 / TIM7 | 1 ms / 20 ms 更新中断 |
| USART1 | PA9/PA10，115200，接收 DMA + 空闲事件 |
| USART2 | PA2/PA3，115200，接收 DMA + 空闲事件 |
| OLED | PB8/PB9 |
| 按键 | PE4、PF4、PF6 |
| LED0 | PB5 |

## 6. 后续使用入口

在 Keil 打开 MDK-ARM 下的 uvprojx；在 CubeMX 打开 .ioc。当前尚无可以运行的 CAN 示例，第一项实验入口已经预留在 Mode_1。后续实施前需要结合实际开发板确认 CAN 引脚及外部硬件连接；本次没有据此推定接线。

本概述依据项目配置和应用源码形成；未逐行审查第三方 HAL/CMSIS 全库，也未将已有编译产物视为本次验证结果。
