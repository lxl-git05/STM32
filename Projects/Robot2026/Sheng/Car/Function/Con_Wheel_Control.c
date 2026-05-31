#include "Con_Wheel_Control.h"

Car_Status_Typedef curr_Status = Car_Stop;
Car_Status_Typedef next_Status = Car_Stop;

// 1. Car_Stop
void Car_Stop_Setup(void)
{
	PID_Angle_Curr_Reset() ;	// 重置初始yaw角度
	PID_Goal_Angle_Set(0) ;		// 目标角度为0
}
void Car_Stop_Tick(void)		// 小车停止控制
{
	PID_Angle_Tick(0) ;				// 外环:基础速度为0
}

// 2. Car_Turn_F
void Car_Turn_F_Setup(void)
{
	PID_Angle_Curr_Reset() ;	// 重置初始yaw角度
	PID_Goal_Angle_Set(0) ;		// 目标角度为0
	Motor_Pos_Clear() ;				// 清除累计位移
}
void Car_Turn_F_Tick(int BaseSpeed)
{
	PID_Angle_Tick(BaseSpeed) ;	// 外环:小车走直线，并且配置 BaseSpeed 基础速度
}
bool Car_Turn_F_Is_Exit(int Position)	// 小车直行一段距离之后改状态
{
	return Motor_A.PID_Pos.realPoint_Now > Position ;
}

// 3. Car_Turn_L
void Car_Turn_L_Setup(void)
{
	PID_Angle_Curr_Reset() ;		// 重置初始yaw角度
	PID_Goal_Angle_Set(100) ;		// 目标角度配置
}
void Car_Turn_L_Tick(void)
{
	PID_Angle_Tick(0) ;					// 基础速度为0
}
bool Car_Turn_L_Is_Exit(void)
{
    float err = fabs(PID_Angle.goalPoint - MPU_Real.yaw);

    float gyro = fabs(MPU_Cali.GZ);	// 角速度小,代表几乎暂停,说明转向完成

    if (err < 2.0f && gyro < 5.0f)
    {
        return true;
    }

    return false;
}

// 4. Car_Turn_R
void Car_Turn_R_Setup(void)
{
	PID_Angle_Curr_Reset() ;		// 重置初始yaw角度
	PID_Goal_Angle_Set(-100) ;	// 目标角度配置
}
void Car_Turn_R_Tick(void)
{
	PID_Angle_Tick(0) ;				// 基础速度为0
} 
bool Car_Turn_R_Is_Exit(void)
{
    float err = fabs(PID_Angle.goalPoint - MPU_Real.yaw);

    float gyro = fabs(MPU_Cali.GZ);	// 角速度小,代表几乎暂停,说明转向完成

    if (err < 2.0f && gyro < 5.0f)
    {
        return true;
    }

    return false;
}

// 5. Car_Turn_H
void Car_Turn_H_Setup(void)
{
	PID_Angle_Curr_Reset() ;	// 重置初始yaw角度
	PID_Goal_Angle_Set(180) ;	// 目标角度配置
}
void Car_Turn_H_Tick(void)
{
	PID_Angle_Tick(0) ;				// 基础速度为0
} 
bool Car_Turn_H_Is_Exit(void)
{
	return MPU_Real.yaw > 180 ;
}

