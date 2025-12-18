#include "Mymain.h"
#include "Initial.h"
// =================== 全局变量 ===================

// =================== 实验区域 ===================

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	MPU6050_Init() ;
	MPU6050_Data_Error_Check(1000) ;
	
	while(1)
	{
//		Menu_Func() ;
		// 3. MPU6050读取数据	
		MPU6050_Update_Data() ;
		
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			MPU6050_Data_Error_Check(1000) ;
		}
		
		OLED_ShowFloatNum(0,0  ,MPU_Real.roll,2,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,20 ,MPU_Real.pitch,2,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,40 ,MPU_Real.yaw,2,3,OLED_8X16) ;
		
		OLED_ShowFloatNum(60,0 ,MPU_Cali.GX,2,3,OLED_8X16) ;
		OLED_ShowFloatNum(60,20,MPU_Cali.GY,2,3,OLED_8X16) ;
		OLED_ShowFloatNum(60,40,MPU_Cali.GZ,2,3,OLED_8X16) ;	
		
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
	// 功能4 : 得到积分角度
	
	MPU6050_Raw_Error_Update();	// 更新去零参数
	
	static int MPU_Count  = 0 ;
	MPU_Count ++ ;
	
	MPU_Real.yaw += MPU_Cali.GZ * 1 * 1.0 / 1000;
	// 10ms
	if (MPU_Count == 10)
	{
		MPU_Count = 0 ;
		MPU6050_Raw_Deal(10) ;	// 10ms更新
	}
}
