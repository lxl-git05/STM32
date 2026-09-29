## 2026-09-28 22:31 | 构建 STM32F407 模板工程

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| AllHeader.h | ./Top/AllHeader.h | 修改 | 精简统一头文件到当前模板模块 |
| AllHeader.c | ./Top/AllHeader.c | 修改 | 初始化 OLED、串口和定时器 |
| Mymain.c | ./Top/Mymain.c | 修改 | 保留模式调度并移除 AT 保存 |
| MyISR.h | ./Top/MyISR.h | 修改 | 声明统一中断接口 |
| MyISR.c | ./Top/MyISR.c | 修改 | 实现 1ms 和 20ms 回调分发 |
| Mode_G.h | ./Mode/Mode_G.h | 修改 | 收敛模式枚举 |
| Mode_G.c | ./Mode/Mode_G.c | 修改 | 实现全局初始化和 KEY0 模式切换 |
| Mode_1.h | ./Mode/Mode_1.h | 修改 | 移除旧 AT 参数声明 |
| Mode_1.c | ./Mode/Mode_1.c | 修改 | 移除非本模板硬件业务 |
| Mode_2.h | ./Mode/Mode_2.h | 修改 | 更新用途说明 |
| Mode_2.c | ./Mode/Mode_2.c | 修改 | 实现按键计算、OLED 和串口输出 |
| Mode_3.c | ./Mode/Mode_3.c | 修改 | 移除旧菜单和电机业务 |
| Mode_4.c | ./Mode/Mode_4.c | 修改 | 移除旧 IMU 和电机业务 |
| Key.h | ./Hardware/Key.h | 修改 | 按键数量改为三个 |
| Key.c | ./Hardware/Key.c | 修改 | 改为 HAL GPIO 按键读取 |
| OLED.c | ./Hardware/OLED.c | 修改 | 改为 HAL GPIO 软件 I²C |
| Serial_porting.h | ./Function/Serial_porting.h | 修改 | 简化串口输出接口 |
| Serial_porting.c | ./Function/Serial_porting.c | 修改 | 实现 USART1 调试发送 |
| main.c | ./Core/Src/main.c | 修改 | 调用 Mymain 并分发定时器回调 |
| tim.c | ./Core/Src/tim.c | 修改 | 调整 TIM6/TIM7 优先级 |
| stm32f4xx_hal_msp.c | ./Core/Src/stm32f4xx_hal_msp.c | 修改 | 调整 NVIC 优先级分组 |
| stm32f4xx_hal_conf.h | ./Core/Inc/stm32f4xx_hal_conf.h | 修改 | 启用 HAL UART |
| Template_F407ZGT6.ioc | ./Template_F407ZGT6.ioc | 修改 | 登记 USART1 和 NVIC 配置 |
| Template_F407ZGT6.uvprojx | ./MDK-ARM/Template_F407ZGT6.uvprojx | 修改 | 登记模板源文件和 HAL UART 驱动 |
| README-F4.md | ./README-F4.md | 修改 | 补充串口引脚 |
| 模板工程构建报告.md | ./模板工程构建报告.md | 新增 | 记录构建细节与验证结果 |

