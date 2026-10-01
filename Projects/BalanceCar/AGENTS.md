# 工程 RTOS API 约定

- 后续新增或修改的所有用户 RTOS 代码统一使用 FreeRTOS 原生 API，例如 xTaskCreate、vTaskDelay、vTaskDelete、xQueueCreate、xSemaphoreCreateMutex、xEventGroupCreate 和 xTimerCreate；不使用 CMSIS-RTOS 的 osXXX API。
- 用户任务句柄使用 TaskHandle_t；共享 RTOS 句柄只在 User/Msg/Msg.c 定义一次，在 Msg.h 中 extern 声明。
- xTaskCreate 的栈深度单位为 StackType_t；毫秒延时使用 pdMS_TO_TICKS 转换；任务创建结果必须检查。
- 保持 CubeMX 可重新生成：main.c、freertos.c 只修改 USER CODE 区域。CubeMX 自动生成的内核启动与默认任务创建可保留 CMSIS 封装，用户启动任务内部使用原生 API。
- 保留现有格式和注释，仅修改必要内容。其他用户提供的环境、文件边界及 Claude_Change.md 记录规则继续适用。
