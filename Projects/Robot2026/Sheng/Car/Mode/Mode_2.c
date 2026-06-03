#include "Mode_2.h"
#include "AllHeader.h"

extern int Base_Speed ;

float Motor_Angle_Check ;

void Mode_2_Setup(void)
{
   OLED_Clear() ;
}

void Mode_2_Loop(void)
{
		OLED_Printf(0, 0, OLED_6X8, "=====Mode_Angle=====") ;
    // Serial参数更改
    if (Serial_GetNewPackageFlag_ABC(&Serial1))
    {
        // 得到数据
        Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &PID_Angle.Kp) ;
        Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &PID_Angle.Ki) ;
        Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &PID_Angle.Kd) ;
        Serial_SetFloatData(&Serial1, "Angle", "Angle=%f", &Motor_Angle_Check) ;
		}
		PID_Angle.goalPoint = Motor_Angle_Check ;
		// OLED展示PID参数
		OLED_Printf(0, 30, OLED_6X8, "%.2f,%.2f,%.2f" , PID_Angle.Kp , PID_Angle.Ki , PID_Angle.Kd) ;
		OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
		// 按键控制
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			Base_Speed = 40 ;
			PID_Angle_Curr_Reset()  ;	// 40速度往前直行
		}
		if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			PID_Angle_Curr_Reset()  ;
			PID_Goal_Angle_Set(-90) ;	// 直角转向
		}
		OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
		OLED_Printf(0,50,OLED_6X8 , "p:%.2f i:%.2f d:%.2f", PID_Angle.Kp,PID_Angle.Ki,PID_Angle.Kd) ;
}

// 20ms让小车旋转固定角度
void Mode_2_Tick(void)
{
		PID_Angle_Tick(Base_Speed) ;
		Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n",PID_Angle.goalPoint ,PID_Angle.realPoint_Now ,PID_Angle.setPoint );
}

void Mode_2_Exit(void)
{
  
}
