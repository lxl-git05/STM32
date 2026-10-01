# Function

组合 Hardware 驱动与 Software 算法，实现姿态、底盘、通信和系统监测等功能。
新增模块采用 `Attitude/Attitude.c`、`Attitude/Attitude.h` 等独立子目录，提供给 Task 调用的业务接口。
头文件仅包含自身需要的依赖，不引用 AllHeader.h。
