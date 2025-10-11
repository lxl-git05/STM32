#include "Serial.h"
#include "usart.h"

#include "string.h"
#include <stdarg.h>
#include <stdio.h>


// DMA串口接收变量
uint8_t RX_SerialArr[RX_Serial_LEN] ;	// 接收数组

// **********HEX模式检测:**********
// 串口处理信息变量
int Serial_New_Package_HEX[RX_Serial_LEN] ; // 正确信息存储数组长度管够,以后再改
// 数据包解析完成flag
int Serial_New_Package_HEX_Flag = 0 ;
// 错误查询参数
int error_Serial_HEX = 0 ;

// **********文本(ABC)模式检测:**********
// 串口处理信息变量
char Serial_New_Package_ABC[RX_Serial_LEN] ; // 正确信息存储数组长度管够,以后再改
// 数据包解析完成flag
int Serial_New_Package_ABC_Flag = 0 ;
// 错误查询参数
int error_Serial_ABC = 0 ;


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

// HEX:初步处理数据包(仅合并数据)
void Serial_Data_Deal_HEX(void)
{
	// 1. 第2个数据为数据长度(第一个为帧头),由于是高低位,所以除以2才是真正的数据长度
	Serial_New_Package_HEX[0] = RX_SerialArr[1] / 2;
	// 2. 存入数据
	for (int i = 2 , j = 1 ; i < 2 + RX_SerialArr[1] ; i += 2 , j ++)
	{
		Serial_New_Package_HEX[j] = Merge_2Bytes(RX_SerialArr[i] , RX_SerialArr[i + 1] ) ;
	}
}	

// HEX:数据检测+存储核心函数
void Serial_Data_Check_HEX(void)
{
	// 判断模式,如果是文本模式就退出,给文本检测函数操作
	if (RX_SerialArr[0] == '@')
	{
		return ;
	}
	// 1. 检测帧头合规性
	if (RX_SerialArr[0] != 0xFF)
	{
		error_Serial_HEX = 1 ;	// 错误1:帧头不合规
		// 操作:
		// 清空正式数组,后续调用会显示空数据
		memset(Serial_New_Package_HEX, 0, sizeof(Serial_New_Package_HEX));
		// 存储标志位置0
		Serial_New_Package_HEX_Flag = 0 ;
	}
	// 2. 借数据长度检测帧尾合规性
	else if (RX_SerialArr[RX_SerialArr[1] + 2] != 0xFE)
	{
		error_Serial_HEX = 2 ;	// 错误1:数据长度有问题或者帧尾空缺
		// 操作:
		// 清空正式数组,后续调用会显示空数据
		memset(Serial_New_Package_HEX, 0, sizeof(Serial_New_Package_HEX));
		// 存储标志位置0
		Serial_New_Package_HEX_Flag = 0 ;
	}
	else
	{
		// 初步处理数据包(仅合并数据)
		Serial_Data_Deal_HEX() ;
		// 无错误
		error_Serial_HEX = 0 ; 
		// 存储标志位置1
		Serial_New_Package_HEX_Flag = 1 ;
	}
}

