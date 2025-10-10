#include "Serial.h"
#include "usart.h"

#include "string.h"
#include <stdarg.h>
#include <stdio.h>

// 串口处理信息变量
int Serial_New_Package[RX_Serial_LEN] ; // 正确信息存储数组长度管够,以后再改
// 数据包解析完成flag
int Serial_New_Package_Flag = 0 ;
// 错误查询参数
int error_Serial = 0 ;

// DMA串口接收变量
uint8_t RX_SerialArr[RX_Serial_LEN] ;	// 接收数组

// ********** 函数 **********
// 串口发送数组
void Serial_SendData_DMA(uint8_t *pData, uint16_t Size)
{
	// 确保DMA空闲（防止两次同时发）
	if (Serial_huart.gState == HAL_UART_STATE_READY)
	{
			HAL_UART_Transmit_DMA(&Serial_huart, pData, Size);
	}
}

// 从高8位和低8位合成一个数据*
uint16_t Merge_2Bytes(uint8_t high, uint8_t low)
{
    return ((uint16_t)high << 8) | low;
}

// 初步处理数据包(仅合并数据)
void Serial_Data_Deal(void)
{
	// 1. 第2个数据为数据长度(第一个为帧头),由于是高低位,所以除以2才是真正的数据长度
	Serial_New_Package[0] = RX_SerialArr[1] / 2;
	// 2. 存入数据
	for (int i = 2 , j = 1 ; i < 2 + RX_SerialArr[1] ; i += 2 , j ++)
	{
		Serial_New_Package[j] = Merge_2Bytes(RX_SerialArr[i] , RX_SerialArr[i + 1] ) ;
	}
}	

// 数据检测+存储核心函数
void Serial_Data_Check(void)
{
	// 1. 检测帧头合规性
	if (RX_SerialArr[0] != 0xFF)
	{
		error_Serial = 1 ;	// 错误1:帧头不合规
		// 操作:
		// 清空正式数组,后续调用会显示空数据
		memset(Serial_New_Package, 0, sizeof(Serial_New_Package));
		// 存储标志位置0
		Serial_New_Package_Flag = 0 ;
	}
	// 2. 借数据长度检测帧尾合规性
	else if (RX_SerialArr[RX_SerialArr[1] + 2] != 0xFE)
	{
		error_Serial = 2 ;	// 错误1:数据长度有问题或者帧尾空缺
		// 操作:
		// 清空正式数组,后续调用会显示空数据
		memset(Serial_New_Package, 0, sizeof(Serial_New_Package));
		// 存储标志位置0
		Serial_New_Package_Flag = 0 ;
	}
	else
	{
		// 初步处理数据包(仅合并数据)
		Serial_Data_Deal() ;
		// 无错误
		error_Serial = 0 ; 
		// 存储标志位置1
		Serial_New_Package_Flag = 1 ;
	}
}

// 得到串口接收标志位
uint8_t Serial_GetNewPackageFlag(void)
{
	if (Serial_New_Package_Flag == 1)			//如果标志位为1
	{
		Serial_New_Package_Flag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

// *串口空闲中断回调函数*
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == Serial_USART)
	{
		// 加上'\0'防止越界
		if (Size < RX_Serial_LEN)
		{
			RX_SerialArr[Size] = '\0';
		}
		
		// 清理缓冲区剩余数据，防止旧数据残留
    memset(RX_SerialArr + Size, 0, RX_Serial_LEN - Size);
		
		// *******************数据检测+存储端*******************
		Serial_Data_Check() ;
		
//		// 发送回显:debug时使用
//		HAL_UART_Transmit_DMA(&Serial_huart, (uint8_t *)RX_SerialArr, Size + 1 );
		
		// 每次处理完需要重新开启DMA中断
		HAL_UARTEx_ReceiveToIdle_DMA(&Serial_huart , RX_SerialArr , RX_Serial_LEN ) ;
	}
}
