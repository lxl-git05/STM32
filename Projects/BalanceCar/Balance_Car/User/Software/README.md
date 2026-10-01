# Software

与硬件无关的纯算法，例如 PID、Filter、FIFO、CRC、AHRS。
新增模块采用 `PID/PID.c`、`PID/PID.h` 等独立子目录，通过参数接收输入、返回结果。
不要调用 HAL GPIO/UART/CAN，也不要依赖任务或 AllHeader.h。