// HEX:得到串口接收标志位
uint8_t Serial_GetNewPackageFlag_HEX(void)
{
	if (Serial_New_Package_HEX_Flag == 1)			//如果标志位为1
	{
		Serial_New_Package_HEX_Flag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}
// HEX:得到错误原因
int Serial_GetError_HEX(void)
{
	return error_Serial_HEX ;
}

// 文本:数据检测+存储+数据处理(仅保留文本)核心函数
void Serial_Data_Check_ABC(void)
{
	// 判断模式,如果是HEX模式就退出,给HEX检测函数操作
	if (RX_SerialArr[0] == 0xFF)
	{
		return ;
	}
	// 开始检测文本
	// 1. 检测数据包帧头是否错误
	else if (RX_SerialArr[0] != '@')
	{
		error_Serial_ABC = 1 ; // 错误1:帧头不合规
	}
	// 2. 开始一边处理数据一边检测帧尾,从1开始(跳过帧头)
	else
	{
		int i = 0 ;
		for (i = 1 ; RX_SerialArr[i+1] != '$' ; i++)
		{
			Serial_New_Package_ABC[i-1] = RX_SerialArr[i] ;
			// 检测是否溢出
			if (i > 20)
				break ;
		}
		// 补1位!!!,毕竟无论如何都是被迫离开for,少了1位
		Serial_New_Package_ABC[i-1] = RX_SerialArr[i] ;	
		// 情况1:退出for是因为检测到了帧尾
		if (RX_SerialArr[i+1] == '$')
		{
			// 3. 开始检测第2个帧尾
			if (RX_SerialArr[i+2] != '#')
			{
				error_Serial_ABC = 3 ; // 错误3:第2个帧尾不合规
				memset(Serial_New_Package_ABC, 0, sizeof(Serial_New_Package_ABC));	// 清空记录数据
				return ;
			}
			else
			{
				// 初步处理数据包(仅合并数据)
				Serial_New_Package_ABC[i] = '\0' ;	// 加个结尾符号
				// Serial_Data_Deal_ABC() ;
				// 无错误
				error_Serial_ABC = 0 ; 
				// 存储标志位置1
				Serial_New_Package_ABC_Flag = 1 ;
			}
		}
		// 情况2:退出for是因为溢出了,说明第1个帧尾没有检测到
		else
		{
			error_Serial_ABC = 2 ; // 错误2:第1个帧尾不合规
			memset(Serial_New_Package_ABC, 0, sizeof(Serial_New_Package_ABC));	// 清空记录数据
			return ;
		}
	}
}

// 文本:得到串口接收标志位
uint8_t Serial_GetNewPackageFlag_ABC(void)
{
	if (Serial_New_Package_ABC_Flag == 1)			//如果标志位为1
	{
		Serial_New_Package_ABC_Flag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

// 文本:得到错误原因
int Serial_GetError_ABC(void)
{
	return error_Serial_ABC ;
}

// 文本:1. 封装一个函数,实现简易浮点数变量调试
bool Serial_SetFloatData( char *KeyWord , char *cmd , float *Data)
{
	// KeyWord为关键词,有别与别的指令 cmd为整句话,包含%f等,VOFA怎么写这里也怎么写 Data为接收改变量的变量
	// 浮点数加上%f就行,位数不用管,空格也需要注意,具体协议还得看VOFA怎么输出的
	// 例:	建议VOFA发送:	@Kp=%.2f$#	串口接收:	Serial_SetFloatData("Kp" , "Kp=%f" , &test1) ;
	if ( strstr(Serial_New_Package_ABC , KeyWord) != NULL )
	{
		sscanf(Serial_New_Package_ABC, cmd , Data);
		return true ;
	}
	else
	{
		return false ;
	}
}

// 文本:2. 封装一个函数,实现简易整数变量调试
bool Serial_SetIntData( char *KeyWord , char *cmd , int *Data)
{
	// KeyWord为关键词,有别与别的指令 cmd为整句话,包含%d等,VOFA没有%d,所以VOFA写%.0f即可代表%d Data为接收改变量的变量
	// 整数加上%d即可,空格也需要注意,具体协议还得看VOFA怎么输出的
	// 例:	建议VOFA发送:	@test=%.0f$#	串口接收:	Serial_SetIntData("test" , "test=%d" , &check1) ;
	if ( strstr(Serial_New_Package_ABC , KeyWord) != NULL )
	{
		sscanf(Serial_New_Package_ABC, cmd , Data);
		return true ;
	}
	else
	{
		return false ;
	}
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
		// HEX模式检测
		Serial_Data_Check_HEX() ;
		Serial_Data_Check_ABC() ;
		// 发送回显:debug时使用
//		HAL_UART_Transmit_DMA(&Serial_huart, (uint8_t *)RX_SerialArr, Size + 1 );
		
		// 每次处理完需要重新开启DMA中断
		HAL_UARTEx_ReceiveToIdle_DMA(&Serial_huart , RX_SerialArr , RX_Serial_LEN ) ;
	}
}
