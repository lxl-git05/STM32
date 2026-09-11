# JX-DaoLiBai-Template 底层架构审查

审查日期：2026-09-11  
审查范围：`Core`、`Hardware`、`Software`、`Tools`、`Top`、`Mode`、`.ioc`、Keil 工程配置与最近构建记录。  
审查方式：只读通读；不把 `Drivers` 中 ST 官方 HAL/CMSIS 源码作为用户代码审查对象。

## 理解验证状态

| 核心概念 | 调用链已核对 | WHY 已解释 | 边界已验证 | 状态 |
|---------|-------------|-----------|-----------|------|
| CubeMX 生成层与自有代码边界 | 是 | 是 | 是 | 清晰，但 Keil 工程重生成风险未自动化 |
| 超级循环与 Mode 生命周期 | 是 | 是 | 是 | 方向正确，周期调度链未闭合 |
| SysTick 与主循环协作 | 是 | 是 | 是 | 1 ms 有效，10/20 ms 断开 |
| UART DMA + Idle 接收 | 是 | 是 | 是 | 基础可用，数据所有权和流式组帧不足 |
| Motor/PWM/Encoder/PID | 是 | 是 | 是 | 资源映射正确，类型、量纲和安全停机需收紧 |
| ADC2 多通道轮询 | 是 | 是 | 是 | 当前方案合理，错误传播不足 |

## 项目完整地图

```text
Core/                   CubeMX 生成层：时钟、GPIO、DMA、ADC、TIM、USART、IRQ
Hardware/               板级设备与协议：Key、OLED、Serial、RP、PWM、Encoder、Motor
Software/               与 MCU 无关的算法目标层：当前为 MyPID
Tools/                  DWT 周期计数工具
Top/                    初始化聚合、主循环、软件时间入口
Mode/                   Mode_G 状态管理与 Mode_1～Mode_10 生命周期
JX-DaoLiBai-Template.ioc CubeMX 单一硬件配置来源
MDK-ARM/                Keil 工程及构建输出
```

入口和主调用链：

```text
Reset_Handler
└─ main()
   ├─ HAL_Init()                         建立 HAL 1 ms SysTick
   ├─ SystemClock_Config()               72 MHz；ADC 12 MHz
   ├─ MX_GPIO / DMA / USART / ADC / TIM
   └─ Mymain()
      ├─ Mode_G_Setup()
      │  ├─ Initial_ALL()
      │  │  ├─ OLED_Init()
      │  │  ├─ Serial_Init()
      │  │  ├─ RP_Init()
      │  │  ├─ Motor_Init()
      │  │  └─ Encoder_Init()
      │  └─ Initial_Timer()
      └─ while (1)
         ├─ OLED_Clear()
         ├─ Mode_G_Loop()
         ├─ Mode Setup / Loop / Exit 分发
         └─ OLED_Update()
```

## 1. 快速概览

这是一个典型的 STM32 裸机分层工程：CubeMX 管 MCU 外设，Hardware 封装设备，Software 放算法，Mode 表达业务状态，Top 负责启动和超级循环。这个方向是正确的，也很适合当前单板、单电机、快速实验的规模；相比把所有代码塞进 `main.c`，已经明显更容易定位问题和复用模块。

当前瓶颈并不是目录还不够多，而是几个跨层契约尚未固定：谁拥有 1/10/20 ms 时间基准、ISR 和主循环如何交接数据、模式退出时谁负责让电机进入安全状态、PID 输出到 PWM 的单位和范围是什么。继续堆功能会把这些隐含约定放大成间歇故障，因此应先收紧契约，再扩展 Mode。

最新 Keil 构建记录为 0 Error、0 Warning；这证明语法、链接和当前资源引用成立，但不能证明竞态、越界、周期任务死路径和控制量纲正确。

## 2. 背景与动机

### 为什么保留当前分层

CubeMX 文件会重复生成，而业务代码变化频繁。把二者物理分开，可以让外设初始化继续由工具维护，同时把 Serial、Motor、Mode 等逻辑留在稳定目录中；否则每次重新生成都可能覆盖业务实现。

Mode 的 `Setup/Loop/Tick/Exit` 也值得保留。它把“进入状态、持续运行、周期控制、退出清理”表达成生命周期，比在一个大循环中堆条件分支更适合后续比赛任务。

### 为什么现在必须调整

速度环、位置环依赖稳定周期和安全退出。当前 `Mode_n_Tick()` 没有调用源，PWM 又会保持最后一次比较值；一旦某模式开始驱动电机，模式切换就可能留下持续输出。与此同时，Key 和 Serial 的状态跨 ISR/主循环共享，却没有快照、队列或临界区，这类问题通常无法靠编译器警告发现。

## 3. 核心概念网络

