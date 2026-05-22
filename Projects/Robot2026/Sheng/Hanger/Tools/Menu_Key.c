#include "Mode_3.h"
#include "AllHeader.h"

// 测试全局变量
float check ;

// 测试函数声明
void Check_Serial(Serial_Typedef* pSerial);
void Check_PWM(void) ;  // 需要初始化
void Check_Encoder(void) ;  // 需要初始化

void Mode_3_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_3=====") ;
}

void Mode_3_Loop(void)
{
	// 本loop函数建议只执行一个check任务,防止未知Bug
}

void Mode_3_Exit(void)
{
    OLED_Clear() ;
}

void Mode_3_Tick(void)
{
	
}

// 1. 测试串口功能
void Check_Serial(Serial_Typedef* pSerial)
{
    if (Serial_GetNewPackageFlag_ABC(pSerial))
    {
        Serial_SetFloatData(pSerial, "Kp", "Kp=%f", &check) ;
        Serial_printf(pSerial , "%f\n", &check) ;
    }
    OLED_Printf(0, 40, OLED_6X8, "%.2f" , check) ;  // 测试OLED
}

// 2. 测试PWM功能, 记得先初始化哦
void Check_PWM(void)
{
    static int PWM_Servo_Check = 50;    // 50-250
    if (Key_Check(KEY_0, KEY_LONG))
    {
        PWM_Servo_Check += 50 ;
        if (PWM_Servo_Check > 250)
        {
            PWM_Servo_Check = 50 ;
        }
    }
    MyPWM_SetCompare(&MyPWM_Servo1, PWM_Servo_Check) ;
}

// 3. 测试编码器功能, 记得先初始化哦
void Check_Encoder(void)
{
    OLED_Printf(0, 40, OLED_6X8, "%d" , MyEncoder_Get_CNT(&Motor_A_Encoder)) ;
}
        return NULL;

    // 转回真正的数据结构类型
    return (OLED_MenuItem*)node->data;
}

// 菜单界面开关逻辑,true为开,false为关
bool Menu_isOpen_Mode(void)
{
	// 菜单界面开关逻辑
	if (Key_Check(KEY_1 , KEY_LONG))
	{
		// 如果是关闭状态就打开
		if (Menu_Open_Mode == 0)
		{
			Menu_Open_Mode = 1; 	// 打开
		}
		// 如果是打开状态就预备关闭
		else if (Menu_Open_Mode == 1)
		{
			Menu_Open_Mode = 2 ;	// 预备关闭
		}
	}
	// 预备关闭
	if (Menu_Open_Mode == 2)
	{
		OLED_Clear()  ;
		OLED_Update() ;
		Menu_Open_Mode = 0 ;
	}
	return Menu_Open_Mode ;
}

// 菜单展示界面(放在主函数),所有菜单的通用逻辑:长按KEY1打开OLED,再次长按KEY1关闭OLED,长按KEY2回到主界面
void Menu_Func(void)
{
	// 菜单界面开关逻辑,长按按键1打开菜单,再次长按按键1关闭菜单,如果菜单打开就执行对应逻辑
	if (Menu_isOpen_Mode() == true)
	{
		// 通用逻辑:长按KEY2回到主界面
		if (Key_Check(KEY_2 , KEY_LONG))
		{
			Menu_Confirm_index = 0 ;
		}
		OLED_Clear() ;
		// OLED菜单展示界面
		OLED_MenuItem* Menu_Now_Item = Menu_Get_Item(Menu_Confirm_index) ;
		Menu_Now_Item->MenuCallback() ;
		// 怕自己忘记更新OLED,直接在这里更新得了
		OLED_Update() ;	
	}
}

// ******************菜单回调函数定义****************** 

void Menu_Main_Callback(void)
{
	// OLED展示界面
	OLED_Printf(0 , 0 , OLED_8X16 , "======Menu======") ;
	
	OLED_Printf(0 , 20  , OLED_6X8 , "Task1") ;
	OLED_Printf(0 , 35  , OLED_6X8 , "Task2") ;
	OLED_Printf(0 , 50  , OLED_6X8 , "Task3") ;
	
	OLED_Printf(60 , 20 , OLED_6X8 , "Task4") ;
	OLED_Printf(60 , 35 , OLED_6X8 , "Task5") ;
	OLED_Printf(60 , 50 , OLED_6X8 , "Check") ;
	
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		++Menu_Confirm_index  ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	else if (Key_Check(KEY_1 , KEY_DOUBLE))
	{
		Menu_Confirm_index += Menu_Total_Num - 1;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
}

void Menu_Check_Callback(void)
{
	// 调参模式
	Key_Param_Check() ;
}

void Menu_Task1_Callback(void)
{
	OLED_Printf(0 , 0  , OLED_8X16 , "=========Task1=========") ;
//	OLED_Printf(0 , 15 , OLED_6X8 , "KEY1_L -> Close Menu");
//	OLED_Printf(0 , 30 , OLED_6X8 , "KEY2_L -> Back Main");
//	OLED_Printf(0 , 45 , OLED_6X8 , "KEY2_S -> Task1_GO");
//		
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Menu_Confirm_index ++ ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	// KEY2单击:
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		
	}
}

void Menu_Task2_Callback(void)
{
	OLED_Printf(0 , 0 , OLED_8X16 , "=========Task2=========") ;
//	OLED_Printf(0 , 15 , OLED_6X8 , "KEY1_L -> Close Menu");
//	OLED_Printf(0 , 30 , OLED_6X8 , "KEY2_L -> Back Main");
//	OLED_Printf(0 , 45 , OLED_6X8 , "KEY2_S -> Task2_GO!");
		
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Menu_Confirm_index ++ ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	// KEY2单击:
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		
	}
	
}

void Menu_Task3_Callback(void)
{
	OLED_Printf(0 , 0 , OLED_8X16 , "=========Task3=========") ;
//	OLED_Printf(0 , 15 , OLED_6X8 , "KEY1_L -> Close Menu");
//	OLED_Printf(0 , 30 , OLED_6X8 , "KEY2_L -> Back Main");
//	OLED_Printf(0 , 45 , OLED_6X8 , "KEY2_S -> Car_Just_Go");
		
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Menu_Confirm_index ++ ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	// KEY2单击:
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		
	}
	
}

void Menu_Task4_Callback(void)
{
	OLED_Printf(0 , 0 , OLED_8X16 , "=========Task4=========") ;
	
//	OLED_Printf(0 , 15 , OLED_6X8 , "KEY1_L -> Close Menu");
//	OLED_Printf(0 , 30 , OLED_6X8 , "KEY2_L -> Back Main");
//	OLED_Printf(0 , 45 , OLED_6X8 , "KEY2_S -> Car_Go");
		
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Menu_Confirm_index ++ ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	// KEY2单击:
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		
	}
	
}

void Menu_Task5_Callback(void)
{
	OLED_Printf(0 , 0 , OLED_8X16 , "=========Task5=========") ;
	
//	OLED_Printf(0 , 15 , OLED_6X8 , "KEY1_L -> Close Menu");
//	OLED_Printf(0 , 30 , OLED_6X8 , "KEY2_L -> Back Main");
//	OLED_Printf(0 , 45 , OLED_6X8 , "KEY2_S -> Car_Go");
		
	// 按键逻辑:单击进入下一个菜单
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Menu_Confirm_index ++ ;
		Menu_Confirm_index = Menu_Confirm_index % Menu_Total_Num ;
	}
	// KEY2单击:
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		
	}
}
