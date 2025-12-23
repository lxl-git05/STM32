#ifndef __MPU6050_ANGLE_H
#define __MPU6050_ANGLE_H

#include "MPU6050_lxl.h"

// 角度和加速度误差值
typedef struct
{
	float AccErrorX	 ;	// 加速度误差
	float AccErrorY	 ;	// 加速度误差
	float AccErrorZ	 ;	// 加速度累次平均值,水平放置时应该是1g,所以不进行零漂处理!
	
	float	GyroErrorX ;	// 角速度误差 
	float	GyroErrorY ;	// 角速度误差
	float GyroErrorZ ;	// 角速度误差
}ImuOffset_Typedef;

// 得到的零偏纠正值
typedef struct
{
	float AX ;	// 加速度
	float AY ;	// 加速度
	float AZ ;	// 加速度,由于别的数据处理都是使用去零漂的数据,所以这里不删除了,但是事实上就是raw
	
	float	GX ;	// 角速度
	float	GY ;	// 角速度
	float GZ ;	// 角速度
}ImuCali_Typedef;

// 得到的真实数据
typedef struct
{
	float	AccX ;	  // 加速度x 
	float	AccY ;	  // 加速度y
	float AccZ ;		// 加速度z
	
	float	roll ;	// 角度x 
	float	pitch ;	// 角度y
	float yaw ;		// 角度z
}ImuReal_Typedef ;			// 真实角度值

// 引出参数
extern ImuOffset_Typedef  MPU_Offset;			// 误差纠正参数
extern ImuCali_Typedef	  MPU_Cali	 ;		// 纠正后的数据
extern ImuReal_Typedef 	  MPU_Real  ;			// 最终的确定角度

// ************函数************
// 自动纠正误差(可配合按键使用)
void MPU6050_Data_Error_Check(int Sanple_Cnt) ;
// 减去误差后的数据
void MPU6050_Raw_Error_Update(void) ;
// 数据深度处理
void MPU6050_Raw_Deal(int Deal_dt_ms) ;


#endif
