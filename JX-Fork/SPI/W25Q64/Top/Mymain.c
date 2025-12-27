#include "Mymain.h"
#include "Initial.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================

uint8_t ArrayW[] = {0x0a , 0xf2 , 0x13 , 0x45} ;
uint8_t ArrayR[4] ;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	W25Q64_Init() ;
	
	// 读取ID号
	uint8_t MID = 0 ;
	uint16_t DID = 0 ;
	W25Q64_ReadID(&MID, &DID) ;
	
	OLED_ShowHexNum(0  , 20 , MID , 4 ,OLED_6X8) ;
	OLED_ShowHexNum(20 , 20 , DID , 4 ,OLED_6X8) ;
	
	// 写入读取全流程:必须先擦除!!!因为可以1->0,但是不能0->1,所以最好是擦除了!!!
	
	// 先擦除
	W25Q64_SectorErase(0x000000) ;
	
	// 写
	W25Q64_PageProgram(0x000000 , ArrayW , 4) ;
	
	// 读
	W25Q64_ReadData(0x000000 , ArrayR , 4 ) ;
	
	// 展示结果
	OLED_ShowHexNum(0  , 40 , ArrayW[0] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(20 , 40 , ArrayW[1] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(40 , 40 , ArrayW[2] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(60 , 40 , ArrayW[3] , 2 , OLED_6X8) ;
	
	OLED_ShowHexNum(0  , 50 , ArrayR[0] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(20 , 50 , ArrayR[1] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(40 , 50 , ArrayR[2] , 2 , OLED_6X8) ;
	OLED_ShowHexNum(60 , 50 , ArrayR[3] , 2 , OLED_6X8) ;
	
	while(1)
	{
//		Menu_Func() ;
		
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
