#include "Mode_5.h"
#include "AllHeader.h"

// 定义各项参数
void Mode_5_Setup(void)
{
   OLED_Clear() ;
}

// 预备控制
void Mode_5_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"========Mode_5========") ;
}

void Mode_5_Tick(void)
{

}

void Mode_5_Exit(void)
{
  
}
