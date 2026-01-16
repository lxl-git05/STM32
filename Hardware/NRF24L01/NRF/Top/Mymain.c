#include "Mymain.h"
#include "Initial.h"
#include "NRF24L01.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================

uint8_t Check[5] ;
int  a= 0 ;
int b = 0 ;
void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	NRF_Init() ;		// 初始化
	
	NRF_ReadRegs(0x10 , Check , 5 ) ;
	
	while(1)
	{
//		Menu_Func() ;
		
		if (1)
		{
			NRF_TxPacket[0] ++ ;
			NRF_TxPacket[1] += 2 ;
			NRF_TxPacket[2] += 3 ;
			NRF_TxPacket[3] += 4 ;
			
			NRF24L01_Send() ;
			a ++ ;
			
		}
		
		if (NRF_Receive())
		{
			OLED_ShowHexNum(0  , 20 , NRF_RxPacket[0] , 2 , OLED_6X8) ;
			OLED_ShowHexNum(20 , 20 , NRF_RxPacket[1] , 2 , OLED_6X8) ;
			OLED_ShowHexNum(40 , 20 , NRF_RxPacket[2] , 2 , OLED_6X8) ;
			OLED_ShowNum(70 , 20 , b++ , 8 , OLED_6X8) ;
		}
		
		// 计时器
		Timer_Counter_Begin() ;
		
		Timer_Counter_End() ;
		
		
		OLED_ShowNum(0 , 0 , time_us , 8 , OLED_6X8) ;
		OLED_Update() ;
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