## 2026-09-28 23:01 | 调整 Keil 分组并消除编译警告

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| Template_F407ZGT6.uvprojx | ./MDK-ARM/Template_F407ZGT6.uvprojx | 修改 | 按 Top、Mode、Tools、Function、Hardware 分组登记 C/头文件 |
| Timer_Counter.h | ./Tools/Timer_Counter.h | 修改 | 去除 MySystem 头文件依赖以便 Tools 编译 |
| AllHeader.h | ./Top/AllHeader.h | 修改 | 补齐文件末尾换行 |
| MyISR.h | ./Top/MyISR.h | 修改 | 补齐文件末尾换行 |
| Mode_G.h | ./Mode/Mode_G.h | 修改 | 补齐文件末尾换行 |
| Mode_1.c | ./Mode/Mode_1.c | 修改 | 补齐文件末尾换行 |
| Mode_2.c | ./Mode/Mode_2.c | 修改 | 补齐文件末尾换行 |
| Mode_3.c | ./Mode/Mode_3.c | 修改 | 补齐文件末尾换行 |
| Mode_4.c | ./Mode/Mode_4.c | 修改 | 补齐文件末尾换行 |
| Serial_porting.h | ./Function/Serial_porting.h | 修改 | 补齐文件末尾换行 |
| 模板工程构建报告.md | ./模板工程构建报告.md | 修改 | 更新 Keil 分组和 ARMCC 构建验证结果 |
| allheader.crf | ./MDK-ARM/Template_F407ZGT6/allheader.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| allheader.d | ./MDK-ARM/Template_F407ZGT6/allheader.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| allheader.o | ./MDK-ARM/Template_F407ZGT6/allheader.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| gpio.crf | ./MDK-ARM/Template_F407ZGT6/gpio.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| gpio.d | ./MDK-ARM/Template_F407ZGT6/gpio.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| gpio.o | ./MDK-ARM/Template_F407ZGT6/gpio.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| key.crf | ./MDK-ARM/Template_F407ZGT6/key.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| key.d | ./MDK-ARM/Template_F407ZGT6/key.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| key.o | ./MDK-ARM/Template_F407ZGT6/key.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| main.crf | ./MDK-ARM/Template_F407ZGT6/main.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| main.d | ./MDK-ARM/Template_F407ZGT6/main.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| main.o | ./MDK-ARM/Template_F407ZGT6/main.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_1.crf | ./MDK-ARM/Template_F407ZGT6/mode_1.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_1.d | ./MDK-ARM/Template_F407ZGT6/mode_1.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_1.o | ./MDK-ARM/Template_F407ZGT6/mode_1.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_2.crf | ./MDK-ARM/Template_F407ZGT6/mode_2.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_2.d | ./MDK-ARM/Template_F407ZGT6/mode_2.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_2.o | ./MDK-ARM/Template_F407ZGT6/mode_2.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_3.crf | ./MDK-ARM/Template_F407ZGT6/mode_3.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_3.d | ./MDK-ARM/Template_F407ZGT6/mode_3.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_3.o | ./MDK-ARM/Template_F407ZGT6/mode_3.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_4.crf | ./MDK-ARM/Template_F407ZGT6/mode_4.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_4.d | ./MDK-ARM/Template_F407ZGT6/mode_4.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_4.o | ./MDK-ARM/Template_F407ZGT6/mode_4.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_5.crf | ./MDK-ARM/Template_F407ZGT6/mode_5.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_5.d | ./MDK-ARM/Template_F407ZGT6/mode_5.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_5.o | ./MDK-ARM/Template_F407ZGT6/mode_5.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_6.crf | ./MDK-ARM/Template_F407ZGT6/mode_6.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_6.d | ./MDK-ARM/Template_F407ZGT6/mode_6.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_6.o | ./MDK-ARM/Template_F407ZGT6/mode_6.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_g.crf | ./MDK-ARM/Template_F407ZGT6/mode_g.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_g.d | ./MDK-ARM/Template_F407ZGT6/mode_g.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mode_g.o | ./MDK-ARM/Template_F407ZGT6/mode_g.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| myisr.crf | ./MDK-ARM/Template_F407ZGT6/myisr.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| myisr.d | ./MDK-ARM/Template_F407ZGT6/myisr.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| myisr.o | ./MDK-ARM/Template_F407ZGT6/myisr.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mymain.crf | ./MDK-ARM/Template_F407ZGT6/mymain.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mymain.d | ./MDK-ARM/Template_F407ZGT6/mymain.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| mymain.o | ./MDK-ARM/Template_F407ZGT6/mymain.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled_data.crf | ./MDK-ARM/Template_F407ZGT6/oled_data.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled_data.d | ./MDK-ARM/Template_F407ZGT6/oled_data.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled_data.o | ./MDK-ARM/Template_F407ZGT6/oled_data.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled.crf | ./MDK-ARM/Template_F407ZGT6/oled.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled.d | ./MDK-ARM/Template_F407ZGT6/oled.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| oled.o | ./MDK-ARM/Template_F407ZGT6/oled.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| serial_porting.crf | ./MDK-ARM/Template_F407ZGT6/serial_porting.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| serial_porting.d | ./MDK-ARM/Template_F407ZGT6/serial_porting.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| serial_porting.o | ./MDK-ARM/Template_F407ZGT6/serial_porting.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_cortex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_cortex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_cortex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_cortex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_cortex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_cortex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma_ex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma_ex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma_ex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma_ex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma_ex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma_ex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_dma.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_dma.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_exti.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_exti.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_exti.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_exti.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_exti.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_exti.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ramfunc.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ramfunc.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ramfunc.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ramfunc.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash_ramfunc.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash_ramfunc.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_flash.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_flash.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_gpio.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_gpio.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_gpio.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_gpio.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_gpio.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_gpio.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_msp.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_msp.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_msp.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_msp.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_msp.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_msp.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr_ex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr_ex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr_ex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr_ex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr_ex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr_ex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_pwr.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_pwr.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc_ex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc_ex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc_ex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc_ex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc_ex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc_ex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_rcc.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_rcc.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim_ex.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim_ex.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim_ex.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim_ex.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim_ex.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim_ex.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_tim.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_tim.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_timebase_tim.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_timebase_tim.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_timebase_tim.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_timebase_tim.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_timebase_tim.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_timebase_tim.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_uart.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_uart.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_uart.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_uart.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal_uart.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal_uart.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_hal.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_hal.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_it.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_it.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| stm32f4xx_it.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| system_stm32f4xx.crf | ./MDK-ARM/Template_F407ZGT6/system_stm32f4xx.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| system_stm32f4xx.d | ./MDK-ARM/Template_F407ZGT6/system_stm32f4xx.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| system_stm32f4xx.o | ./MDK-ARM/Template_F407ZGT6/system_stm32f4xx.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6_Template_F407ZGT6.dep | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6_Template_F407ZGT6.dep | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.axf | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.axf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.build_log.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.build_log.htm | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.hex | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.hex | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.htm | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.lnp | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.lnp | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| Template_F407ZGT6.map | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.map | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| tim.crf | ./MDK-ARM/Template_F407ZGT6/tim.crf | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| tim.d | ./MDK-ARM/Template_F407ZGT6/tim.d | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| tim.o | ./MDK-ARM/Template_F407ZGT6/tim.o | 修改 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| timer_counter.crf | ./MDK-ARM/Template_F407ZGT6/timer_counter.crf | 新增 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| timer_counter.d | ./MDK-ARM/Template_F407ZGT6/timer_counter.d | 新增 | Keil ARMCC V5 重新构建产生的目标或中间文件 |
| timer_counter.o | ./MDK-ARM/Template_F407ZGT6/timer_counter.o | 新增 | Keil ARMCC V5 重新构建产生的目标或中间文件 |

