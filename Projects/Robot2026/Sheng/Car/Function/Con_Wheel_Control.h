#ifndef __CON_WHEEL_CONTROL_H
#define __CON_WHEEL_CONTROL_H

#include "AllHeader.h"

// 5种状态: 停车 (Pos + Angle直行) 左转 右转 180度掉头
typedef enum
{
    Car_Stop    ,   // 停车         	 0
		Car_Turn_F  ,   // 角度环直行   	 1
    Car_Turn_L  ,   // 角度环左转   	 2
    Car_Turn_R  ,   // 角度环右转   	 3
    Car_Turn_H  ,   // 角度环自转180度 4
}Car_Status_Typedef ;

extern Car_Status_Typedef curr_Status ;
extern Car_Status_Typedef next_Status ;

// 电机外环控制台,切记控制台只进行控制，不实现状态转换，所以需要手动判断是否控制完成然后进行状态转换
void Car_Control(void) ;

// 状态转换配置
void Car_Control_Change(void) ;

#endif