| 关系 | 上游 | 下游 | 为什么重要 |
|------|------|------|------------|
| 初始化顺序 | CubeMX 外设句柄 | Hardware Init | DMA、ADC、TIM 句柄必须先就绪，当前顺序基本正确 |
| 时间驱动 | SysTick | Key / Scheduler / Mode Tick | 周期必须只有一个所有者；当前只有 Key 链闭合 |
| 状态事务 | next_mode | Exit → Setup → curr_mode | 切换必须原子地完成，避免旧模式 Tick 插入 |
| 数据交接 | UART/Key ISR | 主循环 | ISR 应发布完整事件，主循环消费快照，而非共享可变对象 |
| 控制链 | Encoder → PID → Motor → PWM | TIM/GPIO | 每一层的单位、范围、符号必须一致 |
| 显示链 | Mode 绘制 | OLED framebuffer → 软件 I²C | 绘制可高频，物理刷屏应低频且按 dirty 执行 |

## 4. 算法与实时性

### Key 状态机

`Key_Tick()` 每 1 ms 递减计时器、每 20 ms 采样 GPIO，时间复杂度为 `O(KEY_COUNT)`，当前四键负载很低。算法选择合理，因为单击、双击、长按和连发都能由一个有限状态机统一描述；问题不在算法复杂度，而在事件位同时被 ISR 置位、被主循环清除，缺少原子交接。

### PID

`PID_Update()` 为常数时间、常数空间，适合 MCU 周期控制。但 `dt_ms` 直接参与积分和微分，参数单位不是常见的秒制；`PID_Init()` 又没有初始化全部状态字段。当前全局 `Motor` 依赖 BSS 清零才安全，通用 PID API 对栈对象并不安全。

### OLED

显存绘制大多与绘制像素数量线性相关，全屏刷新固定发送 1024 字节。算法本身适合 SSD1306，但主循环每一轮都执行全屏软件 I²C，使 Loop 周期由显示耗时决定。正确做法是将“写显存”和“提交屏幕”分离，并给物理刷新固定 50～100 ms 周期。

### Serial

HEX 单帧解析为 `O(LEN)`，容量可接受；但 ISR 内直接解析让中断时间随帧长增长，而且接收逻辑把 Idle/HT/TC 事件都送入同一解析入口。长期应变为“ISR 只收块和发布长度，主循环流式提帧”。

## 5. 设计模式评价

### 状态模式雏形

`curr_mode/next_mode` 与四个生命周期函数已经形成状态模式。优点是业务状态显式、切换入口集中；缺点是每新增模式都要同步修改多个 switch、enum、include 和工程文件。建议升级为 `ModeOps` 常量注册表，但不必立即引入复杂框架。

### Facade 初始化聚合

`Initial_ALL()` 像一个简单 Facade，让 `main` 保持干净。它目前没有错误返回，导致 ADC 校准、UART DMA 启动、PWM/Encoder 启动失败时仍进入模式运行；下一步应改为 `App_Init()` 返回整体状态，并在失败时进入 Fault/Null。

### God Header 反模式

`AllHeader.h` 汇总所有 Mode 和 Hardware，使用方便，但隐藏真实依赖并形成逻辑环。每个 `.c` 应只 include 自己真正调用的模块；`AllHeader` 最终应退化为只有初始化声明的 `App_Init.h`，甚至只保留 `.c` 内部实现。

## 6. 关键代码与问题清单

### 6.1 时间调度链

当前位置：`Top/Timer.c:5-29`、`Core/Src/stm32f1xx_it.c:186-195`、`Mode/Mode_G.c:29-58`。

当前只有 `SysTick → Timer_1ms_Callback → Key_Tick`。`Timer_10ms_Callback()` 和 `Timer_20ms_Callback()` 没有调用者，因此所有 `Mode_n_Tick()` 都是死路径。建议在 SysTick 中只增加软件计数并置 `10ms_due/20ms_due`，主循环读取标志后执行任务；这样模式切换、ADC 轮询、串口解析和 OLED 刷新都不在 ISR 内运行。

### 6.2 执行器安全与控制量纲

位置：`Hardware/Motor.c:21-56`、`Hardware/PWM.c:10-13`、`Core/Src/tim.c:45-49`、各 `Mode_n_Exit()`。

TIM2 ARR 为 99，PWM 的物理范围约为 0～99；但 `Motor_SetPWM()` 参数是 `int8_t`，PID 初始化输出范围却是 ±300，底层也不裁剪 Compare。未来把 PID float 输出直接传入时会出现窄化、回绕或饱和，控制器认为输出在变化，硬件却早已顶格。

应统一一个清楚的命令契约，例如 `Motor_SetDuty(int16_t percent)`，入口夹紧到 `[-100,100]`，再映射到 ARR。所有拥有执行器的 Mode 在 `Exit()` 中调用 `Motor_Stop()`，ModeManager 在进入 Null/Fault 时再执行一次全局 safe-stop。

