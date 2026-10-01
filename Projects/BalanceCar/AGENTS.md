# 工程 RTOS API 约定

- 移植参考驱动时保留原有功能与必要接口；去掉抽象层不等于删除标定、恢复或调试功能。平台不兼容而未移植的能力及接口变更必须明确列出，不能以精简为由默默裁剪。

- 后续新增或修改的所有用户 RTOS 代码统一使用 FreeRTOS 原生 API，例如 xTaskCreate、vTaskDelay、vTaskDelete、xQueueCreate、xSemaphoreCreateMutex、xEventGroupCreate 和 xTimerCreate；不使用 CMSIS-RTOS 的 osXXX API。
- Msg 只管理通信结构和通信资源；任务句柄使用 TaskHandle_t，在所属 xxx_Task.c 中唯一定义并创建，在 xxx_Task.h 中 extern 声明。其他文件需要任务通知时包含对应任务头文件。队列、信号量、互斥锁等共享通信资源在 Msg.c 定义，Msg.h 声明。
- xTaskCreate 的栈深度单位为 StackType_t；毫秒延时使用 pdMS_TO_TICKS 转换；任务创建结果必须检查。
- 保持 CubeMX 可重新生成：main.c、freertos.c 只修改 USER CODE 区域。CubeMX 自动生成的内核启动与默认任务创建可保留 CMSIS 封装，用户启动任务内部使用原生 API。
- 保留现有格式和注释，仅修改必要内容。其他用户提供的环境、文件边界及 Claude_Change.md 记录规则继续适用。
- 用户 HAL 回调集中在 User/ISR/ISR.c，只做短小分发；中断通信使用原生 FromISR API。IRQHandler 保留在 stm32f1xx_it.c，HAL Driver 不修改。CubeMX 在 main.c 生成的 TIM4 时基回调通过 USER CODE 转发到 ISR，不重复定义或重复调用 HAL_IncTick。
- 用户 HAL 回调集中在 User/ISR/ISR.c，只做短小分发；中断通信使用原生 FromISR API。IRQHandler 保留在 stm32f1xx_it.c，HAL Driver 不修改。CubeMX 在 main.c 生成的 TIM4 时基回调通过 USER CODE 转发到 ISR，不重复定义或重复调用 HAL_IncTick。

- ISR.c 中简单的实例分支直接写在 HAL Callback 内，不增加同文件的一对一转发函数。只有跨文件调用或复杂模块处理才抽取接口；TIM4 的 CubeMX 生成回调继续保留必要的跨文件转发。
