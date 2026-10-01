# 1. 程序框架逻辑

## 1-1 Config

+ Allheader包含所有头文件
+ Initial包含
  + ==硬件初始化==
  + ==任务初始化==
    + 包含任务优先级宏定义
  + ==通信数据结构初始化**及其声明**==

+ ISR是所有硬件中断的入口回调，实现清晰的回调可视，省的去到各个文件写回调



## 1-2 Task

+ ==写一个Task==
  + 声明句柄并且extern
  + 去到Task_init初始化
  + 可以用了

## 1-3 Hardware

+ 驱动代码，与裸机无异



##  1-4 Function

+ 集成software 和 hardware，也就是应用层



## 1-5 Software

+ 软件算法



## 1-6 Tools

+ 工具