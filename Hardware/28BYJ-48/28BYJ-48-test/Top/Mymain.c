#include "Mymain.h"
#include "Initial.h"
#include "Con_28BYJ.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================
int RPM = 0 ;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	// 初始化28BYJ
	Con_BYJ_Init() ;
	
	while(1)
	{	
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			Con_BYJ_Set_RPM(++RPM) ;	// 加速
		}
		else if (Key_Check(KEY_1 , KEY_DOUBLE))
		{
			Con_BYJ_Stop() ;											// 停止运动
		}
		else if (Key_Check(KEY_1 , KEY_LONG))
		{
			Con_BYJ_Set_Position(BYJ_Ni) ;		// 逆时针旋转
		}
		else if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			Con_BYJ_Set_Tar_Angle(BYJ_MAX_Rotate_RPM , 90) ;
		}
		else if (Key_Check(KEY_2 , KEY_LONG))
		{
			Con_BYJ_Set_Tar_Angle(BYJ_MAX_Rotate_RPM , -180) ;
		}
		
		OLED_Update() ;	// OLED更新
	}
}

// Systick定时中断,1ms周期
void HAL_SYSTICK_Callback(void)
{
	// 功能1: 按键
	Key_Tick() ;
}
