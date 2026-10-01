# Hardware

真实设备驱动：GPIO、HAL 调用、设备寄存器、协议及初始化。
新增模块采用 `LED/LED.c`、`LED/LED.h` 等独立子目录，在 Hardware_Init 中调用设备初始化。
不要在这里重复调用 CubeMX 的 MX_xxx_Init，也不要实现上层业务或控制算法。
