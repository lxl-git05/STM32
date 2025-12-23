#include "Mymain.h"
#include "Initial.h"
#include "28byj48.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================
int check1 = 1 ;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	// 初始化28BYJ
	BYJ_Delay_Init() ;
	
	while(1)
	{
//		Menu_Func() ;
		
		for(int i = 0 ; i < 8 ; i ++)
		{
			BYJ_Half_Drive(i) ;
			HAL_Delay(check1) ;
		}
		
	}
}

// Systick定时中断,1ms周期
void HAL_SYSTICK_Callback(void)
{
	// 功能1: 按键
	Key_Tick() ;
	// 功能2: LED闪烁指示灯
	LED_Flash_Mode_Tick() ;
	// 功能3: 单次任务处理序列
	task_Once_Cnt_Tick() ;
}
