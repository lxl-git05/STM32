#include "Mode_G.h"
#include "AllHeader.h"

//#define MPU6050_Check

int Base_Speed = 0 ;

Mode_Typedef curr_mode = Mode_Null   ;      // 当前模式
Mode_Typedef next_mode = Mode_Null  ;      // 下一个模式

// ========================== 系统setup loop ==========================

// 初始化
void Mode_G_Setup(void)
{
    // 全局初始化
    Initial_ALL() ;	
    // 定时器必须最后初始化!!!
    Initial_Timer() ;
}

// 循环loop
void Mode_G_Loop(void)
{
    // 检测程序是否可行
    if (Key_Check(KEY_0, KEY_SINGLE))// 单击
    {
        Flash_Mode_Set(Flash_Mode_Fast) ;  
        #ifdef MPU6050_Check
        MPU6050_Data_Error_Check(1000) ;
        #endif
    }
    // 进入下一个模式
    if (Key_Check(KEY_0, KEY_DOUBLE))// 双击
    {   
        Mode_To_Next() ;
    }
    // OLED展示
		OLED_Clear() ;
    if (curr_mode == Mode_Null) {OLED_Printf(0,0,OLED_6X8,"====Null====") ;}
		
		// 测试
		if (Key_Check(KEY_1,KEY_SINGLE))
		{
			next_Status = Car_Turn_F ;
		}
		OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
		OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
}

// ========================== 系统定时器配置 ==========================

// 1ms定时器
void Timer_1ms_Callback(void)
{
  // 功能1: 按键
	Key_Tick() ;
	// 功能2: LED闪烁指示灯
	Flash_Mode_Tick() ;
}

// 20ms定时器
void Timer_20ms_Callback(void)
{
	// 1. 电机速度更新与PID控制
//	PID_Angle_Tick(Base_Speed) ;
	Car_Control_Change() ;
	Car_Control() ;
	// 底层速度环
	Motor_Speed_Update_Tick(20) ;
	// 2. MPU6050更新参数
	#ifndef MPU6050_Check 
	MPU6050_Angle_Update_Tick() ;   // 耗时1.45ms
	#endif     
	// 3. 各个模式调试
	if (curr_mode == Mode_PID  ) {Mode_1_Tick() ;}
	if (curr_mode == Mode_Angle) {Mode_2_Tick() ;}
	if (curr_mode == Mode_Pos)   {Mode_4_Tick() ;}
}

// ========================== 系统状态配置 ==========================
// 进入下一状态
void Mode_To_Next(void)
{
    // Mode_End纯属标记模式尽头防止越界
    uint32_t next_val = (uint32_t)next_mode + 1;
    next_mode = (next_val == (uint32_t)Mode_End) ? Mode_Null : (Mode_Typedef)next_val;
}

// 将当前状态转换为:
void Mode_ChangeTo(Mode_Typedef nextmode)
{
    if (nextmode >= Mode_End) { return;}  

    next_mode = nextmode ;
}
