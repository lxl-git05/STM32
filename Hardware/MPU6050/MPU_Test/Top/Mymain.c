#include "Mymain.h"
#include "Initial.h"
// =================== 全局变量 ===================
float check1 = 5.0f;
float check[50] ;
int k = 0 ;
int flag = 0 ;
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
		
		OLED_ShowFloatNum(0,0  ,MPU_Real.roll,3,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,20 ,MPU_Real.pitch,3,3,OLED_8X16) ;
		OLED_ShowFloatNum(0,40 ,MPU_Real.yaw,3,3,OLED_8X16) ;
		
		OLED_ShowFloatNum(70,0 ,MPU_Cali.GX,2,2,OLED_8X16) ;
		OLED_ShowFloatNum(70,20,MPU_Cali.GY,2,2,OLED_8X16) ;
		OLED_ShowFloatNum(70,40,MPU_Cali.GZ,2,2,OLED_8X16) ;	
		
		static int cnt = 0 ;
		cnt ++ ;
		
		Set_Current_USART(USART1_IDX) ;
		printf("%f,%f,%f,%d\n" ,  MPU_Raw_Data.GZ , MPU_Cali.GZ , MPU_Real.yaw,cnt) ;
		
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
	if (MPU_Count == 10) // 10ms
	{
		MPU_Count = 0 ;
		MPU6050_Raw_Deal(10) ;	// 10ms更新
	}
	// 实验
	if (MPU_Cali.GZ > check1 || MPU_Cali.GZ < -check1)
	{
		LED_Flash_Mode_Set_Mode(LED_Flash_ON) ;
	}
	else
	{
		LED_Flash_Mode_Set_Mode(LED_Flash_OFF) ;
	}
}
