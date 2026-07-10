## 2026-07-09 15:30 | 补全步进电机配置和读取函数

| 文件名 | 文件路径（相对工作区） | 操作类型 | 说明 |
|--------|----------------------|----------|------|
| Stepper.h | Stepper_Driver/Hardware/Stepper.h | 修改 | 新增pulse_angle结构体成员，增加5个函数声明，添加外部变量声明 |
| Stepper.c | Stepper_Driver/Hardware/Stepper.c | 修改 | 补全相对/绝对位置设置函数、重置零点函数、速度/位置读取函数，更新全局变量 |
