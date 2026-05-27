#include "Mode_4.h"
#include "AllHeader.h"

//#define Motoe_A_Pos_Check

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
		OLED_Printf(0, 0, OLED_6X8, "=====Mode_Pos=====") ;
		// 展示A和B的运动位移
		Motor_Pos_Update(&Motor_A) ;
		Motor_Pos_Update(&Motor_B) ;
		OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
		// 修改PID
		// Serial参数更改
    if (Serial_GetNewPackageFlag_ABC(&Serial1))
    {
#ifdef  Motoe_A_Pos_Check
        // 得到数据
        Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &Motor_A.PID_Pos.Kp) ;
        Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &Motor_A.PID_Pos.Ki) ;
        Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &Motor_A.PID_Pos.Kd) ;
//        Serial_SetFloatData(&Serial1, "goalPoint_A", "goalPoint_A=%f", &Goal_Pos) ;
        Motor_SetPos(&Motor_A, Goal_Pos);
//        // OLED展示PID参数
//        OLED_Printf(0, 30, OLED_6X8, "%.2f,%.2f,%.2f" , Motor_A.PID_Pos.Kp , Motor_A.PID_Pos.Ki , Motor_A.PID_Pos.Kd) ;
#else 
        Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &Motor_B.PID_Pos.Kp) ;
        Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &Motor_B.PID_Pos.Ki) ;
        Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &Motor_B.PID_Pos.Kd) ;
//        Serial_SetFloatData(&Serial1, "goalPoint_B", "goalPoint_B=%f", &Goal_Pos) ;
//        // OLED展示PID参数
//        OLED_Printf(0, 30, OLED_6X8, "%.2f,%.2f,%.2f" , Motor_B.PID_Pos.Kp , Motor_B.PID_Pos.Ki , Motor_B.PID_Pos.Kd) ;
#endif  
    }
		
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			PID_Angle_Curr_Reset();
			PID_Goal_Angle_Set(180) ;
		}
		else if (Key_Check(KEY_1 , KEY_DOUBLE))
		{
			
		}
		else if (Key_Check(KEY_1 , KEY_LONG))
		{
			
		}
		else if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			PID_Angle_Curr_Reset();
			PID_Goal_Angle_Set(-90) ;
		}
}

void Mode_4_Tick(void)
{

#ifdef  Motoe_A_Pos_Check
    Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n",Motor_A.PID_Pos.goalPoint ,Motor_A.PID_Pos.realPoint_Now ,Motor_A.PID_Pos.setPoint );
#else 
    Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n",Motor_B.PID_Pos.goalPoint ,Motor_B.PID_Pos.realPoint_Now ,Motor_B.PID_Pos.setPoint );
#endif
}

void Mode_4_Exit(void)
{
  
}
