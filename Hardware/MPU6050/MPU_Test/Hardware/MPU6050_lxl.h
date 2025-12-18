#ifndef __MPU6050_LXL_H
#define __MPU6050_LXL_H

#include "main.h"

// 宏定义MPU6050的寄存器信息,使得更好理解
// 寄存器地址宏定义
#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG				0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I			0x75
// 内部参数定义


// 函数声明

// 初始化MPU6050相关配置
void MPU6050_Init(void);
// MPU6050写入数据
void MPU6050_WriteReg(uint8_t RegAddress , uint8_t Data);
// MPU6050读取数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress) ;
// MPU6050参数读取
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ) ;
// MPU6050得到器件ID
uint8_t MPU6050_GetID(void) ;

#endif
