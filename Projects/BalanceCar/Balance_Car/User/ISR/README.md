# ISR 中断管理层

路径：硬件 IRQ → stm32f1xx_it.c → HAL_xxx_IRQHandler → ISR.c 中的 HAL Callback → Hardware 短处理 / Msg / Task 通知。

stm32f1xx_it.c 和 HAL Driver 保持原样。ISR.h 只包含接口所需的 HAL 类型头文件，不包含 AllHeader.h。

## 当前入口

| HAL 回调 | 处理位置 | 用途 |
|---|---|---|
| HAL_TIM_PeriodElapsedCallback | ISR_TIM_Handler | main.c USER CODE 转发，保留 TIM4 时基 |
| HAL_UART_RxCpltCallback | ISR.c 中直接处理 | USART1/2/3 中断或 DMA 接收完成 |
| HAL_UART_TxCpltCallback | ISR.c 中直接处理 | UART 中断或 DMA 发送完成 |
| HAL_UART_ErrorCallback | ISR.c 中直接处理 | UART 及其 DMA 错误分发 |
| HAL_CAN_RxFifo0MsgPendingCallback | ISR.c 中直接处理 | CAN FIFO0 接收通知 |

上述用户分发目前没有实际通信业务。UART/DMA 已配置 IRQ，CAN RX0 已配置 IRQ，但当前尚未启动 UART 收发或 CAN 接收通知。本层不启动外设、不创建消息资源、不添加虚构任务通知。

GPIO 按键当前为普通输入；TIM1/2、SPI、I2C、ADC 未启用相应 IRQ，暂不覆盖这些 HAL 弱回调。以后确实使用 EXTI、输出比较、DMA 半完成、UART ReceiveToIdle、SPI/I2C/ADC 中断时，再在 ISR.c 添加必要 HAL 回调。简单分支直接放在回调内，不额外增加同文件转发函数；ISR.h 只声明需要跨文件调用的接口。

## TIM4 时基例外

CubeMX 在 main.c 的 USER CODE 区域外生成 HAL_TIM_PeriodElapsedCallback 和 HAL_IncTick。保留唯一的生成回调，在 Callback 1 区域调用 ISR_TIM_Handler；ISR.c 不再次定义该官方回调。TIM4 在这里直接返回，不再调用 HAL_IncTick，避免重复计时。这样重新 Generate Code 后仍只有一个强定义。

TIM4 在调度器启动前就工作，当前 HAL tick 中断优先级为 15，满足 FreeRTOS 的优先级限制，但调度器和消息对象尚未就绪时不能调用 FromISR API。本框架的 TIM4 分支仅保留时基，不发送用户任务通知。

## ISR 中通信

- 用户 RTOS 操作统一使用原生 xQueueSendFromISR、xSemaphoreGiveFromISR、xTaskNotifyFromISR、vTaskNotifyGiveFromISR、xEventGroupSetBitsFromISR 等相应 API。
- 先确认 Msg 中对象已创建、任务句柄有效，并确认产生该回调的实际中断优先级允许调用 FreeRTOS。USART1/2/3、其 DMA 和 CAN RX0 当前优先级均为 5，configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 为 5。
- 使用 BaseType_t xHigherPriorityTaskWoken = pdFALSE，调用后使用 portYIELD_FROM_ISR(xHigherPriorityTaskWoken)。检查队列满等失败情况，避免无界重试。
- xEventGroupSetBitsFromISR 依赖软件定时器守护任务和命令队列，设置是延后执行的，必须检查返回值。
- UART DMA 回调由 HAL 内部转发到 UART 回调，不要改写 HAL 管理的 DMA XferCpltCallback/XferErrorCallback 指针。当前 DMA 为 NORMAL 模式，不添加无实际需求的半完成回调。
- 启用 CAN FIFO0 消息通知之前，必须实现有界的 FIFO 读取及保存，或关闭通知并让任务读取后重新开启；不能只通知任务而让 FIFO 一直保持 pending，导致中断持续重入。
- HAL 回调也可能由轮询或同步 abort 等非中断路径触发。新增回调时核实调用上下文，不能仅凭 Callback 名称就调用 FromISR。

ISR 只保存必要数据并通知任务。不要执行 OLED 刷新、大量 printf、复杂 PID、长循环、阻塞 HAL 通信、vTaskDelay 或普通 xQueueSend/xSemaphoreGive。复杂处理转移到 Hardware 短函数或任务。

## 增加新回调

1. 搜索工程中的同名 HAL Callback，确认已有强定义的位置；不要修改 HAL 的 __weak 实现。
2. 在 CubeMX 配置对应中断、NVIC 优先级及外设模式。
3. 在 ISR.c 唯一定义所需官方回调，直接完成简单分支和短处理。只有跨文件接口或处理复杂到需要独立模块时才增加分发函数；跨文件接口在 ISR.h 声明。
4. 按 Instance 或 GPIO_Pin 分发，再调用短小 Hardware 函数或安全的 FromISR 通知。
5. 所有共享 RTOS 句柄仍在 Msg.c 定义，Msg.h extern 声明；不得在 ISR.c 再次定义。
6. 运行 User/Sync_Keil.ps1 更新工程，并检查重复回调和实际 IRQ 调用路径。