此外，`Motor_Speed_Update()` 的公式乘以 60，实际得到 RPM，但注释写“圈/s”；`Gap_Time_ms` 和 PPR/减速比也没有零值检查。这些单位必须在闭环调参前固定。

### 6.3 编码器数据模型

位置：`Hardware/Encoder.c:4-24`、`Hardware/Encoder.h:8`、`Hardware/Motor.c:49-56`。

累计量 `Encoder_cnt` 是 `int32_t`，Getter 却返回 `int16_t`，大约累计 32767 脉冲后角度会回绕。应把返回链完整改成 `int32_t`。更稳的增量算法是不清零 TIM3，而是保留上一次 `uint16_t` 计数，通过模减得到 `int16_t delta`；这样可避免“读 CNT 到清零 CNT 之间”的脉冲丢失窗口。

`Motor_Typedef.Encoder_Cnt` 当前没有使用，与 `Encoder_cnt` 构成双重状态源；应删除，或让它成为唯一累计位置。

### 6.4 PID 初始化安全

位置：`Software/MyPID.c:7-17,30-36`。

`PID_Init()` 只写 Kp/Ki/Kd、限幅和 dt，没有初始化 `PID_Func`、历史误差、积分、微分滤波和死区。全局 `Motor` 因 BSS 为零暂时没出错，但任何未清零的局部 `Pid_Typedef` 都可能让 `PID_Func` 成为垃圾非空指针并跳到非法地址。这是 API 级最高风险，应首先修复为完整确定性初始化，并拒绝 `pid == NULL`、`dt <= 0`。

PID 内部建议统一使用 `dt_s`。积分限幅字段若限制的是累计误差，应命名为 `sum_error_max`；若希望限制积分输出，则应对 `iout` 限幅并实现输出饱和 anti-windup。

### 6.5 ISR 与主循环的数据所有权

位置：`Hardware/Key.c:11,37-47,75-145`、`Hardware/Serial_porting.c:53-162,286-315`。

`Key_Flag` 在 SysTick 中做置位，在主循环中做读改清；ISR 若插在主循环“读旧值—写回”之间，新事件会丢失。仅加 `volatile` 不能修复 read-modify-write 竞态。应在极短临界区内 fetch-and-clear，或改事件队列/计数器；`Key_Check()` 还需要 `n < KEY_COUNT`。

Serial 的 `frame_valid`、ABC flag、长度和数据也由 ISR 写、主循环读。主循环取到 flag 后尚未复制完数据时，新帧可能覆盖同一对象。应使用双缓冲或一次性 copy-out API：ISR 发布完整 buffer 索引，主循环取得稳定快照后解析。

### 6.6 Serial 协议边界

位置：`Hardware/Serial_porting.c:53-88,120-152,286-315`、`Hardware/Serial_base.h:14-18`。

- ABC 解析没有接收 `Size`，会读取本次 DMA 有效区之外的旧缓冲数据；短帧可能与旧 `$#` 尾部拼成假包。
- HEX 单字 XOR 失败时保留旧值，却仍把整帧标为 valid；控制数据会静默混入上一帧。
- Receive-to-IDLE 的 HT 事件也进入解析入口，会用半帧扰动状态；HT 时重启通常返回 HAL_BUSY，但返回值被忽略。应关闭 HT 或按 `HAL_UARTEx_GetRxEventType()` 分支。
- 协议 LEN 是 `uint8_t`，最大只能表达 255，`Serial_RX_MAX_WORDS=256` 与检查条件不一致。
- 初始化先启动 RX DMA，再初始化协议常量，存在极短首帧竞态；常量应编译期 `const`，或至少先初始化再收包。

### 6.7 栈与格式化安全

位置：`MDK-ARM/startup_stm32f103xb.s:32`、`Hardware/Serial_porting.c:32-47,222-244`、`Hardware/OLED.c:903-910`。

Keil 栈为 1024 字节；`Serial_Send_HEX_Package()` 单个局部 TX 数组约 773 字节，再叠加调用链与中断上下文，余量很小。TX 缓冲应放入 Serial 实例或静态发送队列，并有 busy 状态管理。

`OLED_Printf()` 使用 256 字节局部数组和无界 `vsprintf`，格式化结果超过 255 字节会直接破坏栈。必须改为 `vsnprintf` 并检查返回值。`OLED_UpdateArea()` 只检查 X 起点、不裁剪 Width，也存在跨行读越界；字符索引还应限制在可显示 ASCII 范围。

### 6.8 主循环负载与显示

位置：`Top/Mymain.c:8-68`、`Hardware/OLED.c:444-454`。

