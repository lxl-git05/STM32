#include "Serial.h"
#include "usart.h"
#include "main.h"
#include "string.h"

// *************串口功能参数*************
uint8_t USART_RX_BUF[USART_RX_BUF_LEN] ;

int Serial_RxData ;	// 接收到的1个字节的数据
int Serial_RxFlag ;	// 接收到数据的标志位(注意不要直接引用这个,Serial_GetRxFlag才是正确的)

// *************状态机处理数据参数*************

// 状态机处理数据参数
int DataState = 0 ;
int isDataNumByte = 0 ;
// 接收数据数组
int TempArr[Data_Serial] ;	// 临时数据
int DataArr[Data_Serial]	;	// 正式数据

// 错误查询参数
int error_Serial ;	

// ************* 函数定义 *************

// 串口发送一个字节
void Serial_SendByte(uint8_t Byte)
{
	HAL_UART_Transmit_IT(&huart1, &Byte, 1);
	// 检查UART是否就绪,如果不等待肯定出问题
	while (huart1.gState != HAL_UART_STATE_READY) ;
}

// 串口发送数组
void Serial_SendArray(uint8_t *Array , uint16_t Length)
{
	uint16_t i ;
	for (i = 0 ; i < Length ; i ++)
	{
		Serial_SendByte(Array[i]) ;
	}
}

// 串口发送字符串
void Serial_SendString(char *String)
{
	uint8_t i ;
	for (i = 0 ; String[i] != 0 ; i ++)
	{
		Serial_SendByte(String[i]) ;
	}
}

// 指数运算
uint32_t Serial_Pow(uint32_t X , uint32_t Y)
{
	uint32_t Result = 1 ;
	while(Y--)
	{
		Result *= X ;
	}
	return Result ;
}

// 串口发送数字串
void Serial_SendNumber(uint32_t Number , uint8_t Length)
{
	uint8_t i ;
	for(i = 0 ; i < Length ; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10 , Length - i - 1) % 10 + '0') ;
	}
}

// 串口printf重定向*
int fputc(int ch , FILE *f)
{
	Serial_SendByte(ch) ;
	return ch ;
}

// 得到串口接收标志位*
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

// 得到串口接收数据
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;			//返回接收的数据变量
}

// 从高8位和低8位合成一个数据*
uint16_t Merge_2Bytes(uint8_t high, uint8_t low)
{
    return ((uint16_t)high << 8) | low;
}

// ************** 数据分配端	**************
void DataCheck(void)
{
	// 接收数据
	int RxData = Serial_RxData ;

	// 状态机判断数据个数变量
	static int DataNum = 0 ;			// 帧包的数据个数
	static int DataRealNum = 0 ;	// 真实的数据个数

 // 开始处理
 if (DataState == 0) // 任务: 等待帧头
 {
   // 操作:清空相关参数
   DataNum = 0 ;       // 帧包的数据个数
   DataRealNum = 0 ;   // 真实的数据个数
   isDataNumByte = 1 ; // 是否是显示数据个数的数位
	 
   // 实现任务:遇见0xFF(帧头)
   if (RxData == 0xFF)
   {
     DataState = 1 ;     // 状态转移
		 error_Serial = 0 ;	 // 报错机制,目前无错误
   }
	 else
	 {
		 error_Serial = 1 ;	// 错误1:帧头识别错误
		 // 清空数组
		 memset(TempArr, 0, sizeof(TempArr));
		 memset(DataArr, 0, sizeof(DataArr));
	 }
 }
 else if (DataState == 1)
 {
   // 实现任务:遇见0xFE(帧尾)  
   if (RxData == 0xFE)
   {
     // 开始处理(状态转移有2个方向)
     if (DataRealNum != DataNum) // 情况1.数据长度不正确,error
     {
       // 清空数组（全部置为0）
       memset(TempArr, 0, sizeof(TempArr));
			 memset(DataArr, 0, sizeof(DataArr));
       DataState = 0 ;
			 error_Serial = 2 ; // 错误2:数据接收长度不正确
     }
     else                        // 情况2.数据长度正确,great
     {
       // 临时数组数据结果处理转入正式数组
       DataArr[0] = DataNum ;  // 数据数量
       // 高低位合并
       for (int i = 0 , j = 1 ; i < DataNum ; i += 2 , j ++ )
       {
         DataArr[j] = Merge_2Bytes( TempArr[i] , TempArr[i + 1] ) ;
       }
       // 清空临时数组（全部置为0）
       memset(TempArr, 0, sizeof(TempArr));
       // 回到初始状态
			 error_Serial = 0 ;	// 本轮数据处理无错误
       DataState = 0 ;
     }
   }
   else  // 操作1:数据处理:放入临时array内,但是最后一位数据是帧尾,所以操作在后,任务在前
   {
     // 第一个数据为预期数据数
     if (isDataNumByte == 1)
     {
				DataNum = RxData ;  // 预期数据数量
				isDataNumByte = 0 ; // 后面的数据就不再是预期数据数了
     }
     else
     {
       TempArr[DataRealNum ++] = RxData ;
     }
   }
	 	// 操作2:判断是否溢出
	 if ( DataRealNum >= OutLen)  // 如果没有帧尾那就会溢出
   {
     // 清空临时数组（全部置为0）
     memset(TempArr, 0, sizeof(TempArr));
		 memset(DataArr, 0, sizeof(DataArr));
     // 回到初始状态,前面的if else白干
		 error_Serial = 3 ; 		// 错误3:数据帧尾格式错误,导致长度异常
     DataState = 0 ;
   }
 }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 取到接收到的字节
        uint8_t rx = USART_RX_BUF[0];
        
        // 接收数据处理
				Serial_RxData = rx ;
        Serial_RxFlag = 1 ;
				// 数据包处理
				DataCheck() ;
			
        // 再次打开中断接收
        HAL_UART_Receive_IT(&huart1, USART_RX_BUF, 1);
    }
}

