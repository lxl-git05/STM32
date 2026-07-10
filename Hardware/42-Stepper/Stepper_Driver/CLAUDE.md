# Stepper_Driver 步进电机驱动

## 项目概述

基于 STM32H7 + Emm_V5 闭环步进电机的双串口驱动，支持命令队列调度，防止 DMA 异步发送导致的死锁和粘包问题。

## 硬件配置

| 项目 | 电机1 | 电机2 |
|------|-------|-------|
| UART | USART3 | USART6 |
| TX/RX 引脚 | PB10/PB11 | PC6/PC7 |
| DMA RX | DMA1_Stream4 | DMA1_Stream6 |
| 波特率 | 115200 | 115200 |
| 电机地址 | 0x01 | 0x01 |

## 核心概念：为什么需要命令队列？

DMA 发送是**异步**的。`Emm_V5_Vel_Control()` 将数据放入 DMA 缓冲区后立即返回，不等待发送完成。

```
旧设计: while(tx_busy);  // 死等 → 可能卡死
新设计: Enqueue → 队列 → 5ms轮询出队 → 发送
```

**关键优势**：
1. 主循环调用 `Enqueue` 立即返回，不阻塞
2. 5ms 定时器统一调度，不会粘包
3. STOP 命令优先级最高，队列满时丢弃旧命令

## 快速开始

### 1. 初始化
```c
Stepper_Init();  // 在 Mode_2_Setup() 中调用
```

### 2. 5ms 定时器（必须）
```c
// 在 SysTick 或定时器中断中调用
Timer_5ms_Callback(void) {
    Stepper_Polling_5ms();  // 处理队列和状态查询
}
```

### 3. 主循环控制
```c
// 速度模式
Stepper_Vel_Enqueue(&Stepper1, 100, 0);  // 速度100RPM

// 相对位置 (相对当前角度运动)
Stepper_RelPos_Enqueue(&Stepper1, 50, 0, 360.0f);  // 相对运动360度

// 绝对位置 (运动到指定角度)
Stepper_AbsPos_Enqueue(&Stepper1, 50, 0, 0.0f);  // 运动到0度

// 位置清零
Stepper_ResetZero_Enqueue(&Stepper1);

// 停车 (最高优先级)
Stepper_Stop_Enqueue(&Stepper1);
```

## 命令队列 API

| 函数 | 说明 | 参数 |
|------|------|------|
| `Stepper_Vel_Enqueue(p, vel, acc)` | 速度模式 | vel: 速度(正负), acc: 加速度 |
| `Stepper_RelPos_Enqueue(p, vel, acc, angle)` | 相对位置 | angle: 相对运动角度(度) |
| `Stepper_AbsPos_Enqueue(p, vel, acc, angle)` | 绝对位置 | angle: 目标角度(度) |
| `Stepper_ResetZero_Enqueue(p)` | 位置清零 | - |
| `Stepper_Stop_Enqueue(p)` | 停车 | 队列满也执行 |

**返回值**: `true`=成功入队, `false`=队列满（STOP除外）

## 命令类型

```c
typedef enum {
    CMD_NONE = 0,       // 空命令
    CMD_VEL,            // 速度模式
    CMD_REL_POS,        // 相对位置
    CMD_ABS_POS,        // 绝对位置
    CMD_RESET_ZERO,     // 位置清零
    CMD_STOP,           // 立即停车
    CMD_QUERY_POS,      // 查询位置
    CMD_QUERY_VEL,      // 查询速度
} Stepper_CmdType;
```

## 电机状态读取

状态查询在 `Stepper_Polling_5ms()` 中自动完成，存储在结构体中：

```c
Stepper1.Speed_Now   // 当前速度
Stepper1.Pos_Now     // 当前角度
Stepper1.cmd_queue.count  // 队列中的命令数量
```

## 关键设计

### 两电机完全独立
- 独立 UART 和 DMA
- 独立 `tx_busy` 标志
- 独立命令队列
- 各自 5ms 轮询，互不阻塞

### 原子操作
入队/出队使用 `__disable_irq()` 保护，可安全从中断调用。

### 查询状态交替
- 电机1: 交替查询位置 → 速度 → 位置 → ...
- 电机2: 交替查询位置 → 速度 → 位置 → ...

## 文件结构

```
Stepper_Driver/
├── Hardware/
│   ├── Stepper.h      # 头文件：结构体、API声明
│   ├── Stepper.c      # 实现：队列、调度、初始化
│   └── Emm_V5.h       # 电机驱动库（第三方）
├── Mode/
│   └── Mode_2.c       # 使用示例
└── CLAUDE.md          # 本文档
```

## pulse_angle 参数

每脉冲对应的角度值，用于角度和脉冲数的换算。

```
angle(度) = clk(脉冲数) × pulse_angle(度)
```

| 电机 | pulse_angle | 说明 |
|------|-------------|------|
| 电机1 | 0.004f | 1.8°步距角 / 75:1减速比 / 162细分 |
| 电机2 | 0.01f | 1.8°步距角 / 180细分 |

## 常见问题

### Q: 为什么主循环调用 Enqueue 后没有立即执行？
A: 命令入队后等待 5ms 定时器轮询才出队发送，这是设计预期。

### Q: 队列满了会怎样？
A: 普通命令入队失败返回 `false`；STOP 命令会丢弃最旧命令保证执行。

### Q: 两个电机可以同时发送吗？
A: 可以，两个 UART 完全独立，各自轮询，互不干扰。

### Q: 如何调试队列状态？
A: `Stepper1.cmd_queue.count` 显示队列中的命令数量（OLED 显示 Q:xx）。

## 注意事项

1. **必须调用 `Stepper_Polling_5ms()`** 在 5ms 定时器中，否则队列不会执行
2. **不要直接调用旧 API** 如 `Stepper_Speed_Set()`，它们存在死锁风险
3. **STOP 命令优先级最高**，队列满时会丢弃其他命令
