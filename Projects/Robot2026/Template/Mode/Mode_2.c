#include "Mode_2.h"
#include "AllHeader.h"

void Mode_2_Setup(void)
{
//    OLED_Clear() ;
//    OLED_Printf(0, 0, OLED_6X8, "=====Mode_Angle=====") ;
}

void Mode_2_Loop(void)
{
	
}

// 20ms让小车旋转固定角度
void Mode_2_Tick(void)
{
//    // 1. 得到真实角度(yaw)
//    // 将tick写在MPU更新之后即可
//    PID_Angle.realPoint_Now = MPU_Real.yaw ;

//    // 3. PID计算
//    PID_Update(&PID_Angle, PID_Angle.realPoint_Now ) ;

//    // 4. 输出小车转速, 差速
//    Motor_SetSpeed(&Motor_A, - PID_Angle.setPoint) ;    // A B 差速不要搞岔
//    Motor_SetSpeed(&Motor_B,   PID_Angle.setPoint) ;

//    // 5. 展示效果
//    Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n",PID_Angle.goalPoint ,PID_Angle.realPoint_Now ,PID_Angle.setPoint );
}

void Mode_2_Exit(void)
{
    
}
