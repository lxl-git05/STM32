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
| Msg | 管理通信结构和通信资源，Msg_Init 创建通信对象；不存放任务句柄 |
| Hardware | 设备驱动及硬件协议，调用 HAL |
| Software | 独立于硬件的算法 |
| Function | 组合 Hardware 与 Software 实现业务功能 |
| Tools | DWT 代码耗时与调用间隔计时 |

推荐依赖为 Task → Function → Hardware/Software，Hardware → HAL。Msg 是任务间通信资源；模块头文件不得包含 AllHeader.h。

## 添加模块

1. Task：添加 Task/xxx_Task.c、xxx_Task.h；提供 xxx_Task_Init 和线程入口，在 Initial.c 的 Task_Init 中按依赖顺序调用。任务句柄在所属 xxx_Task.c 定义，xxx_Task.h extern 声明，由该任务创建函数赋值。线程创建失败必须处理。
2. Hardware：添加 Hardware/设备名/设备名.c 和 .h；在 Hardware_Init 中调用真实设备初始化。此时调度器尚未启动，不使用 vTaskDelay、RTOS 同步对象或依赖任务的驱动。需要 RTOS 的设备启动步骤放到启动任务中，并在业务任务创建前完成。
3. Software：添加 Software/算法名/算法名.c 和 .h；尽量只依赖标准 C 类型，向 Function 提供纯算法接口。
4. Function：添加 Function/功能名/功能名.c 和 .h；组合驱动与算法，由 Task 周期调用。若需要初始化，在使用它的业务任务启动前完成。
5. Msg：按需要添加 QueueHandle_t、SemaphoreHandle_t（信号量及互斥锁）、EventGroupHandle_t、TimerHandle_t 等句柄，在 Msg_Init 使用 xQueueCreate、xSemaphoreCreateMutex、xEventGroupCreate、xTimerCreate 等原生 API 创建并检查返回值。任务通知包含对应 xxx_Task.h，使用该任务声明的 TaskHandle_t；Msg 不管理任务句柄或创建任务。Msg.h 按实际类型 include queue.h、semphr.h、event_groups.h 或 timers.h。
6. 按需要将新模块头文件加入 AllHeader.h；每个模块 .h 只 include 自身依赖，并使用唯一 include guard。

## Keil 与 CubeMX 再生成

当前九个目录（含 Tools）都已加入 Keil Include Path 和 Group。添加模块子目录或重新 Generate Code 后，关闭 Keil 工程，在本工作区运行：

```powershell
powershell -NoProfile -File .\Balance_Car\User\Sync_Keil.ps1
```

