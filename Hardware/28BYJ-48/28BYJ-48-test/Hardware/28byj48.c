#include "28byj48.h"
#include "tim.h"
// 步进电机方向定义
typedef enum
{
	BYJ_Shun ,	// 顺时针旋转
	BYJ_Ni			// 逆时针旋转
}BYJ_Pos_Typedef;
// 步进电机参数结构体
typedef struct
{
	BYJ_Pos_Typedef BYJ_Pos ;	// 电机旋转方向
	int RPM ;									// 电机目标转速
}BYJ_Typedef ;

// =============== 底层代码 ===============
// 高低电平设置
#define IN1(x) HAL_GPIO_WritePin(BYJ_Pin_1_GPIO_Port, BYJ_Pin_1_Pin, (GPIO_PinState)(x))
#define IN2(x) HAL_GPIO_WritePin(BYJ_Pin_2_GPIO_Port, BYJ_Pin_2_Pin, (GPIO_PinState)(x))
#define IN3(x) HAL_GPIO_WritePin(BYJ_Pin_3_GPIO_Port, BYJ_Pin_3_Pin, (GPIO_PinState)(x))
#define IN4(x) HAL_GPIO_WritePin(BYJ_Pin_4_GPIO_Port, BYJ_Pin_4_Pin, (GPIO_PinState)(x))

// 初始化驱动BYJ的定时器中断
void BYJ_Delay_Init(void)
{
	// setup启动定时器中断模式
	HAL_TIM_Base_Start_IT(&htim2);
}
// 配置步进电机的节拍
void BYJ_Set_Step(uint8_t IN1_State ,uint8_t IN2_State ,uint8_t IN3_State ,uint8_t IN4_State )
{
	// 配置节拍
	IN1(IN1_State);
  IN2(IN2_State);
  IN3(IN3_State);
  IN4(IN4_State);
}

// 28BYJ-48 半步驱动（8步序列）
void BYJ_Half_Drive(uint8_t step)
{
	switch(step)
  {
		case 0 :
			BYJ_Set_Step(1,0,0,0) ;
		break ;
		case 1 :
			BYJ_Set_Step(1,1,0,0) ;
		break ;
		case 2 :
			BYJ_Set_Step(0,1,0,0) ;
		break ;
		case 3 :
			BYJ_Set_Step(0,1,1,0) ;
		break ;
		case 4 :
			BYJ_Set_Step(0,0,1,0) ;
		break ;
		case 5 :
			BYJ_Set_Step(0,0,1,1) ;
		break ;
		case 6 :
			BYJ_Set_Step(0,0,0,1) ;
		break ;
		case 7 :
			BYJ_Set_Step(1,0,0,1) ;
		break ;
		default:
			BYJ_Set_Step(0,0,0,0) ;
		break ;
	}
}
// =============== 驱动控制代码 ===============
// 配置方向和速度



// TIM2定时器
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //判断是否是TIM2中断
  if (htim->Instance == TIM2)
  {
    // 执行功能:
		
  }
}



















