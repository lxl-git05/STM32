#ifndef USER_ALLHEADER_H
#define USER_ALLHEADER_H

/* CubeMX peripheral headers. */
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Use native FreeRTOS APIs for application RTOS operations. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

/* Add actual Hardware, Software and Function module headers here. */
#include "Initial.h"
#include "ISR.h"
#include "Msg.h"
#include "App_Task.h"

#endif /* USER_ALLHEADER_H */
