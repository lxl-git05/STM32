#ifndef __AD_H
#define __AD_H

#include "main.h"
#include "adc.h"

#define AD_ERROR_VALUE 0xFFFFU

void AD_Init(void) ;

uint16_t AD_GetValue(void) ;

#endif
