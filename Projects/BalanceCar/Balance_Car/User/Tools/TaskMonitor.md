# 原生 FreeRTOS 任务报告（不修改业务任务循环）

TaskMonitor_Print 在 KEY_0 长按时采集 uxTaskGetSystemState，业务任务不再调用 TaskMonitor_Begin/End。保持原周期、业务和优先级。

| 字段 | 来源与单位 |
|---|---|
| Name / State / Prio | 原生任务快照 |
| RunTime(us) | ulRunTimeCounter，累计被调度运行时间，当前DWT统计时基为微秒 |
| CPUwin% | 相邻两次快照运行计数增量 / 总计数增量；首次从启动算起 |
| CPUall% | 累计任务运行计数 / 累计总计数，与ESP32示例的计算方式相同 |
| StackMin(B) | usStackHighWaterMark * sizeof(StackType_t)，历史最低剩余栈，不是当前剩余 |
| Heap total / Free / MinFree | RTOS堆总量、当前余量、历史最低余量，字节 |

IDLE、Timer和用户任务统一打印，不需要修改这些任务。静态快照缓冲避免报告自身申请动态堆。采集期间短暂暂停调度，打印前恢复；分行等待只属于诊断输出。

仅凭原生任务快照无法得知业务循环次数、周期超时次数、单轮执行耗时或启动延迟，相关列和业务埋点已删除。RTOS切换次数不等同于业务循环次数。

STM32的FreeRTOSConfig.h仍须启用运行时间统计并提供计时时基；ESP32示例能直接使用ulRunTimeCounter，是建立在其工程已配置运行时间统计的基础上。本工程保持USER CODE内原生统计配置，Tools/TaskMonitor只提供DWT微秒时基及窗口快照差值，不含业务循环接口。无需额外硬件定时器，不修改RTOS内核。

注意：不在调度器运行后清零DWT或切换CPU时钟。Tick采样扩展原始DWT约59.65秒回绕；32位微秒累计值约71.6分钟回绕，所以CPUall和RunTime仅在未回绕区间内解释为启动以来累计值。CPUwin通过无符号差值可跨一次回绕（窗口需小于71.6分钟），FreeRTOS10.3.1略过跨回绕的一个调度片，可能有少量误差。

CPU计数包含当时任务被中断打断的时间，不能用来独立计算ISR负载。正在运行但尚未结束的调度片未计入任务累计值；首次统计包含已删除DefaultTask的执行，因此百分比合计可能略低于100%。

新任务自动出现在报告中，不需要添加监控调用。当前最多16个任务；增加容量需同步检查RAM占用。

构建验证：Keil ARMCC5 编译、链接0错误0警告；尚未上板验证新表格。

报告缓冲、状态名称和输出全部在 TaskMonitor.c 内部；Debug_Task 只调用 TaskMonitor_Print()。公开接口只保留计时初始化、计时读取和报告打印。CubeMX可启用GENERATE_RUN_TIME_STATS并生成时基函数入口，但DWT具体实现及traceTASK_INCREMENT_TICK仍由用户代码提供；切换生成方式时应避免同名宏重复定义。
