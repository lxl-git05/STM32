#include "Mymain.h"

// *******************全局变量*******************
extern Serial_HEX_Data_Typedef   Serial_Hex_Data ;			// 解析好的HEX数据包
extern Serial_ABC_Data_Typedef   Serial_ABC_Data ;			// 解析好的ABC数据包

// *******************实验区域*******************
int check1 ;
int check2 ;
int check[50] ;


void Mymain(void)
{
	HAL_SYSTICK_Config(SystemCoreClock / 1000);
	// OLED初始化
	OLED_Init() ;
	OLED_ShowString(0 , 0 , "Hello World" , OLED_8X16 ) ;
	// 串口初始化
	Serial_Init(&Serial_huart) ;
	// 
	
	// ******************* 实验区域 *******************
	
	while(1)
	{
		// 测试按键功能
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
		}
		// ******************* 实验区域 *******************
		
		
		
		// 必须存在:OLED更新
		OLED_Update() ;
	}
}



// Systick定时中断
void HAL_SYSTICK_Callback(void)
{
	// 计时
	static int count_sys = 0 ;
	count_sys ++ ;
	// 功能1:
	if (count_sys % 1000 == 0)
	{
//		HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin ) ;
	}
	// 功能2: 按键
	Key_Tick() ;
}