## 2026-09-28 23:45 | 完善 Serial1/Serial2 和串口 API

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| Serial_porting.h | ./Function/Serial_porting.h | 修改 | 恢复参考工程全部公开 Serial API 并声明 Serial1/Serial2 |
| Serial_porting.c | ./Function/Serial_porting.c | 修改 | 配置两路 USART、RX DMA、ABC/HEX 解析及完整发送接口 |
| Serial_base.h | ./Hardware/Serial_base.h | 修改 | 移除 MySystem 依赖并为中断标志增加 volatile |
| Mode_2.c | ./Mode/Mode_2.c | 修改 | 改用参考接口向 Serial1 打印 num |
| MyISR.h | ./Top/MyISR.h | 修改 | 声明 USART 和 DMA 统一中断入口 |
| MyISR.c | ./Top/MyISR.c | 修改 | 集中转发 USART/DMA 与 UART 接收回调 |
| stm32f4xx_it.h | ./Core/Inc/stm32f4xx_it.h | 修改 | 声明 USART1/2 与两路 DMA 中断 |
| stm32f4xx_it.c | ./Core/Src/stm32f4xx_it.c | 修改 | 接入 USART1/2 与两路 DMA 中断处理 |
| Template_F407ZGT6.ioc | ./Template_F407ZGT6.ioc | 修改 | 登记 USART2 PA2/PA3 和两路 RX DMA 配置 |
| Template_F407ZGT6.uvprojx | ./MDK-ARM/Template_F407ZGT6.uvprojx | 修改 | Hardware 分组加入 Serial_base.c/.h |
| README-F4.md | ./README-F4.md | 修改 | 补充 Serial1/Serial2 接收引脚与配置 |
| 模板工程构建报告.md | ./模板工程构建报告.md | 修改 | 记录双串口、完整 API 和构建结果 |
| allheader.crf | ./MDK-ARM/Template_F407ZGT6/allheader.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| allheader.d | ./MDK-ARM/Template_F407ZGT6/allheader.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| allheader.o | ./MDK-ARM/Template_F407ZGT6/allheader.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| main.crf | ./MDK-ARM/Template_F407ZGT6/main.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| main.d | ./MDK-ARM/Template_F407ZGT6/main.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| main.o | ./MDK-ARM/Template_F407ZGT6/main.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_1.crf | ./MDK-ARM/Template_F407ZGT6/mode_1.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_1.d | ./MDK-ARM/Template_F407ZGT6/mode_1.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_1.o | ./MDK-ARM/Template_F407ZGT6/mode_1.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_2.crf | ./MDK-ARM/Template_F407ZGT6/mode_2.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_2.d | ./MDK-ARM/Template_F407ZGT6/mode_2.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_2.o | ./MDK-ARM/Template_F407ZGT6/mode_2.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_3.crf | ./MDK-ARM/Template_F407ZGT6/mode_3.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_3.d | ./MDK-ARM/Template_F407ZGT6/mode_3.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_3.o | ./MDK-ARM/Template_F407ZGT6/mode_3.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_4.crf | ./MDK-ARM/Template_F407ZGT6/mode_4.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_4.d | ./MDK-ARM/Template_F407ZGT6/mode_4.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_4.o | ./MDK-ARM/Template_F407ZGT6/mode_4.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_5.crf | ./MDK-ARM/Template_F407ZGT6/mode_5.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_5.d | ./MDK-ARM/Template_F407ZGT6/mode_5.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_5.o | ./MDK-ARM/Template_F407ZGT6/mode_5.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_6.crf | ./MDK-ARM/Template_F407ZGT6/mode_6.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_6.d | ./MDK-ARM/Template_F407ZGT6/mode_6.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_6.o | ./MDK-ARM/Template_F407ZGT6/mode_6.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_g.crf | ./MDK-ARM/Template_F407ZGT6/mode_g.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_g.d | ./MDK-ARM/Template_F407ZGT6/mode_g.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mode_g.o | ./MDK-ARM/Template_F407ZGT6/mode_g.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| myisr.crf | ./MDK-ARM/Template_F407ZGT6/myisr.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| myisr.d | ./MDK-ARM/Template_F407ZGT6/myisr.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| myisr.o | ./MDK-ARM/Template_F407ZGT6/myisr.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mymain.crf | ./MDK-ARM/Template_F407ZGT6/mymain.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mymain.d | ./MDK-ARM/Template_F407ZGT6/mymain.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| mymain.o | ./MDK-ARM/Template_F407ZGT6/mymain.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_base.crf | ./MDK-ARM/Template_F407ZGT6/serial_base.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_base.d | ./MDK-ARM/Template_F407ZGT6/serial_base.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_base.o | ./MDK-ARM/Template_F407ZGT6/serial_base.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_porting.crf | ./MDK-ARM/Template_F407ZGT6/serial_porting.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_porting.d | ./MDK-ARM/Template_F407ZGT6/serial_porting.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| serial_porting.o | ./MDK-ARM/Template_F407ZGT6/serial_porting.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| stm32f4xx_it.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.crf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| stm32f4xx_it.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.d | 修改 | Keil ARMCC V5 串口改造构建产物 |
| stm32f4xx_it.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.o | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6_Template_F407ZGT6.dep | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6_Template_F407ZGT6.dep | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.axf | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.axf | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.build_log.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.build_log.htm | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.hex | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.hex | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.htm | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.lnp | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.lnp | 修改 | Keil ARMCC V5 串口改造构建产物 |
| Template_F407ZGT6.map | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.map | 修改 | Keil ARMCC V5 串口改造构建产物 |

