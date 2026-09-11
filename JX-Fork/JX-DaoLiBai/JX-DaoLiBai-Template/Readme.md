# JX-DaoLiBai-Template

主控：STM32F103C8Tx（LQFP48）

## 当前引脚配置

| 引脚 | 标签 / 外设功能 | 配置模式 | 上下拉 / 初始状态 | 用途说明 |
|------|-----------------|----------|-------------------|----------|
| PA9 | USART1_TX | 复用推挽输出，高速 | 无上下拉 | Serial1 串口发送，波特率 115200 |
| PA10 | USART1_RX | 异步串口输入 | 无上下拉 | Serial1 串口接收，使用 DMA + 空闲中断 |
| PA11 | KEY3 | GPIO 输入 | 上拉 | 按键 3，低电平按下 |
| PA12 | KEY4 | GPIO 输入 | 上拉 | 按键 4，低电平按下 |
| PA13 | SWDIO | Serial Wire 调试 | — | SWD 数据线 |
| PA14 | SWCLK | Serial Wire 调试 | — | SWD 时钟线 |
| PB8 | OLED_SCL | GPIO 推挽输出，高速 | 初始低电平 | OLED 软件时序时钟线 |
| PB9 | OLED_SDA | GPIO 推挽输出，高速 | 初始低电平 | OLED 软件时序数据线 |
| PB10 | KEY1 | GPIO 输入 | 上拉 | 按键 1，低电平按下 |
| PB11 | KEY2 | GPIO 输入 | 上拉 | 按键 2，低电平按下 |
| PC13 | LED0 | GPIO 推挽输出，低速 | 初始低电平 | 板载 LED |
| PC14 | OSC32_IN | LSE 外部晶振 | — | 32.768 kHz 低速外部晶振输入 |
| PC15 | OSC32_OUT | LSE 外部晶振 | — | 32.768 kHz 低速外部晶振输出 |
| PD0 | OSC_IN | HSE 外部晶振 | — | 高速外部晶振输入 |
| PD1 | OSC_OUT | HSE 外部晶振 | — | 高速外部晶振输出 |

> 引脚信息依据 `JX-DaoLiBai-Template.ioc`、`Core/Src/gpio.c`、`Core/Src/usart.c` 与 `Core/Inc/main.h` 整理。SysTick 为内部虚拟资源，不占用物理引脚。
