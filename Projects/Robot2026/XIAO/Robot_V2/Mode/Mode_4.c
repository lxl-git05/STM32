#include "Mode_4.h"
#include "AllHeader.h"

// 定义各项参数
void Mode_4_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0,5,OLED_8X16,"================") ;
	 OLED_ShowChinese(0,25,"欢迎使用有衣有靠") ;
	 OLED_Printf(0,45,OLED_8X16,"================") ;
}

// 预备控制
void Mode_4_Loop(void)
{
	OLED_Printf(0, 0, OLED_6X8, "=====Mode_4=====") ;
}

void Mode_4_Tick(void)
{
	
}

void Mode_4_Exit(void)
{
  
}
