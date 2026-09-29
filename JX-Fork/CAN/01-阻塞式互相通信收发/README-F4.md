# Template-F4

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
  + 时基定时器选择TIM13

# 2. 项目硬件配置

## 2-1 基础配置

**APB1（时钟 84 MHz）,这些定时器的 定时器时钟频率 = ==84 MHz==。**

**TIM2 , TIM3 , TIM4 , TIM5 , TIM6 , TIM7 , TIM12 , TIM13 , TIM14**

**APB2（时钟 168 MHz）,这些定时器的 定时器时钟频率 = ==168 MHz==。**

**TIM1 （高级定时器） ,TIM8 （高级定时器） , TIM9 , TIM10 , TIM11**

| 引脚号 |  Label   |           备注           |
| :----: | :------: | :----------------------: |
|  PB5   |   LED0   |         板载 LED         |
|  PE4   |   KEY0   |        板载 按键0        |
|  PF4   |   KEY1   | 外置 按键2 上拉（按键1） |
|  PF6   |   KEY2   | 外置 按键3 上拉（按键2） |
|  PB8   | OLED_SCL |         软件OLED         |
|  PB9   | OLED_SDA |         软件OLED         |
|  PA9   | USART1_TX | 串口调试发送（115200） |
|  PA10  | USART1_RX | Serial1 接收（DMA + 空闲检测） |
|  PA2   | USART2_TX | Serial2 发送（115200） |
|  PA3   | USART2_RX | Serial2 接收（DMA + 空闲检测） |

## 2-2 定时器

| 定时器 | 更新中断 |
| :----: | :------: |
|  TIM6  |   1ms    |
|  TIM7  |   20ms   |



