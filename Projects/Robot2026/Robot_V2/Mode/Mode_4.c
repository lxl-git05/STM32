#include "Mode_4.h"
#include "AllHeader.h"

typedef enum
{
	Robot_Init ,
	Robot_Up ,
	
}Robot_Status_Typedef ;

Robot_Status_Typedef Robot_Status ;

// 定义各项参数

void Mode_4_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_Main=====") ;
}

void Mode_4_Loop(void)
{
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		
	}
	
}


void Mode_4_Tick(void)
{
	
}

void Mode_4_Exit(void)
{
  
}
