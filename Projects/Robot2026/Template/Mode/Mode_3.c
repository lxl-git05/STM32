#include "Mode_3.h"
#include "AllHeader.h"

float check ;

void Mode_3_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_3=====") ;
}

void Mode_3_Loop(void)
{
	// // 1. 测试串口功能
    // if (Serial_GetNewPackageFlag_ABC(&Serial1))
    // {
    //     Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &check) ;
    //     Serial_printf(&Serial1 , "%f\n", &check) ;
    // }
    // OLED_Printf(0, 40, OLED_6X8, "%.2f" , check) ;  // 测试OLED
    // 2. 测试电机
//     OLED_ClearArea(0, 10, 120, 10) ;
//     OLED_Printf(0 , 10, OLED_6X8, "A:%.0f" ,  Motor_A.PID_s.realPoint_Now) ;
//     OLED_Printf(60, 10, OLED_6X8, "B:%.0f" , -Motor_B.PID_s.realPoint_Now) ;
//     // Serial参数更改
//     if (Serial_GetNewPackageFlag_ABC(&Serial1))
//     {
// #ifdef  Motoe_A_PID_Check
//         // 得到数据
//         Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &Motor_A.PID_s.Kp) ;
//         Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &Motor_A.PID_s.Ki) ;
//         Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &Motor_A.PID_s.Kd) ;
//         Serial_SetFloatData(&Serial1, "goalPoint_A", "goalPoint_A=%f", &Motor_A.PID_s.goalPoint) ;
//         Motor_SetSpeed(&Motor_A, Motor_A.PID_s.goalPoint) ;
//         // OLED展示PID参数
//         OLED_Printf(0, 30, OLED_6X8, "%.2f,%.2f,%.2f" , Motor_A.PID_s.Kp , Motor_A.PID_s.Ki , Motor_A.PID_s.Kd) ;
// #else 
//         Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &Motor_B.PID_s.Kp) ;
//         Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &Motor_B.PID_s.Ki) ;
//         Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &Motor_B.PID_s.Kd) ;
//         Serial_SetFloatData(&Serial1, "goalPoint_B", "goalPoint_B=%f", &Motor_B.PID_s.goalPoint) ;
//         Motor_SetSpeed(&Motor_B, Motor_B.PID_s.goalPoint) ;
//         // OLED展示PID参数
//         OLED_Printf(0, 30, OLED_6X8, "%.2f,%.2f,%.2f" , Motor_B.PID_s.Kp , Motor_B.PID_s.Ki , Motor_B.PID_s.Kd) ;
// #endif  
//      }
}

void Mode_3_Exit(void)
{
    OLED_Clear() ;
}

void Mode_3_Tick(void)
{
	
}
