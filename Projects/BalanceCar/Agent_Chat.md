

## 硬件Hardware改造加入

现在需要你开始帮我进行硬件底层的加入





## 工程初始创建

请帮助我对当前 STM32CubeMX + FreeRTOS 工程建立一套长期可复用的基础工程框架。

目标是尽量将用户代码与 CubeMX 自动生成代码隔离。不要大规模修改 CubeMX 自动生成文件，只允许在 USER CODE 区域加入必要入口调用。

请首先检查当前工程结构、FreeRTOS API 类型以及 CubeMX 已生成的外设，然后按照以下要求完成重构。

### 一、建立 User 工程目录

建立：

User/
├─ Initial/
├─ AllHeader/
├─ Task/
├─ Msg/
├─ Hardware/
├─ Software/
└─ Function/

并将对应目录加入工程 Include Path / Keil Group，确保工程能够正常编译。

### 二、Initial 初始化层

建立：

User/Initial/Initial.c
User/Initial/Initial.h

提供：

```
void Hardware_Init(void);
void Msg_Init(void);
void Task_Init(void);
```

其中：

Hardware_Init()

只负责初始化用户自己的硬件驱动，例如：

LED_Init()
Key_Init()
OLED_Init()
IMU_Init()
Motor_Init()

不要把 CubeMX 自动生成的：

MX_GPIO_Init()
MX_DMA_Init()
MX_CAN_Init()
MX_UART_Init()
MX_I2C_Init()
MX_SPI_Init()

移动到 Hardware_Init()。

这些 MCU 外设初始化仍然由 CubeMX/main.c 管理，避免重新 Generate Code 后产生重复初始化问题。

在 main.c 的外设初始化全部完成以后，在合适的 USER CODE 区域调用：

```
Hardware_Init();
```

### 三、AllHeader 公共头文件

建立：

User/AllHeader/AllHeader.h

该文件只负责聚合工程常用公共头文件。

包括：

1. CubeMX 外设头文件
2. FreeRTOS 常用头文件
3. 用户 Hardware
4. Software
5. Function
6. Msg
7. Task

例如：

```
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "can.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "Msg.h"
#include "App_Task.h"
```

要求：

- `.c` 文件允许直接引用 AllHeader.h。
- 各模块自己的 `.h` 文件不要引用 AllHeader.h。
- 各 `.h` 应只 include 自己真正需要的依赖，避免循环 include。

### 四、Task 层

建立：

User/Task/App_Task.c
User/Task/App_Task.h

目前只创建一个 App_Task。

提供类似：

```
void App_Task_Init(void);
void App_Task(void *argument);
```

App_Task 当前业务循环保持空框架即可，例如：

```
for (;;)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
```

Task_Init() 统一负责创建所有业务任务。

目前只创建 App_Task。

以后新增任务时按照：

xxx_Task.c
xxx_Task.h

进行独立文件管理。

Task 层只负责：

- 任务创建
- 任务周期
- 调度
- 调用 Function 层

尽量不要直接实现复杂硬件驱动和算法。

### 五、Msg 层

建立：

User/Msg/Msg.c
User/Msg/Msg.h

Msg 层统一管理 FreeRTOS 任务之间的通信对象，包括：

- Queue
- Semaphore
- Mutex
- EventGroup
- Task Notification相关句柄
- 未来可能增加的软件Timer等

所有需要跨文件访问的 RTOS Handle：

只允许在 Msg.c 中真正定义一次。

例如：

```
QueueHandle_t xxxQueue = NULL;
SemaphoreHandle_t xxxMutex = NULL;
```

在 Msg.h 中使用：

```
extern QueueHandle_t xxxQueue;
extern SemaphoreHandle_t xxxMutex;
```

其他 Task 只需要：

```
#include "Msg.h"
```

即可访问对应通信对象。

Msg_Init() 统一创建所有通信资源。

当前如果没有实际通信需求，可以先建立空框架。

### 六、Hardware 层

User/Hardware/

负责所有真实硬件设备驱动。

例如以后：

Hardware/
├─ LED/
├─ Key/
├─ OLED/
├─ MPU6050/
├─ Motor/
└─ Encoder/

Hardware 层负责：

- GPIO操作
- HAL外设调用
- 设备寄存器
- 硬件通信协议
- 设备初始化

Hardware 不实现上层业务逻辑。

### 七、Software 层

User/Software/

负责与具体硬件无关的软件算法。

例如：

Software/
├─ PID/
├─ Filter/
├─ FIFO/
├─ CRC/
└─ AHRS/

要求尽量保持纯算法，不直接调用 HAL GPIO/UART/CAN 等硬件接口。

### 八、Function 层

User/Function/

Function 是 Hardware 与 Software 的组合功能层。

例如以后：

Function/
├─ Chassis/
├─ Attitude/
├─ Communication/
└─ SystemMonitor/

例如：

Chassis Function 可以：

读取 Hardware/Motor
+
调用 Software/PID
+
完成底盘速度控制

Task 层主要调用 Function，而不是把所有硬件和算法逻辑直接堆进 Task。

推荐依赖方向：

```
Task
 ↓
Function
 ↙     ↘
Hardware Software
 ↓
HAL
```

Msg 作为任务间通信公共资源使用。

### 九、FreeRTOS DefaultTask

保留 CubeMX 创建的 StartDefaultTask，但它不承担任何长期业务逻辑。

它只作为系统启动任务。

在 StartDefaultTask 中按照：

```
Msg_Init();
Task_Init();
vTaskDelete(NULL);
```

执行。

即：

FreeRTOS Scheduler启动
→ StartDefaultTask运行
→ 创建Msg通信资源
→ 创建所有业务Task
→ DefaultTask删除自身

如果当前工程使用 CMSIS-RTOS API，请根据当前工程实际 API 风格选择等价、安全的任务退出方式，不要无理由混用 CMSIS-RTOS 和 Native FreeRTOS API。

### 十、最终初始化流程

最终程序结构应尽量形成：

```
main()
 ↓
HAL_Init()
 ↓
SystemClock_Config()
 ↓
CubeMX MX_xxx_Init()
 ↓
Hardware_Init()
 ↓
FreeRTOS初始化
 ↓
osKernelStart()
 ↓
StartDefaultTask
 ↓
Msg_Init()
 ↓
Task_Init()
 ↓
删除StartDefaultTask
 ↓
App_Task等业务任务正常运行
```

### 十一、其他要求

1. 不要随意修改 CubeMX 自动生成代码。
2. 修改 main.c、freertos.c 时只尽量使用 USER CODE 区域。
3. 不要删除 CubeMX 的代码生成标记。
4. 不要重复初始化外设。
5. 不要制造循环 include。
6. 所有新建 `.h` 使用 include guard。
7. 保持 C 语言风格，不使用 C++。
8. 完成后确保工程至少在代码层面不存在明显的符号重复、函数未声明、头文件循环引用问题。
9. 如果当前工程结构与上述要求发生冲突，优先保持 CubeMX 可重新生成代码的能力。
10. 完成后说明：

- 新增了哪些目录
- 新增了哪些文件
- main.c 修改位置
- freertos.c 修改位置
- 各层之间的依赖关系
- 后续我要添加一个新 Task / Hardware / Software / Function 时应该如何添加。