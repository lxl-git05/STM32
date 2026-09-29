#ifndef __MYCAN_H
#define __MYCAN_H

#include "main.h"
#include "can.h"

typedef struct
{
	CAN_TxHeaderTypeDef TxMessage ;
	uint8_t Data[8] ;
} MyCAN_Tx_Msg ;

typedef struct
{
	CAN_RxHeaderTypeDef RxMessage ;
	uint8_t Data[8] ;
} MyCAN_Rx_Msg ;

/* Call after MX_CAN1_Init(); configure FIFO0 filters and start CAN1. */
void MyCAN_Init(void);
/* Standard data frame: ID <= 0x7FF, Length <= 8; foreground only, 100 ms timeout.
 * The tutorial's void interface does not report transmission success/failure. */
void MyCAN_Transmit(MyCAN_Tx_Msg* Tx_Msg) ;
/* Poll FIFO0 without consuming a message. */
uint8_t MyCAN_ReceiveFlag(void);
/* Data must hold 8 bytes. Remote frames/no message return Length = 0. */
HAL_StatusTypeDef MyCAN_Receive(MyCAN_Rx_Msg* Rx_Msg) ;


#endif