## 2026-09-29 12:30 | 使用 CubeMX 串口配置并修复重复中断

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| Serial_porting.c | ./Function/Serial_porting.c | 修改 | 改用 CubeMX huart1/huart2 并删除手写 GPIO/UART/DMA/NVIC 初始化 |
| Serial_porting.h | ./Function/Serial_porting.h | 修改 | 移除重复 IRQ 辅助接口声明，保留模板串口 API |
| MyISR.c | ./Top/MyISR.c | 修改 | 删除重复串口 IRQ 转发，保留 UART 接收和错误回调 |
| MyISR.h | ./Top/MyISR.h | 修改 | 移除不再使用的重复 IRQ 接口声明 |
| stm32f4xx_it.c | ./Core/Src/stm32f4xx_it.c | 修改 | 删除 USER CODE 中与 CubeMX 生成代码重复的 4 个 IRQ |
| 模板工程构建报告.md | ./模板工程构建报告.md | 修改 | 记录 CubeMX 句柄接管与 ARMCC 构建验证 |
| allheader.crf | ./MDK-ARM/Template_F407ZGT6/allheader.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| allheader.d | ./MDK-ARM/Template_F407ZGT6/allheader.d | 修改 | Keil ARMCC V5 验证构建产物 |
| allheader.o | ./MDK-ARM/Template_F407ZGT6/allheader.o | 修改 | Keil ARMCC V5 验证构建产物 |
| main.crf | ./MDK-ARM/Template_F407ZGT6/main.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| main.d | ./MDK-ARM/Template_F407ZGT6/main.d | 修改 | Keil ARMCC V5 验证构建产物 |
| main.o | ./MDK-ARM/Template_F407ZGT6/main.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_1.crf | ./MDK-ARM/Template_F407ZGT6/mode_1.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_1.d | ./MDK-ARM/Template_F407ZGT6/mode_1.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_1.o | ./MDK-ARM/Template_F407ZGT6/mode_1.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_2.crf | ./MDK-ARM/Template_F407ZGT6/mode_2.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_2.d | ./MDK-ARM/Template_F407ZGT6/mode_2.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_2.o | ./MDK-ARM/Template_F407ZGT6/mode_2.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_3.crf | ./MDK-ARM/Template_F407ZGT6/mode_3.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_3.d | ./MDK-ARM/Template_F407ZGT6/mode_3.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_3.o | ./MDK-ARM/Template_F407ZGT6/mode_3.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_4.crf | ./MDK-ARM/Template_F407ZGT6/mode_4.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_4.d | ./MDK-ARM/Template_F407ZGT6/mode_4.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_4.o | ./MDK-ARM/Template_F407ZGT6/mode_4.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_5.crf | ./MDK-ARM/Template_F407ZGT6/mode_5.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_5.d | ./MDK-ARM/Template_F407ZGT6/mode_5.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_5.o | ./MDK-ARM/Template_F407ZGT6/mode_5.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_6.crf | ./MDK-ARM/Template_F407ZGT6/mode_6.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_6.d | ./MDK-ARM/Template_F407ZGT6/mode_6.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_6.o | ./MDK-ARM/Template_F407ZGT6/mode_6.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_g.crf | ./MDK-ARM/Template_F407ZGT6/mode_g.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_g.d | ./MDK-ARM/Template_F407ZGT6/mode_g.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mode_g.o | ./MDK-ARM/Template_F407ZGT6/mode_g.o | 修改 | Keil ARMCC V5 验证构建产物 |
| myisr.crf | ./MDK-ARM/Template_F407ZGT6/myisr.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| myisr.d | ./MDK-ARM/Template_F407ZGT6/myisr.d | 修改 | Keil ARMCC V5 验证构建产物 |
| myisr.o | ./MDK-ARM/Template_F407ZGT6/myisr.o | 修改 | Keil ARMCC V5 验证构建产物 |
| mymain.crf | ./MDK-ARM/Template_F407ZGT6/mymain.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| mymain.d | ./MDK-ARM/Template_F407ZGT6/mymain.d | 修改 | Keil ARMCC V5 验证构建产物 |
| mymain.o | ./MDK-ARM/Template_F407ZGT6/mymain.o | 修改 | Keil ARMCC V5 验证构建产物 |
| serial_porting.crf | ./MDK-ARM/Template_F407ZGT6/serial_porting.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| serial_porting.d | ./MDK-ARM/Template_F407ZGT6/serial_porting.d | 修改 | Keil ARMCC V5 验证构建产物 |
| serial_porting.o | ./MDK-ARM/Template_F407ZGT6/serial_porting.o | 修改 | Keil ARMCC V5 验证构建产物 |
| stm32f4xx_it.crf | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.crf | 修改 | Keil ARMCC V5 验证构建产物 |
| stm32f4xx_it.d | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.d | 修改 | Keil ARMCC V5 验证构建产物 |
| stm32f4xx_it.o | ./MDK-ARM/Template_F407ZGT6/stm32f4xx_it.o | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6_Template_F407ZGT6.dep | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6_Template_F407ZGT6.dep | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.axf | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.axf | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.build_log.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.build_log.htm | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.hex | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.hex | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.htm | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.htm | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.lnp | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.lnp | 修改 | Keil ARMCC V5 验证构建产物 |
| Template_F407ZGT6.map | ./MDK-ARM/Template_F407ZGT6/Template_F407ZGT6.map | 修改 | Keil ARMCC V5 验证构建产物 |
