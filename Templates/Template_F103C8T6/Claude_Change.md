## 2026-09-29 17:16 | 将 F407 模板完整移植到 F103（SysTick 1ms/20ms）

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| AllHeader.c | ./Top/AllHeader.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| AllHeader.h | ./Top/AllHeader.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| MyISR.c | ./Top/MyISR.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| MyISR.h | ./Top/MyISR.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mymain.c | ./Top/Mymain.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mymain.h | ./Top/Mymain.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_1.c | ./Mode/Mode_1.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_1.h | ./Mode/Mode_1.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_2.c | ./Mode/Mode_2.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_2.h | ./Mode/Mode_2.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_3.c | ./Mode/Mode_3.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_3.h | ./Mode/Mode_3.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_4.c | ./Mode/Mode_4.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_4.h | ./Mode/Mode_4.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_5.c | ./Mode/Mode_5.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_5.h | ./Mode/Mode_5.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_6.c | ./Mode/Mode_6.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_6.h | ./Mode/Mode_6.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_G.c | ./Mode/Mode_G.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Mode_G.h | ./Mode/Mode_G.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Timer_Counter.c | ./Tools/Timer_Counter.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Timer_Counter.h | ./Tools/Timer_Counter.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Serial_porting.c | ./Function/Serial_porting.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Serial_porting.h | ./Function/Serial_porting.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Key.c | ./Hardware/Key.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Key.h | ./Hardware/Key.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| OLED_Data.c | ./Hardware/OLED_Data.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| OLED_Data.h | ./Hardware/OLED_Data.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| OLED.c | ./Hardware/OLED.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| OLED.h | ./Hardware/OLED.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Serial_base.c | ./Hardware/Serial_base.c | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| Serial_base.h | ./Hardware/Serial_base.h | 新增 | 移植 F407 模板的应用架构、模式或驱动，并适配 F103 |
| main.c | ./Core/Src/main.c | 修改 | 在 CubeMX 用户代码块接入 Mymain |
| stm32f1xx_it.c | ./Core/Src/stm32f1xx_it.c | 修改 | 在 SysTick 用户代码块接入 1ms/20ms 应用回调 |
| Template_F103C8T6.uvprojx | ./MDK-ARM/Template_F103C8T6.uvprojx | 修改 | 增加应用分组与头文件搜索路径 |
| README-F1.md | ./README-F1.md | 修改 | 记录两按键映射、串口及 SysTick 模板行为 |
| 模板工程构建报告.md | ./模板工程构建报告.md | 新增 | 记录移植内容、硬件映射、API 和构建验证 |
| allheader.crf | ./MDK-ARM/Template_F103C8T6/allheader.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| allheader.d | ./MDK-ARM/Template_F103C8T6/allheader.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| allheader.o | ./MDK-ARM/Template_F103C8T6/allheader.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| dma.crf | ./MDK-ARM/Template_F103C8T6/dma.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| dma.d | ./MDK-ARM/Template_F103C8T6/dma.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| dma.o | ./MDK-ARM/Template_F103C8T6/dma.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| gpio.crf | ./MDK-ARM/Template_F103C8T6/gpio.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| gpio.d | ./MDK-ARM/Template_F103C8T6/gpio.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| gpio.o | ./MDK-ARM/Template_F103C8T6/gpio.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| key.crf | ./MDK-ARM/Template_F103C8T6/key.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| key.d | ./MDK-ARM/Template_F103C8T6/key.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| key.o | ./MDK-ARM/Template_F103C8T6/key.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| main.crf | ./MDK-ARM/Template_F103C8T6/main.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| main.d | ./MDK-ARM/Template_F103C8T6/main.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| main.o | ./MDK-ARM/Template_F103C8T6/main.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_1.crf | ./MDK-ARM/Template_F103C8T6/mode_1.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_1.d | ./MDK-ARM/Template_F103C8T6/mode_1.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_1.o | ./MDK-ARM/Template_F103C8T6/mode_1.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_2.crf | ./MDK-ARM/Template_F103C8T6/mode_2.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_2.d | ./MDK-ARM/Template_F103C8T6/mode_2.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_2.o | ./MDK-ARM/Template_F103C8T6/mode_2.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_3.crf | ./MDK-ARM/Template_F103C8T6/mode_3.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_3.d | ./MDK-ARM/Template_F103C8T6/mode_3.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_3.o | ./MDK-ARM/Template_F103C8T6/mode_3.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_4.crf | ./MDK-ARM/Template_F103C8T6/mode_4.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_4.d | ./MDK-ARM/Template_F103C8T6/mode_4.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_4.o | ./MDK-ARM/Template_F103C8T6/mode_4.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_5.crf | ./MDK-ARM/Template_F103C8T6/mode_5.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_5.d | ./MDK-ARM/Template_F103C8T6/mode_5.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_5.o | ./MDK-ARM/Template_F103C8T6/mode_5.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_6.crf | ./MDK-ARM/Template_F103C8T6/mode_6.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_6.d | ./MDK-ARM/Template_F103C8T6/mode_6.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_6.o | ./MDK-ARM/Template_F103C8T6/mode_6.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_g.crf | ./MDK-ARM/Template_F103C8T6/mode_g.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_g.d | ./MDK-ARM/Template_F103C8T6/mode_g.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mode_g.o | ./MDK-ARM/Template_F103C8T6/mode_g.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| myisr.crf | ./MDK-ARM/Template_F103C8T6/myisr.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| myisr.d | ./MDK-ARM/Template_F103C8T6/myisr.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| myisr.o | ./MDK-ARM/Template_F103C8T6/myisr.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mymain.crf | ./MDK-ARM/Template_F103C8T6/mymain.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mymain.d | ./MDK-ARM/Template_F103C8T6/mymain.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| mymain.o | ./MDK-ARM/Template_F103C8T6/mymain.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled_data.crf | ./MDK-ARM/Template_F103C8T6/oled_data.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled_data.d | ./MDK-ARM/Template_F103C8T6/oled_data.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled_data.o | ./MDK-ARM/Template_F103C8T6/oled_data.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled.crf | ./MDK-ARM/Template_F103C8T6/oled.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled.d | ./MDK-ARM/Template_F103C8T6/oled.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| oled.o | ./MDK-ARM/Template_F103C8T6/oled.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_base.crf | ./MDK-ARM/Template_F103C8T6/serial_base.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_base.d | ./MDK-ARM/Template_F103C8T6/serial_base.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_base.o | ./MDK-ARM/Template_F103C8T6/serial_base.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_porting.crf | ./MDK-ARM/Template_F103C8T6/serial_porting.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_porting.d | ./MDK-ARM/Template_F103C8T6/serial_porting.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| serial_porting.o | ./MDK-ARM/Template_F103C8T6/serial_porting.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| startup_stm32f103xb.d | ./MDK-ARM/Template_F103C8T6/startup_stm32f103xb.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| startup_stm32f103xb.o | ./MDK-ARM/Template_F103C8T6/startup_stm32f103xb.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_cortex.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_cortex.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_cortex.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_cortex.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_cortex.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_cortex.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_dma.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_dma.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_dma.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_dma.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_dma.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_dma.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_exti.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_exti.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_exti.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_exti.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_exti.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_exti.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash_ex.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash_ex.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash_ex.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash_ex.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash_ex.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash_ex.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_flash.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_flash.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio_ex.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio_ex.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio_ex.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio_ex.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio_ex.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio_ex.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_gpio.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_gpio.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_msp.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_msp.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_msp.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_msp.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_msp.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_msp.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_pwr.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_pwr.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_pwr.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_pwr.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_pwr.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_pwr.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc_ex.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc_ex.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc_ex.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc_ex.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc_ex.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc_ex.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_rcc.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_rcc.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_uart.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_uart.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_uart.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_uart.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal_uart.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal_uart.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_hal.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_hal.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_it.crf | ./MDK-ARM/Template_F103C8T6/stm32f1xx_it.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_it.d | ./MDK-ARM/Template_F103C8T6/stm32f1xx_it.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| stm32f1xx_it.o | ./MDK-ARM/Template_F103C8T6/stm32f1xx_it.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| system_stm32f1xx.crf | ./MDK-ARM/Template_F103C8T6/system_stm32f1xx.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| system_stm32f1xx.d | ./MDK-ARM/Template_F103C8T6/system_stm32f1xx.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| system_stm32f1xx.o | ./MDK-ARM/Template_F103C8T6/system_stm32f1xx.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6_Template_F103C8T6.dep | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6_Template_F103C8T6.dep | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.axf | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.axf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.build_log.htm | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.build_log.htm | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.hex | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.hex | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.htm | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.htm | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.lnp | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.lnp | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.map | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.map | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| Template_F103C8T6.sct | ./MDK-ARM/Template_F103C8T6/Template_F103C8T6.sct | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| timer_counter.crf | ./MDK-ARM/Template_F103C8T6/timer_counter.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| timer_counter.d | ./MDK-ARM/Template_F103C8T6/timer_counter.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| timer_counter.o | ./MDK-ARM/Template_F103C8T6/timer_counter.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| usart.crf | ./MDK-ARM/Template_F103C8T6/usart.crf | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| usart.d | ./MDK-ARM/Template_F103C8T6/usart.d | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| usart.o | ./MDK-ARM/Template_F103C8T6/usart.o | 修改 | Keil 构建生成或更新的中间文件、映像与日志 |
| startup_stm32f103xb.lst | ./MDK-ARM/startup_stm32f103xb.lst | 修改 | Keil 汇编清单 |
