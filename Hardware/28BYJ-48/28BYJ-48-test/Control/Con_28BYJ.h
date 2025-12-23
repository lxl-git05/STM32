#ifndef __CON_28BYJ_H
#define __CON_28BYJ_H

#include "28byj48.h"			// 底层驱动代码

// 初始化
void Con_BYJ_Init(void);
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
#endif
