#include "MPU6050_lxl.h"
#include "MyI2C.h"

#define MPU6050_ADDRESS 0xD0	// 这是含读写位和移位的地址写法, (0xDx = 0x68 << 1) | x 

#define MPU6050_IS_Soft_Drive	// 软件模拟IIC,如果注释掉就是硬件IIC

#ifndef MPU6050_IS_Soft_Drive
extern I2C_HandleTypeDef hi2c2; // HAL库硬件IIC确定IIC的总线(I2C1)
static I2C_HandleTypeDef* hi2c_MPU6050 = &hi2c2;
#endif

// 写入数据
void MPU6050_WriteReg(uint8_t RegAddress , uint8_t Data)
{
	// 软件I2C
	#ifdef MPU6050_IS_Soft_Drive
	// 开始信号
	MyI2C_Start() ;	
	// 第1个字节(从机地址)
	MyI2C_SendByte(MPU6050_ADDRESS) ;	// 定位从机地址
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	
	// 第2个字节(从机寄存器地址)
	MyI2C_SendByte(RegAddress) ;			// 定位从机寄存器地址
	MyI2C_ReceiveAck() ;	// 得到应答位
	
	// 第3个字节(写入的数据),可for_loop从数组中写入多个字节
	MyI2C_SendByte(Data) ;			// 写入的数据
	MyI2C_ReceiveAck() ;	// 得到应答位
	// 停止信号
	MyI2C_Stop() ;
	#else
	// HAL库硬件IIC
	//或者直接指定地址写：
	HAL_I2C_Mem_Write(hi2c_MPU6050, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, 10000);
	#endif
}

// 读取数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	// 读取的数据
	uint8_t Data ;
	
//	IIC软件
	#ifdef MPU6050_IS_Soft_Drive
	// 开始信号
	MyI2C_Start() ;	
	// 第1个字节(从机地址)
	MyI2C_SendByte(MPU6050_ADDRESS) ;	// 定位从机地址为写
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	// 第2个字节(从机寄存器地址)
	MyI2C_SendByte(RegAddress) ;			// 定位从机寄存器地址
	MyI2C_ReceiveAck() ;	// 得到应答位
	
	// 重新开始信号(Sr)
	MyI2C_Start() ;	
	// 再次第1个字节(从机地址)
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01) ;	// 定位从机地址为读
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	
	// 接收1个字节,可for_loop接收多个字节存在数组里面
	Data = MyI2C_ReceiveByte() ;
	MyI2C_SendAck(1) ; 		// 发送应答为:无应答,使得从机交出SDA控制权
	// 停止信号
	MyI2C_Stop() ;	
	// HAL库硬件IIC
	#else
	HAL_I2C_Mem_Read(hi2c_MPU6050, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, 10000);
	#endif
	// 返回数据
	return Data ;
}

// 初始化MPU6050相关配置
void MPU6050_Init(void)
{
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		// 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		// 电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		// 采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);				// 配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	// 陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	// 加速度计配置寄存器，选择满量程为±16g
}

// 连续读取6个参数,更快.只需要1.5ms 5.7ms
void MPU6050_GetData(
    int16_t *AccX, int16_t *AccY, int16_t *AccZ,
    int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	// 数据接收区,连续读取
	uint8_t buf[14];
	// 软件连续读取
	#ifdef MPU6050_IS_Soft_Drive
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();

	MyI2C_SendByte(MPU6050_ACCEL_XOUT_H);
	MyI2C_ReceiveAck();

	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	// 读取
	for(uint8_t i = 0; i < 14; i++)
	{
			buf[i] = MyI2C_ReceiveByte();

			if(i == 13)
					MyI2C_SendAck(1);   // NACK
			else
					MyI2C_SendAck(0);   // ACK
	}
	// 停止
	MyI2C_Stop();
	#else
	HAL_I2C_Mem_Read(hi2c_MPU6050,MPU6050_ADDRESS,MPU6050_ACCEL_XOUT_H,I2C_MEMADD_SIZE_8BIT,buf,14,1000);
	#endif
	
	// 数据处理
	// 得到加速度
	*AccX  = (int16_t)(buf[0] << 8 | buf[1]);
	*AccY  = (int16_t)(buf[2] << 8 | buf[3]);
	*AccZ  = (int16_t)(buf[4] << 8 | buf[5]);
	// 得到角速度(跳过了温度)
	*GyroX = (int16_t)(buf[8]  << 8 | buf[9]);
	*GyroY = (int16_t)(buf[10] << 8 | buf[11]);
	*GyroZ = (int16_t)(buf[12] << 8 | buf[13]);
}


uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I) ;
}

/*
// MPU6050获取数据,一个一个读,慢
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}
*/
