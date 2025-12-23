[toc]

# 模块名:

## 1. 项目简介

​	28BYJ-48模块，步进电机

## 2. 核心功能

​	按照1-15圈/分钟旋转，快了会堵转

​	采用定时器调整节拍+调节定时中断ARR的方式调节转速

## 3. 核心函数	

* 底层库 28byj48.h
  * 初始化
  * 配置节拍
  * 配置方向
  * 配置速度
  * 停止
  * 中断驱动
* 实例库 Con_28BYJ.h
  * **创建实例 ：BYJ**
  * 初始化
  * 配置方向
  * 配置速度
  * 停止
  * 中断驱动
  * **中断调用**

## 4. 基础必备代码

### 4-1 库导入

```c
#include "Con_28BYJ.h"	// 只需要导入驱动库（也就是实例库），底层不需要导入
```

### 4-2 全局变量(域)

```c
BYJ_Typedef BYJ ;// 创建的实例
```

### 4-3 setup

```c
Con_BYJ_Init() ;
```

### 4-4 while

```c
// 配置方向
void Con_BYJ_Set_Position(BYJ_Pos_Typedef Pos);
// 配置速度
void Con_BYJ_Set_RPM(int RPM);
// 停止
void Con_BYJ_Stop(void) ;
// 在中断驱动BYJ
void Con_BYJ_Drive(void) ;
// 驱动目标角度
void Con_BYJ_Set_Tar_Angle(int Set_RPM ,int Goal_Angle) ;
```



### 4-5 while后函数

```c
// BYJ_TIM定时器，用来修改节拍，8拍模式，!!!放在了实例函数.c里面！！！
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //判断是否是BYJ_TIM中断,当前默认是1ms执行一次
  if (htim->Instance == BYJ_TIM)
  {
    Con_BYJ_Drive() ;
  }
}
```



## 6. 功能示例代码

​	上面有，工程里面也有

## 5. Cube配置

* TIM2定时器，初始是PSC = 72-1 ，使得中断计时的tick为1us,ARR为1000，也就是中断默认1ms触发一次

## 7. 引脚定义

| 引脚号 |        标签1        |     标签2     |
| :----: | :-----------------: | :-----------: |
|  PA_1  | BYJ_Pin_1_GPIO_Port | BYJ_Pin_1_Pin |
|  PA_2  | BYJ_Pin_2_GPIO_Port | BYJ_Pin_2_Pin |
|  PA_3  | BYJ_Pin_3_GPIO_Port | BYJ_Pin_3_Pin |
|  PA_4  | BYJ_Pin_4_GPIO_Port | BYJ_Pin_4_Pin |

## 8. 注意事项

​	最大转速为11转/min，是自己测出来的，再快就堵转了，可能在别的供电or设备不同，自己去底层修改宏定义即可

​	目标角度旋转最大转速为9转/min，多了卡死，多测测吧:smile:

## 9. 参考资料

1. [Arduino程序设计（十五）步进电机驱动（28BYJ-48）-CSDN博客](https://blog.csdn.net/weixin_44887565/article/details/142656430?ops_request_misc=&request_id=&biz_id=102&utm_term=28BYJ RPM&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-2-142656430.142^v102^pc_search_result_base3&spm=1018.2226.3001.4187)
2. [STM32之28BYJ-48步进电机驱动-CSDN博客](https://blog.csdn.net/qq_50749196/article/details/149002968?ops_request_misc=&request_id=&biz_id=102&utm_term=28BYJ RPM&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-149002968.142^v102^pc_search_result_base3&spm=1018.2226.3001.4187)
3. * [手把手教会你驱动28BYJ-48步进电机，从0带着敲代码，实现模块封装，方便移植，测试过F1和F4板子，完美兼容。_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1UF411y7rK/?spm_id_from=888.80997.embed_other.whitelist&t=1.92737&bvid=BV1UF411y7rK&vd_source=362392e571870fdd0784b51ea97fc582)

## 10. 更新日志

* 2025/12/23
  * 完成该工程
