#include "Mode_2.h"
#include "AllHeader.h"
#include "Stepper.h"
#include "Emm_V5.h"

void Mode_2_Setup(void)
{
		OLED_Clear();
	
		SCB_DisableDCache();	// 一定要记得这个，否则缓存更新会使得数组数据不更新
	
		Stepper_Init() ;			// 步进电机初始化
}

// ============== Mode_2 主循环 ==============
void Mode_2_Loop(void)
{
	// 测试步进电机 使用USART2进行驱动 USART1 作为调试口
	static int cnt = 0 ;
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Emm_V5_Vel_Control(1, 0, 1000, 10, 0);
		cnt++;
	}
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		Emm_V5_Stop_Now(0x01 , false) ;	// 停车,不启用多机
	}

	OLED_Printf(0,0,OLED_6X8,"Stepper:%d rxFlag:%d",cnt,rxFrameFlag) ;
	OLED_Printf(0,10,OLED_6X8,"rxCount:%d",rxCount) ;
	OLED_Printf(0,20,OLED_6X8,"%d %d %d %d %d %d %d %d",
	rxCmd[0],rxCmd[1],rxCmd[2],rxCmd[3],rxCmd[4],rxCmd[5],rxCmd[6],rxCmd[7]) ;
}

void Mode_2_Tick(void)
{
	
}

void Mode_2_Exit(void)
{
	
}