每轮循环先清空 1 KB 显存，再经软件 I²C 推送完整 1024 字节。结果是应用 Loop 没有稳定周期，CPU 时间主要花在显示；切换模式那一轮还会提交空显存，可能闪屏。建议显示任务固定为 50～100 ms，只有 dirty 时刷新，并在切换完成后先绘制新模式再提交。

### 6.9 初始化与 CubeMX 边界

当前 `HAL_Init → Clock → GPIO → DMA → USART → ADC → TIM` 顺序正确，ADC `/6 = 12 MHz`、DMA Channel4/5、USART IRQ、TIM2 PWM 和 TIM3 Encoder 配置都无需改。

但 HAL SysTick 在 `HAL_Init()` 后已经运行，业务 `Timer_1ms_Callback()` 会在 GPIO 和 Key 尚未完成初始化时被调用。建议增加 `app_ready` 门控：ISR 始终执行 `HAL_IncTick()`，只有 `App_Init()` 完成后才发布业务 tick。`Timer_Initial()` 不应再次配置 SysTick，以免形成双重所有权。

CubeMX 的 C 文件 USER CODE 块目前使用正确；风险在 Keil `.uvprojx` 中手工加入的 Hardware/Top/Mode 分组和 IncludePath。重新生成工程可能丢失它们，应把“生成后恢复分组/路径、diff、全量构建”写成固定流程或脚本。

## 7. 测试覆盖

工程没有自有自动化测试。当前 Keil 构建只能验证编译链接，不能验证：

- Key 的单击/双击/长按时间边界和并发丢事件；
- Mode 的 Exit/Setup 顺序及故障回退；
- Serial 的分片、粘包、错误校验、半传输事件；
- PID 的首次调用、重初始化、dt=0、输出饱和；
- Encoder 的 16 位回绕与累计方向。

建议优先把 Key 状态机、PID、Serial 解析器和 ModeManager 提炼为不依赖 HAL 的纯 C，在 PC 上用表驱动输入测试。Hardware Port 层只负责把 GPIO/ADC/UART 数据送进纯逻辑。

## 8. 可迁移场景

### 从单电机扩展到双电机

不变的是 Motor 的“方向、PWM、编码器、PID”组合；需要改变的是所有硬编码的 `htim2/htim3` 和全局 `Motor`。将 HAL 句柄、通道、GPIO 端口/引脚和参数注入 `MotorContext` 后，同一套控制逻辑才能创建两个实例，而不是复制 `MotorA.c/MotorB.c`。

### 从裸机扩展到 RTOS

不变的是 Mode 生命周期和驱动 API；需要改变的是 Scheduler 和事件交接。若现在就保证 ISR 只置位/入队、解析和显示都在主上下文，未来可自然替换为任务通知/队列；如果继续在 ISR 中解析 Serial、运行 Mode Tick，迁移时会遇到优先级和锁依赖问题。

## 9. 建议目标分层与实施顺序

```text
Core/ 或 Generated/        CubeMX 生成，只拥有 MCU 初始化和 IRQ 转发
Hardware/Port/             GPIO、ADC、TIM、UART 的薄硬件绑定
Services/                  Key事件、Serial解析、Display调度、MotorControl
App/Mode/                  ModeOps、ModeManager、具体业务模式
Top/App_Main.c             初始化、调度 pending 任务、故障入口
```

建议按以下顺序改，不需要一次推倒重来：

1. **立即修正确性**：PID 完整初始化、OLED `vsnprintf`、Encoder Getter 改 `int32_t`、Motor/PWM 统一范围、Serial 校验失败不得标 valid。
2. **建立调度契约**：1 ms 只计时/置位，补齐 10/20 ms pending 调度；OLED 降频；Mode Tick 回到主上下文。
3. **建立安全契约**：新增 `Motor_Stop()`，所有执行器 Mode Exit 必停，Null/Fault 全局兜底。
4. **建立数据所有权**：Key 原子取事件；Serial 双缓冲/流式解析；所有 Init 和 I/O 返回状态。
5. **收紧依赖**：拆 God Header，Software 去 HAL 依赖，按需把硬编码全局句柄改为 context 注入。

## 10. 最终评价

当前架构可以评为“方向正确、适合快速验证，但实时控制契约尚未完成”。值得保留的是 CubeMX 与自有代码的物理分层、Mode 生命周期、Hardware API 和显存式 OLED；最需要改变的不是文件夹名字，而是时间、状态、单位、错误和安全这五类跨层契约。

如果只做一轮小改，优先完成：

1. 补齐 10/20 ms 调度；
2. `Motor_Stop()` + Mode Exit 安全停机；
3. Encoder Getter 32 位、PWM/PID 限幅统一；
4. PID_Init 确定性清零；
5. Key/Serial 的 ISR→主循环快照交接。

完成这五项后，这套骨架才适合继续承载速度环、位置环和更多传感器。
