#include "MPU6050_Angle.h"
#include "math.h"

// 全局变量
ImuOffset_Typedef  MPU_Offset;			// 误差纠正参数
ImuCali_Typedef		 MPU_Cali	 ;			// 纠正后的数据
ImuReal_Typedef 	 MPU_Real  ;			// 最终的确定角度
							
// 积分后的角度
float gyroAngleX = 0.0f ;
float gyroAngleY = 0.0f ;

// 自动纠正误差(可配合按键使用)
void MPU6050_Data_Error_Check(int Sample_Cnt)
{
    if (Sample_Cnt < 200 || Sample_Cnt > 2000)
        Sample_Cnt = 1000;  // 默认1000次，更稳定

    float sum_ax = 0.0f, sum_ay = 0.0f, sum_az = 0.0f;
    float sum_gx = 0.0f, sum_gy = 0.0f, sum_gz = 0.0f;

    for (int i = 0; i < Sample_Cnt; i++)
    {
        MPU6050_Update_Data();

        sum_ax += MPU_Raw_Data.AX;
        sum_ay += MPU_Raw_Data.AY;
        sum_az += MPU_Raw_Data.AZ;
        sum_gx += MPU_Raw_Data.GX;
        sum_gy += MPU_Raw_Data.GY;
        sum_gz += MPU_Raw_Data.GZ;
    }

    // 原始数据零偏（g 和 °/s）
    MPU_Offset.AccErrorX = sum_ax / Sample_Cnt;
    MPU_Offset.AccErrorY = sum_ay / Sample_Cnt;
    // 可选：计算AZ相对于1g的偏移（小偏移校正）
    MPU_Offset.AccErrorZ = sum_az / Sample_Cnt - 1.0f;  // 只减小偏差，不减重力

    MPU_Offset.GyroErrorX = sum_gx / Sample_Cnt;
    MPU_Offset.GyroErrorY = sum_gy / Sample_Cnt;
    MPU_Offset.GyroErrorZ = sum_gz / Sample_Cnt;

    // 初始化姿态
    MPU_Real.roll  = 0.0f;
    MPU_Real.pitch = 0.0f;
    MPU_Real.yaw   = 0.0f;

    MPU_Real.AccX = 0.0f;
    MPU_Real.AccY = 0.0f;
    MPU_Real.AccZ = 1.0f;  // 重点
}
// 减去误差后的归零数据
void MPU6050_Raw_Error_Update(void)
{	
	// 得到归零数据
	MPU_Cali.AX = MPU_Raw_Data.AX - MPU_Offset.AccErrorX ;
	MPU_Cali.AY = MPU_Raw_Data.AY - MPU_Offset.AccErrorY ;
	MPU_Cali.AZ = MPU_Raw_Data.AZ - MPU_Offset.AccErrorZ ;
	
	MPU_Cali.GX = MPU_Raw_Data.GX - MPU_Offset.GyroErrorX ;
	MPU_Cali.GY = MPU_Raw_Data.GY - MPU_Offset.GyroErrorY ;
	MPU_Cali.GZ = MPU_Raw_Data.GZ - MPU_Offset.GyroErrorZ ;
}

// 数据深度处理
void MPU6050_Raw_Deal(int Deal_dt_ms)
{
	// 根据加速度得到静态角度
	// 计算翻滚角（绕 X 轴）— 使用 Y 和 Z 轴的加速度值
	float accAngleX = atan2(MPU_Cali.AY, MPU_Cali.AZ)  * 180 / 3.14159265358 ;
	// 计算俯仰角（绕 Y 轴）— 使用 X 和 Z 轴的加速度值
	float accAngleY = atan2(-MPU_Cali.AX, MPU_Cali.AZ) * 180 / 3.14159265358 ;
	
	// 得到粗略积分角度
	gyroAngleX += MPU_Cali.GX * Deal_dt_ms * 1.0 / 1000;
	gyroAngleY += MPU_Cali.GY * Deal_dt_ms * 1.0 / 1000;
	
	// 得到偏航角yaw
//	MPU_Real.yaw += MPU_Cali.GZ * Deal_dt_ms * 1.0 / 1000;
	
	// 得到互补滤波角度
	gyroAngleX = 0.98 * gyroAngleX + 0.02 * accAngleX;
	gyroAngleY = 0.98 * gyroAngleY + 0.02 * accAngleY;
	
	// 得到Roll和Pitch
	MPU_Real.roll  = gyroAngleX ;
	MPU_Real.pitch = gyroAngleY;
}
