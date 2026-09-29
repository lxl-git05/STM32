#ifndef __MYCAN_H
#define __MYCAN_H

#include "main.h"
#include "can.h"

/* Call after MX_CAN1_Init(); configure FIFO0 filters and start CAN1. */
void MyCAN_Init(void);
/* Standard data frame: ID <= 0x7FF, Length <= 8; foreground only, 100 ms timeout.
 * The tutorial's void interface does not report transmission success/failure. */
void MyCAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data);
/* Poll FIFO0 without consuming a message. */
uint8_t MyCAN_ReceiveFlag(void);
/* Data must hold 8 bytes. Remote frames/no message return Length = 0. */
void MyCAN_Receive(uint32_t *ID, uint8_t *Length, uint8_t *Data);


#endif
