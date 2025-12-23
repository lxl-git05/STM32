#ifndef __28BYJ48_H
#define __28BYJ48_H

#include "main.h"
// 步进电机方向定义
typedef enum
{
	BYJ_Pos_Shun ,	// 顺时针旋转
	BYJ_Pos_Ni			// 逆时针旋转
}BYJ_Pos_Typedef;

#define BYJ_MAX_RPM 11	// 步进电机在5V时的最大速度(亲测)

// =================== 函数 ===================
// 初始化驱动BYJ的定时器中断
void BYJ_Init(void);
// 配置方向
void BYJ_Set_Position(BYJ_Pos_Typedef Pos);
// 配置速度
void BYJ_Set_RPM(int RPM);
// 停止
void BYJ_Stop(void) ;
#endif
