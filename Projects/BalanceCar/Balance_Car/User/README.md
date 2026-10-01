# User 基础框架

User 位于 CubeMX 项目 Balance_Car 内，与 Core、Drivers、Middlewares 同级。

启动顺序：main → HAL/时钟 → MX_xxx_Init → Hardware_Init → osKernelInitialize → MX_FREERTOS_Init → osKernelStart → StartDefaultTask → Msg_Init → Task_Init → vTaskDelete(NULL) → App_Task。

后续所有用户 RTOS 代码统一使用 FreeRTOS 原生 API（当前内核 10.3.1）：xTaskCreate、vTaskDelay、vTaskDelete、xQueueCreate 等，不使用 CMSIS-RTOS 的 osXXX API。App_Task 用 vTaskDelay(pdMS_TO_TICKS(1000U)) 延时一秒；xTaskCreate 的栈深度单位是 StackType_t，不是字节。App_Task 栈深度为 128（本平台 512 字节），优先级保持为 24，创建失败进入 Error_Handler。CubeMX 自动生成的默认任务创建及内核启动仍保留 CMSIS 封装，以保持重新生成兼容；不要修改 USER CODE 外的生成代码。

## 层次职责

| 目录 | 职责 |
|---|---|
| Initial | Hardware_Init、Task_Init 统一入口；Initial.h 同时声明 Msg_Init |
| AllHeader | 聚合公共头文件，只供 .c 按需使用 |
| ISR | HAL 回调分发、短小硬件处理及 FreeRTOS FromISR 通知 |
| Task | 创建任务、周期与调度，调用 Function |
| Msg | 在 Msg.c 唯一定义用户共享 RTOS 句柄，Msg.h extern 声明；Msg_Init 创建通信对象 |
| Hardware | 设备驱动及硬件协议，调用 HAL |
| Software | 独立于硬件的算法 |
| Function | 组合 Hardware 与 Software 实现业务功能 |

推荐依赖为 Task → Function → Hardware/Software，Hardware → HAL。Msg 是任务间通信资源；模块头文件不得包含 AllHeader.h。

## 添加模块

1. Task：添加 Task/xxx_Task.c、xxx_Task.h；提供 xxx_Task_Init 和线程入口，在 Initial.c 的 Task_Init 中按依赖顺序调用。共享任务句柄在 Msg.c 定义，Msg.h extern 声明。线程创建失败必须处理。
2. Hardware：添加 Hardware/设备名/设备名.c 和 .h；在 Hardware_Init 中调用真实设备初始化。此时调度器尚未启动，不使用 vTaskDelay、RTOS 同步对象或依赖任务的驱动。需要 RTOS 的设备启动步骤放到启动任务中，并在业务任务创建前完成。
3. Software：添加 Software/算法名/算法名.c 和 .h；尽量只依赖标准 C 类型，向 Function 提供纯算法接口。
4. Function：添加 Function/功能名/功能名.c 和 .h；组合驱动与算法，由 Task 周期调用。若需要初始化，在使用它的业务任务启动前完成。
5. Msg：按需要添加 QueueHandle_t、SemaphoreHandle_t（信号量及互斥锁）、EventGroupHandle_t、TimerHandle_t 等句柄，在 Msg_Init 使用 xQueueCreate、xSemaphoreCreateMutex、xEventGroupCreate、xTimerCreate 等原生 API 创建并检查返回值。任务通知使用 Msg 中的 TaskHandle_t；任务句柄在 Task_Init 时赋值，不在 Msg_Init 创建任务。Msg.h 按实际类型 include queue.h、semphr.h、event_groups.h 或 timers.h。
6. 按需要将新模块头文件加入 AllHeader.h；每个模块 .h 只 include 自身依赖，并使用唯一 include guard。

## Keil 与 CubeMX 再生成

当前八个目录都已加入 Keil Include Path 和 Group。添加模块子目录或重新 Generate Code 后，关闭 Keil 工程，在本工作区运行：

```powershell
powershell -NoProfile -File .\Balance_Car\User\Sync_Keil.ps1
```

脚本扫描 User 各层的 .c/.h 和子目录，更新单个 Keil target 的分组及包含路径，可重复运行。User/* 分组由脚本管理，请不要在其中放入 User 外部文件。CubeMX 的 KeepUserCode 已开启；main.c 和 freertos.c 的入口均位于 USER CODE 区域。脚本未绑定到 CubeMX 钩子，Generate Code 后需手动执行并检查构建。

CubeMX 自己的 defaultTaskHandle 保持由生成代码管理；所有用户共享 RTOS 句柄放入 Msg。Hardware_Init 和 Msg_Init 当前为空框架，不包含占位驱动调用，也不创建无实际需求的通信对象。

FreeRTOS 当前动态堆只有 3072 字节，使用 heap_4。以后增加任务、队列和软件定时器时，在 CubeMX 中调整堆、栈及 RTOS 配置，检查内存和创建结果。默认任务退出后内存由 idle task 回收。
