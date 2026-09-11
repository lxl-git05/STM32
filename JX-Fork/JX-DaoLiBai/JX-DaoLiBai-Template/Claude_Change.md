## 2026-09-11 10:55 | 移植 Mymain 与 10 组 Mode 架构

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| AllHeader.c | ./Top/AllHeader.c | 新增 | 提供全局模块和定时器初始化入口 |
| AllHeader.h | ./Top/AllHeader.h | 新增 | 汇总 HAL、Mode 及现有硬件模块头文件 |
| Mymain.c | ./Top/Mymain.c | 新增 | 实现 Mode 状态机主循环及切换生命周期 |
| Mymain.h | ./Top/Mymain.h | 新增 | 声明 Mymain 应用入口 |
| Mode_G.c | ./Mode/Mode_G.c | 新增 | 实现全局模式管理、模式切换和 Tick 分发 |
| Mode_G.h | ./Mode/Mode_G.h | 新增 | 定义模式枚举、状态变量和全局接口 |
| Mode_1.c/.h | ./Mode/Mode_1.c、./Mode/Mode_1.h | 新增 | 建立 Mode_1 空业务骨架 |
| Mode_2.c/.h | ./Mode/Mode_2.c、./Mode/Mode_2.h | 新增 | 建立 Mode_2 空业务骨架 |
| Mode_3.c/.h | ./Mode/Mode_3.c、./Mode/Mode_3.h | 新增 | 建立 Mode_3 空业务骨架 |
| Mode_4.c/.h | ./Mode/Mode_4.c、./Mode/Mode_4.h | 新增 | 建立 Mode_4 空业务骨架 |
| Mode_5.c/.h | ./Mode/Mode_5.c、./Mode/Mode_5.h | 新增 | 建立 Mode_5 空业务骨架 |
| Mode_6.c/.h | ./Mode/Mode_6.c、./Mode/Mode_6.h | 新增 | 建立 Mode_6 空业务骨架 |
| Mode_7.c/.h | ./Mode/Mode_7.c、./Mode/Mode_7.h | 新增 | 建立 Mode_7 空业务骨架 |
| Mode_8.c/.h | ./Mode/Mode_8.c、./Mode/Mode_8.h | 新增 | 建立 Mode_8 空业务骨架 |
| Mode_9.c/.h | ./Mode/Mode_9.c、./Mode/Mode_9.h | 新增 | 建立 Mode_9 空业务骨架 |
| Mode_10.c/.h | ./Mode/Mode_10.c、./Mode/Mode_10.h | 新增 | 建立 Mode_10 空业务骨架 |
| main.c | ./Core/Src/main.c | 修改 | 在 CubeMX 用户代码区引入并调用 Mymain |
| JX-DaoLiBai-Template.uvprojx | ./MDK-ARM/JX-DaoLiBai-Template.uvprojx | 修改 | 注册 Top、Mode 源文件与头文件并使用现有包含路径 |
| Keil 构建产物 | ./MDK-ARM/JX-DaoLiBai-Template/ | 修改 | 完整编译生成 AXF、HEX、MAP、日志及中间文件 |

## 2026-09-11 11:24 | 接入 SysTick、按键切换与 OLED 模式显示

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| stm32f1xx_it.c | ./Core/Src/stm32f1xx_it.c | 修改 | 在 SysTick_Handler 中每 1ms 调用模式定时回调 |
| Key.c | ./Hardware/Key.c | 修改 | 接入四个上拉按键 GPIO 的低电平按下检测 |
| AllHeader.h | ./Top/AllHeader.h | 修改 | 将 Key 模块加入统一头文件 |
| Mode_G.c | ./Mode/Mode_G.c | 修改 | 增加初始化保护、KEY1 单击切换和 OLED 模式号显示 |
| Keil 构建产物 | ./MDK-ARM/JX-DaoLiBai-Template/ | 修改 | 全量编译并更新 AXF、HEX、MAP 和构建日志 |
