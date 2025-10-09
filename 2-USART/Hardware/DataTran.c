#include "DataTran.h"
#include "string.h"
// 数据接收参数
extern int Serial_RxData ;	// 得到接收的数据
extern int Serial_RxFlag ;	// 判断是否有数据

// 状态机处理数据参数
//int DataState = 0 ;
//int isDataNumByte = 0 ;
//// 接收数据数组
//int TempArr[50] ;	// 临时数据
//int DataArr[50]	;	// 正确数据

extern int check[] ;
extern int check2 ;

/*
数据要求:
	发送来的数据是可变长度的数据,并且可以较大
帧协议:
	帧头(0xFF) 数据个数(不含头尾和本身)	数据(可变长度)	 帧尾(0xFE)
*/
// ************** 数据收发端 **************
// 发送一个字节数据
//void DataSendByte(uint8_t Byte)
//{
//	Serial_SendByte(Byte) ;
//}

//// 判断是否有数据
//int IsDataGet(void)
//{
//	return Serial_GetRxFlag() ;
//}

//// 得到接收的数据
//int DataGetByte(void)
//{
//	return Serial_RxData ;
//}

//// ************** 数据初步处理端	**************
//// 获取高8位
//uint8_t Get_HighByte(uint16_t value)
//{
//    return (uint8_t)((value >> 8) & 0xFF);
//}

//// 获取低8位
//uint8_t Get_LowByte(uint16_t value)
//{
//    return (uint8_t)(value & 0xFF);
//}

//// 从高8位和低8位合成一个数据
//uint16_t Merge_2Bytes(uint8_t high, uint8_t low)
//{
//    return ((uint16_t)high << 8) | low;
//}

//// ************** 数据分配端	**************
//void DataCheck(void)
//{
//	// 接收数据
//	int RxData = Serial_RxData ;
// // *检查有没有正确接收*
//	test[testNum++] = RxData ;	

//	// 状态机判断数据个数变量
//	static int DataNum = 0 ;			// 帧包的数据个数
//	static int DataRealNum = 0 ;	// 真实的数据个数

// // 开始处理
// if (DataState == 0) // 任务: 等待帧头
// {
//   // 操作:清空相关参数
//   DataNum = 0 ;       // 帧包的数据个数
//   DataRealNum = 0 ;   // 真实的数据个数
//   isDataNumByte = 1 ; // 是否是显示数据个数的数位

//   // 实现任务:遇见0xFF(帧头)
//   if (RxData == 0xFF)
//   {
//     DataState = 1 ;     // 状态转移
//   }
// }
// else if (DataState == 1)
// {
//   // 实现任务:遇见0xFE(帧尾)  
//   if (RxData == 0xFE)
//   {
//     // 开始处理(状态转移有2个方向)
//     if (DataRealNum != DataNum) // 1.数据长度不正确,error
//     {
//       // 清空数组（全部置为0）
//       memset(TempArr, 0, sizeof(TempArr));
//	   memset(DataArr, 0, sizeof(DataArr));
//       DataState = 0 ;
//     }
//     else                        // 2.数据长度正确,great
//     {
//       // 临时数组数据结果处理转入正式数组
//       DataArr[0] = DataNum ;  // 数据数量
//       // 高低位合并
//       for (int i = 0 , j = 1 ; i < DataNum ; i += 2 , j ++ )
//       {
//         DataArr[j] = Merge_2Bytes( TempArr[i] , TempArr[i + 1] ) ;
//       }
//       // 清空临时数组（全部置为0）
//       memset(TempArr, 0, sizeof(TempArr));
//       // 回到初始状态
//       DataState = 0 ;
//     }
//   }
//   else if ( DataRealNum >= 10)  // 如果没有帧尾那就会溢出
//   {
//     // 清空临时数组（全部置为0）
//     memset(TempArr, 0, sizeof(TempArr));
//     // 回到初始状态
//     DataState = 0 ;
//   }
//   else  // 操作:数据处理:放入临时array内,但是最后一位数据是帧尾,所以操作在后,任务在前
//   {
//     // 第一个数据为预期数据数
//     if (isDataNumByte == 1)
//     {
//       DataNum = RxData ;  // 预期数据数量
//				isDataNumByte = 0 ; // 后面的数据就不再是预期数据数了
//     }
//     else
//     {
//       TempArr[DataRealNum ++] = RxData ;
//     }
//   }
// }
//}

//// 数据处理
//void DataCheck(void)
//{
//	// 接收数据
//	int RxData = Serial_RxData ;

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
//			// 遇见帧尾
//			if (RxData == 0xFE)
//			{
//				DataState = 2 ;
//			}
//			// 第一个数据为预期数据数
//			if (isDataNumByte == 1)
//			{
//				DataNum = RxData ;
//				isDataNumByte = 0 ;
//			}
//			else
//			{
//				TempArr[DataRealNum++] = RxData ;
//			}
//			break ;
//		// 判断数据个数是否合格
//		case 2 :
//			if (DataRealNum + 1 != DataNum)	// DataRealNum从0开始,所以要+1
//			{
//				DataState = 3 ;
//			}
//			else
//			{
//				DataState = 4 ;
//			}
//			break ;
//		// 数据不合格,清空临时arr,回到状态0
//		case 3 :
//			// 清空数组（全部置为0）
//			memset(TempArr, 0, sizeof(TempArr));
//			DataState = 0 ;
//			break ;
//		// 数据合格,临时数据转为正式数据
//		case 4 :
//			DataArr[0] = DataNum ;
//			for (int i = 0 , j = 1 ; i < DataNum ; i += 2 , j ++ )
//			{
//				DataArr[j] = Merge_2Bytes( TempArr[i] , TempArr[i + 1] ) ;
//			}
//			break ;
//		default :
//			break ;
//	}
//}