int Serial_GetError(void)
{
	return error_Serial ;
}

// 数据处理(狮山,但是跑的通)
//void DataCheck(void)
//{
//	// 接收数据
//	int RxData = Serial_RxData ;
//	test[testNum++] = RxData ;	// 检查有没有正确接收

//	// 变量
//	static int DataNum = 0 ;			// 帧包的数据个数
//	static int DataRealNum = 0 ;	// 真实的数据个数

//	// 开始处理
//	switch (DataState)
//	{
//		// 等待帧头
//		case 0 :
//			// 清空相关参数
//			DataNum = 0 ;				// 数组展示的数据个数
//			DataRealNum = 0 ;		// 真实数据个数
//			isDataNumByte = 1 ;	// 是否是显示数据个数的数位
//			// 遇见0xFF(帧头)
//			if (RxData == 0xFF)
//			{
//				DataState = 1 ;
//				isDataNumByte = 1 ;	// 第一个数据为预期数据数
//			}
//			break ;
//		// 等待帧尾
//		case 1 :
//			if (DataRealNum == 8)	// 检测溢出
//			{
//				DataState = 3 ;
//			}
//			// 遇见帧尾
//			if (RxData == 0xFE)
//			{
//				DataState = 2 ;
////				break ;	// 不需要break,直接进入下一环
//			}
//			else
//			{
//				// 第一个数据为预期数据数
//				if (isDataNumByte == 1)
//				{
//					DataNum = RxData ;
//					isDataNumByte = 0 ;
//				}
//				else
//				{
//					TempArr[DataRealNum ++] = RxData ;
//				}
//				break ;
//			}
//		// 判断数据个数是否合格
//		case 2 :
//			if (DataRealNum != DataNum)	
//			{
//				DataState = 3 ;
//			}
//			else
//			{
//				DataState = 4 ;
//			}
////			break ;
//		// 数据合格,临时数据转为正式数据
//		case 4 :
//			if (DataState == 4)
//			{
//				DataArr[0] = DataNum ;
//				for (int i = 0 , j = 1 ; i < DataNum ; i += 2 , j ++ )
//				{
//					DataArr[j] = Merge_2Bytes( TempArr[i] , TempArr[i + 1] ) ;
//				}
//				DataState = 0 ;
//				break ;
//			}
//		// 数据不合格,清空临时arr,回到状态0
//		case 3 :
//			// 清空数组（全部置为0）
//			memset(TempArr, 0, sizeof(TempArr));
//			memset(DataArr, 0, sizeof(DataArr));
//			DataState = 0 ;
//			break ;
//		default :
//			break ;
//	}
//}


//// USART串口通信(暂时不用,可以用来检测串口有没有用上)
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if (huart->Instance == USART1)
//	{
//		// 1.添加结束符确保字符串安全
//		if (Size > 0 && Size < USART_RX_BUF_LEN) 
//		{
//			USART_RX_BUF[Size] = '\0';
//		}

//		// 2.使用字符串匹配代替固定长度检查
//		if (strstr((char *)USART_RX_BUF, "send") != NULL)
//		{
//			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//		}

//		// 3. 发送回显（添加\r\n+正确长度+状态检查）
//		const char *echo_str = "Hello!!!"; // 不换行的回显字符串
//		uint16_t echo_len = strlen(echo_str); // 计算长度
//	
//		if (huart1.gState == HAL_UART_STATE_READY) 
//		{ 
//			HAL_UART_Transmit_IT(&huart1, (uint8_t *)echo_str, echo_len); // 检查UART是否就绪
//		}

//		// 4.重启接收前清除缓存区
//		memset(USART_RX_BUF, 0, USART_RX_BUF_LEN);
//	
//		// 5.再次接收
//		HAL_UARTEx_ReceiveToIdle_IT(&huart1, USART_RX_BUF, USART_RX_BUF_LEN);
//	}
//}


