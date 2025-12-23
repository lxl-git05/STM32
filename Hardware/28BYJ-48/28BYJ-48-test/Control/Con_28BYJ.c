#include "Con_28BYJ.h"		// 控制逻辑代码
#include "tim.h"					// 定时器

// 创建实例
BYJ_Typedef BYJ ;

// 初始化
void Con_BYJ_Init(void)
{
	BYJ_Init(&BYJ , BYJ_Pin_1_GPIO_Port , BYJ_Pin_1_Pin , 
									BYJ_Pin_2_GPIO_Port , BYJ_Pin_2_Pin , 
									BYJ_Pin_3_GPIO_Port , BYJ_Pin_3_Pin , 
									BYJ_Pin_4_GPIO_Port , BYJ_Pin_4_Pin ) ;
}

// 配置方向
void Con_BYJ_Set_Position(BYJ_Pos_Typedef Pos)
{
	BYJ_Set_Position(&BYJ , Pos) ;
}
// 配置速度
void Con_BYJ_Set_RPM(int RPM)
{
	BYJ_Set_RPM(&BYJ , RPM) ;
}
// 停止
void Con_BYJ_Stop(void) 
{
	BYJ_Stop(&BYJ) ;
}
// 驱动目标角度
void Con_BYJ_Set_Tar_Angle(int Set_RPM ,int Goal_Angle)
{
	BYJ_Set_Target_Angle(&BYJ , Set_RPM , Goal_Angle ) ;
}

// 在中断驱动BYJ
void Con_BYJ_Drive(void) 
{
	BYJ_Drive(&BYJ) ;
}
// BYJ_TIM定时器，用来修改节拍，8拍模式
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //判断是否是BYJ_TIM中断,当前默认是1ms执行一次
  if (htim->Instance == BYJ_TIM)
  {
    Con_BYJ_Drive() ;
  }
}
