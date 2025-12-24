#include "Mymain.h"
#include "Initial.h"
#include "MPU6050.h"

// =================== 全局变量 ===================
int Print_Choice ;	// 串口打印数据
int flag = 0 ;
// =================== 实验区域 ===================
MPU6050_t MPU6050;

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	while (MPU6050_Init_Kalman(&hi2c2) == 1);
	/*
	MPU6050_Init() ;
	// MPU6050初始化,等待0.1秒（可根据你的模块调整 3~10秒）
	OLED_ShowString(0, 0, "MPU Warming...", OLED_8X16);
	OLED_Update() ;
	HAL_Delay(100);
	MPU6050_Data_Error_Check(1000) ;
	*/
	while(1)
	{
//		Menu_Func() ;
		Timer_Counter_Func() ;
		/*
		// 3. MPU6050读取数据	
		MPU6050_Update_Data() ;
		
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			MPU6050_Data_Error_Check(1000) ;
		}
		else if (Key_Check(KEY_1 , KEY_DOUBLE))
		{
			Print_Choice ++ ;
			Print_Choice %= 4 ;
		}
		
		OLED_ShowFloatNum(0,0  ,MPU_Real.roll,3,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,20 ,MPU_Real.pitch,3,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,40 ,MPU_Real.yaw,3,3,OLED_8X16) ;
		
		OLED_ShowFloatNum(70,0 ,MPU_Cali.GX,2,2,OLED_8X16) ;
		OLED_ShowFloatNum(70,20,MPU_Cali.GY,2,2,OLED_8X16) ;
		OLED_ShowFloatNum(70,40,MPU_Cali.GZ,2,2,OLED_8X16) ;	
		
		static int cnt = 0 ;
		cnt ++ ;
		
		Set_Current_USART(USART1_IDX) ;
		if (Print_Choice == 0)
		{
			printf("%f,%f,%f,%d\n" ,  MPU_Raw_Data.AX , MPU_Raw_Data.AY , MPU_Raw_Data.AZ ,100) ;
		}
		else if (Print_Choice == 1)
		{
			printf("%f,%f,%f,%d\n" ,  MPU_Cali.AX , MPU_Cali.AY , MPU_Cali.AZ ,200) ;
		}
		else if (Print_Choice == 2)
		{
			printf("%f,%f,%f,%d\n" ,  MPU_Raw_Data.GX , MPU_Raw_Data.GY , MPU_Raw_Data.GZ ,300) ;
		}
		else if (Print_Choice == 3)
		{
			printf("%f,%f,%f,%d\n" ,  MPU_Cali.GX , MPU_Cali.GY , MPU_Cali.GZ ,400) ;
		}
		// 静止检测 + 自动调节零漂 -> 45us
		MPU_Still_Check() ;
		MPU6050_Data_Error_Check_Auto() ;
		*/
		// 卡尔曼滤波实验
		MPU6050_Read_All(&hi2c2, &MPU6050);
		HAL_Delay (100);
		
		// 打印
		OLED_ShowFloatNum(70,0 ,MPU6050.KalmanAngleX,3,3,OLED_8X16) ;
		OLED_ShowFloatNum(70,20,MPU6050.KalmanAngleY,3,3,OLED_8X16) ;
		
		// 打印计时时间
		OLED_ShowNum(0 , 50 , time_us , 8 , OLED_6X8 ) ;
		OLED_ShowNum(60 , 50 , time_Func_us , 8 , OLED_6X8 ) ;
		
		// 更新OLED状态
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
	// 功能3 : 得到积分角度
	/*
	MPU6050_Raw_Error_Update();	// 更新去零参数
	static int MPU_Count  = 0 ;
	MPU_Count ++ ;
	if (MPU_Count == 10) // 10ms
	{
		MPU_Count = 0 ;
		MPU6050_Raw_Deal(10) ;	// 10ms更新
	}
	*/
}
