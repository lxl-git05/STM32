#include "Mymain.h"
#include "Initial.h"
#include "HMC5883.h"
// =================== 全局变量 ===================
void MPU6050_Func(void) ;
void Print_Data(void) ;

int Print_Choice ;	// 串口打印数据
int flag = 0 ;
// =================== 实验区域 ===================

int16_t mag_max[3] = {-32768, -32768, -32768};
int16_t mag_min[3] = {32767, 32767, 32767};

void Mymain(void)
{
	Initial_ALL() ;	
	__enable_irq(); // 与Systick有关的在Systick初始化后初始化
	
	Set_Current_USART(USART1_IDX) ;
	
	MPU6050_Init() ;
	
	// MPU6050初始化,等待0.1秒
	OLED_ShowString(0, 0, "MPU Warming...", OLED_8X16);
	OLED_Update() ;
	HAL_Delay(100);
	MPU6050_Data_Error_Check(1000) ;
	
	// HMC初始化
	HMC_Init() ;
	
	while(1)
	{
		Timer_Counter_Func() ;
		
		MPU6050_Func() ;
		Print_Data() ;
		
		// HMC驱动测试
		Timer_Counter_Begin() ;
		
		HMC5883_Update_Data() ;	// 859us

		Timer_Counter_End() ;
		
		if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			LED_Flash_Mode_Set_Mode(LED_Flash_Fast) ;
//			HMC5883_Calibrate() ;

			
			uint32_t start_time = HAL_GetTick();
			
			while(HAL_GetTick() - start_time < 20000)  // 20秒校准时间
			{
					HMC5883_Update_Data();  // 更新数据
					
					// 更新最大最小值（X, Z, Y 对应索引0,1,2）
					if(HMC_Data.MagX_Raw > mag_max[0]) mag_max[0] = HMC_Data.MagX_Raw;
					if(HMC_Data.MagX_Raw < mag_min[0]) mag_min[0] = HMC_Data.MagX_Raw;
					
					if(HMC_Data.MagZ_Raw > mag_max[1]) mag_max[1] = HMC_Data.MagZ_Raw;
					if(HMC_Data.MagZ_Raw < mag_min[1]) mag_min[1] = HMC_Data.MagZ_Raw;
					
					if(HMC_Data.MagY_Raw > mag_max[2]) mag_max[2] = HMC_Data.MagY_Raw;
					if(HMC_Data.MagY_Raw < mag_min[2]) mag_min[2] = HMC_Data.MagY_Raw;
				
					HAL_Delay(50);  // 约20Hz采样
			}
			
			LED_Flash_Mode_Set_Mode(LED_Flash_ON) ;
			
			// 计算偏移（硬铁校准）
			HMC_Data.MagX_Offset = (mag_max[0] + mag_min[0]) / 2;
			HMC_Data.MagZ_Offset = (mag_max[1] + mag_min[1]) / 2;
			HMC_Data.MagY_Offset = (mag_max[2] + mag_min[2]) / 2;
		}
		
		
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
	MPU6050_Raw_Error_Update();	// 更新去零参数
	static int MPU_Count  = 0 ;
	MPU_Count ++ ;
	if (MPU_Count == 10) // 10ms
	{
		MPU_Count = 0 ;
		MPU6050_Raw_Deal(10) ;	// 10ms更新
	}
	// 实验
	
}

void MPU6050_Func(void)
{
	// MPU6050读取数据	
	MPU6050_Update_Data() ;

	// 静止检测 + 自动调节零漂 -> 45us
	MPU_Still_Check() ;
	MPU6050_Data_Error_Check_Auto() ;
}

void Print_Data(void)
{
	// OLED展示MPU6050数据
	OLED_ShowFloatNum(0,0  ,HMC_Data.MagX_Raw,3,3,OLED_8X16) ;
	OLED_ShowFloatNum(0,20 ,HMC_Data.MagY_Raw,3,3,OLED_8X16) ;
	OLED_ShowFloatNum(0,40 ,HMC_Data.MagZ_Raw,3,3,OLED_8X16) ;
	
	OLED_ShowFloatNum(70,0 ,MPU_Cali.GX,2,2,OLED_8X16) ;
	OLED_ShowFloatNum(70,20,MPU_Cali.GY,2,2,OLED_8X16) ;
	OLED_ShowFloatNum(70,40,MPU_Cali.GZ,2,2,OLED_8X16) ;	
	
	// 打印计时时间
	OLED_ShowNum(0 , 50 , time_us , 8 , OLED_6X8 ) ;
	OLED_ShowNum(60 , 50 , time_Func_us , 8 , OLED_6X8 ) ;
	
	// 串口打印选择
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		MPU6050_Data_Error_Check(1000) ;
	}
	else if (Key_Check(KEY_1 , KEY_DOUBLE))
	{
		Print_Choice ++ ;
		Print_Choice %= 4 ;
	}
	
	// 串口打印
	if (Print_Choice == 0)
	{
//		printf("%f,%f,%f,%d\n" ,  MPU_Raw_Data.AX , MPU_Raw_Data.AY , MPU_Raw_Data.AZ ,100) ;
//		printf("%d,%d,%d,%d\n" ,  HMC_Data.MagX_Offset , HMC_Data.MagY_Offset , HMC_Data.MagZ_Offset ,100) ;
		printf("%d,%d,%d,%d\n" ,  mag_max[0] , mag_max[2] , mag_max[1] ,100) ;
	}
	else if (Print_Choice == 1)
	{
//		printf("%f,%f,%f,%d\n" ,  MPU_Cali.AX , MPU_Cali.AY , MPU_Cali.AZ ,200) ;
		printf("%d,%d,%d,%d\n" ,  mag_min[0] , mag_min[2] , mag_min[1] ,600) ;
	}
	else if (Print_Choice == 2)
	{
//		printf("%f,%f,%f,%d\n" ,  MPU_Raw_Data.GX , MPU_Raw_Data.GY , MPU_Raw_Data.GZ ,300) ;
		printf("%d,%d,%d,%d\n" ,  HMC_Data.MagX_Offset , HMC_Data.MagY_Offset , HMC_Data.MagZ_Offset ,100) ;
	}
	else if (Print_Choice == 3)
	{
		printf("%f,%f,%f,%d\n" ,  MPU_Cali.GX , MPU_Cali.GY , MPU_Cali.GZ ,400) ;
	}
}
