#include "Mymain.h"
#include "Initial.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================
int16_t AX , AY , AZ , GX , GY , GZ ;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	MPU6050_Init() ;
	
	while(1)
	{
//		Menu_Func() ;
		
		// 3. MPU6050读取数据
//		Timer_Counter_Begin() ;
		MPU6050_GetData(&AX , &AY , &AZ , &GX , &GY , &GZ) ;	// 根据实测,采样时间:软件IIC需要5.7ms,硬件IIC为1.5ms
//		Timer_Counter_End() ;
		OLED_ShowSignedNum(0,0,AX,5,OLED_8X16) ;
		OLED_ShowSignedNum(0,20,AY,5,OLED_8X16) ;
		OLED_ShowSignedNum(0,40,AZ,5,OLED_8X16) ;
		
		OLED_ShowSignedNum(60,0,GX,5,OLED_8X16) ;
		OLED_ShowSignedNum(60,20,GY,5,OLED_8X16) ;
		OLED_ShowSignedNum(60,40,GZ,5,OLED_8X16) ;	
		
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