脚本扫描 User 各层的 .c/.h 和子目录，更新单个 Keil target 的分组及包含路径，可重复运行。User/* 分组由脚本管理，请不要在其中放入 User 外部文件。CubeMX 的 KeepUserCode 已开启；main.c 和 freertos.c 的入口均位于 USER CODE 区域。脚本未绑定到 CubeMX 钩子，Generate Code 后需手动执行并检查构建。

CubeMX 自己的 defaultTaskHandle 保持由生成代码管理；通信结构和共享通信资源放入 Msg，任务句柄留在各自任务的 .c/.h。Hardware_Init 初始化 Timer_Counter、OLED 和 Serial1；MPU6050 在独立任务内初始化；Msg_Init 仍为空框架，不创建无实际需求的通信对象。

FreeRTOS 当前动态堆为 6144 字节，使用 heap_4。以后增加任务、队列和软件定时器时，在 CubeMX 中调整堆、栈及 RTOS 配置，检查内存和创建结果。默认任务退出后内存由 idle task 回收。


## 已接入的硬件与函数

- Hardware/MPU6050.c、.h：使用 CubeMX I2C1（PB6/PB7），合并硬件驱动与 Mahony。±2 g、±250 °/s，传感器 100 Hz，DLPF_CFG=4；读取失败不发布新姿态。
- Task/MPU6050_Task.c、.h：Task_Init 注册独立原生 FreeRTOS 任务，384 words 栈、优先级 28（高于 OLED），vTaskDelayUntil 按 20 ms 更新；默认自动静止标定 200 点，初始化失败每秒重试。
- Hardware/OLED：OLED.c/.h、OLED_Data.c/.h 保留参考工程的字库、图形和显示 API。软件 I2C 使用 PA11/PA12，OLED_GPIO_Init 将设备引脚设置为开漏（不会调用 MX_GPIO_Init），总线需要上拉。地址沿用 0x3C。移植文件统一为 UTF-8，中文字宽为 3。仅修改缓冲区不会刷新显示，需调用 OLED_Update。
- Hardware/Serial + Function/Serial：沿用参考的 Serial_base 和 Serial_porting API，只有 Serial1 对象和协议。Serial_Init 只启动 USART1 的 ReceiveToIdle DMA，失败返回 HAL_ERROR。Serial2/3 无协议对象、无用户初始化；CubeMX 原有 MX_USART2/3 初始化仍保留。
- Tools/Timer_Counter.c/.h：移植 DWT 周期计时。Hardware_Init 初始化一次，Begin/End 测量代码耗时，Func 测量调用间隔。全局计时数据适合单一调用方；DWT 32 位计数在 72 MHz 下约 59.65 秒回绕，跨越一个以上回绕无法测量。

Hardware_Init 顺序为 Timer_Counter → OLED → Serial1。MPU6050 由独立任务初始化，App_Task 不再重复初始化或更新它。

### MPU6050/Mahony 调用

MPU6050 任务是驱动和解算器的唯一更新者。其他任务只读取快照：

```c
ImuReal_Typedef imu;
if (MPU6050_GetSnapshot(&imu) == HAL_OK)
{
    /* imu.roll / pitch / yaw / yaw_abs / GyroX / GyroY / GyroZ */
}
```

首次有效更新前、或数据超过 100 ms 未更新时，快照返回 HAL_ERROR。失败计数和数据年龄由 MPU6050_Task_GetFailCount / MPU6050_Task_GetDataAgeTicks 查询。六个 bias 全局变量和 MPU_Yaw_Abs_Get/Reset 保留。读取方不得再次调用初始化或更新函数。

### Serial1 协议

ABC 帧为 @文本$#，文本最大 39 字节；HEX 帧为 FF AA LEN [高字节 低字节 XOR]...55 FE，LEN 最大 255，数据是 int16_t。只有完整帧和所有 XOR 校验通过才发布 HEX 数据。

沿用参考工程的一次接收事件一帧模型：同一帧内不要产生 UART IDLE 间隔，两帧之间需要空闲间隔；暂不支持跨接收事件拼帧或粘包。Serial_RxEventCallback 的解析有上界，最大 255 个数据字，不进行 printf、阻塞发送或延时。数据为最近一帧缓冲，不是消息队列，消费者应及时读取。

通过 Serial_GetNewPackageFlag_ABC/HEX(&Serial1) 获取新帧标志，再调用对应查询 API。Serial_printf、Serial_SendBytes 等为阻塞发送，只从任务调用，并由调用方保证多任务间互斥。新增通信队列或互斥锁时在 Msg 中定义。Serial_printf 使用 256 字节局部格式化缓冲；实际业务任务应评估并调整栈。

UART 错误导致接收停止时，ISR 尝试重新启动 Serial1 接收；重启失败记录 Serial_Err_RX_Start，不在 ISR 中进入 Error_Handler。UART/DMA 内部回调指针继续由 HAL 管理。

### 验证范围

主机侧 HAL 替身测试覆盖 MPU6050 数据转换与失败保留、Mahony 静止/旋转输出、串口合法与校验失败帧、Serial2/3 不启动接收、OLED 绘图边界。实际 ARM 链接、Flash/RAM 占用、I2C 波形与板上收发仍需 Keil 构建和硬件验证。

### Mahony 标定接口恢复

默认 MPU6050_TASK_AUTO_CALIB=1，启动时静止、水平、+Z 朝上标定约 2 秒。使用 EEPROM 恢复值时改为 0，并在 MPU6050 任务启动前恢复六个 bias；当前工程没有 AT24C02 实现。运行时标定必须在 MPU6050 任务内部执行，不能由其他任务并发调用。标定失败不提交新 bias。迁移细节见工作区根目录的 MPU6050迁移说明.md。
