#ifndef __HMC5883_H
#define __HMC5883_H
#include "main.h"
#include "MyI2C.h"
#include "HMC5883_Reg.h"

//#define HMC5883_IS_Soft_Drive	// 软硬驱动

#define HMC5883_ADDRESS 0x3C	// 这是含读写位和移位的地址写法, (0x1E << 1) | x 

#ifndef HMC5883_IS_Soft_Drive
extern I2C_HandleTypeDef hi2c2; // HAL库硬件IIC确定IIC的总线(I2C2)
static I2C_HandleTypeDef* hi2c_HMC5883 = &hi2c2;
#endif

// HMC5883 是 X Z Y
typedef struct
{
	// 原始数据
	int MagX_Raw ;
	int MagY_Raw ;
	int MagZ_Raw ;
	// 数据偏差
	int MagX_Offset ;
	int MagY_Offset ;
	int MagZ_Offset ;
	// 纠正后数据
	int MagX_Cali ;
	int MagY_Cali ;
	int MagZ_Cali ;
}HMC_Data_Typedef ;

// 数据引出
extern HMC_Data_Typedef HMC_Data;

// 初始化
void HMC_Init(void) ;
// 写入数据
void HMC5883_WriteReg(uint8_t RegAddress , uint8_t Data);
// 读取数据
uint8_t HMC5883_ReadReg(uint8_t RegAddress);
// 数据更新,859us更新一次
void HMC5883_Update_Data(void) ;
// 数据校准
void HMC5883_Calibrate(void) ;
#endif