// 电机外环控制台,切记控制台只进行控制，不实现状态转换，所以需要手动判断是否控制完成然后进行状态转换
void Car_Control(void)
{
    if (curr_Status == next_Status)
    {
        switch (curr_Status)
        {
            case Car_Turn_F   : Car_Turn_F_Tick(60) ;    break;    // 寻迹环
            case Car_Turn_L   : Car_Turn_L_Tick() ;    	 break;    // 角度环左转,直到有指令修改Status
            case Car_Turn_R   : Car_Turn_R_Tick() ;   	 break;    // 角度环右转,直到有指令修改Status
            case Car_Turn_H   : Car_Turn_H_Tick() ;  	   break;    // half圈,也就是180度翻转
            case Car_Stop     : Car_Stop_Tick() ;		 		 break; 	 // 停车
        }
    }
    if (curr_Status != next_Status) // 这里一定要用if,因为条件改变是在==的条件下完成的,否则下一刻curr会与next相等,导致永远到不了!=     
    {
        switch (next_Status) // setup
        {
            case Car_Turn_F   : Car_Turn_F_Setup(); break;  // 路口直行
            case Car_Turn_L   : Car_Turn_L_Setup(); break;  // 左转
            case Car_Turn_R   : Car_Turn_R_Setup(); break;  // 右转
            case Car_Turn_H   : Car_Turn_H_Setup(); break;  // 180度旋转
            case Car_Stop     : Car_Stop_Setup()  ; break;  // 停车
        }
    }
    // 状态切换
    curr_Status = next_Status ;
}

//// 状态转换配置
//void Car_Control_Change(void)
//{
//	// 当前状态和下次状态相同才能进入切换状态
//	if (curr_Status != next_Status) {return;}
//	
//	// 开始控制状态转换
//	Motor_Pos_Update(&Motor_A);	// 更新位移
//	Motor_Pos_Update(&Motor_B);	// 更新位移
//	
//	if (curr_Status == Car_Stop)				// 1. 停止跳转
//	{
//		;	// 没啥事
//	}
//	else if(curr_Status == Car_Turn_F)	// 2. 直行跳转
//	{
//		if (Car_Turn_F_Is_Exit(50))
//		{
//			next_Status = Car_Turn_L ;
//		}
//	}
//	else if(curr_Status == Car_Turn_L)	// 3. 左转跳转
//	{
//		if (Car_Turn_L_Is_Exit())
//		{
//			next_Status = Car_Turn_F ;
//		}
//	}
//	else if(curr_Status == Car_Turn_R)	// 4. 右转跳转
//	{
//		if (Car_Turn_R_Is_Exit())
//		{
//			next_Status = Car_Stop ;
//		}
//	}
//	else if(curr_Status == Car_Turn_H)	// 5. 平转跳转
//	{
//		if (Car_Turn_H_Is_Exit())
//		{
////			next_Status =  ;
//		}
//	}
//}

int status_tmp = 0 ;
int Forward_Distance1 = 28 ;
int Forward_Distance2 = 28 ;

// 状态转换配置
void Car_Control_Change(void)
{
	// 当前状态和下次状态相同才能进入切换状态
	if (curr_Status != next_Status) {return;}
	
	// 开始控制状态转换
	Motor_Pos_Update(&Motor_A);	// 更新位移
	Motor_Pos_Update(&Motor_B);	// 更新位移
	
	if (curr_Status == Car_Stop)				// 1. 停止跳转
	{
		;	// 没啥事
	}
	else if(curr_Status == Car_Turn_F)	// 2. 直行跳转
	{
		if (Car_Turn_F_Is_Exit(Forward_Distance1) && status_tmp == 0)
		{
			next_Status = Car_Turn_R ;
			status_tmp ++ ;
		}
		else if (Car_Turn_F_Is_Exit(Forward_Distance2) && status_tmp == 1)
		{
			next_Status = Car_Stop ;
		}
	}
	else if(curr_Status == Car_Turn_L)	// 3. 左转跳转
	{
		if (Car_Turn_L_Is_Exit())
		{
			next_Status = Car_Turn_F ;
		}
	}
	else if(curr_Status == Car_Turn_R)	// 4. 右转跳转
	{
		if (Car_Turn_R_Is_Exit())
		{
			next_Status = Car_Turn_F ;
		}
	}
	else if(curr_Status == Car_Turn_H)	// 5. 平转跳转
	{
		if (Car_Turn_H_Is_Exit())
		{
//			next_Status =  ;
		}
	}
}





