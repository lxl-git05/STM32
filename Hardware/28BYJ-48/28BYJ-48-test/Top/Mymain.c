#include "Mymain.h"
#include "Initial.h"
#include "28byj48.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================
int RPM = 0 ;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	// 初始化28BYJ
	BYJ_Init() ;
	
	while(1)
	{
		// DEBUG配置RPM
//		BYJ_Set_RPM(RPM) ;
		
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			BYJ_Set_RPM(RPM++) ;	// 加速
		}
		else if (Key_Check(KEY_1 , KEY_DOUBLE))
		{
			BYJ_Stop() ;											// 停止运动
		}
		else if (Key_Check(KEY_1 , KEY_LONG))
		{
			BYJ_Set_Position(BYJ_Pos_Ni) ;		// 逆时针旋转
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
