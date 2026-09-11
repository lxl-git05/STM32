#include "AllHeader.h"

void Mode_1_Setup(void)
{
	HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin , GPIO_PIN_RESET) ;	// ¥Úø™LED
}

// 1. ADC≤‚ ‘
void Mode1_RP_Check(void)
{
	// 1. ADC≤‚ ‘
	OLED_Printf(0,20,OLED_6X8,"ADC1=%d",RP_GetValue(1)) ;
	OLED_Printf(0,30,OLED_6X8,"ADC2=%d",RP_GetValue(2)) ;
	OLED_Printf(0,40,OLED_6X8,"ADC3=%d",RP_GetValue(3)) ;
	OLED_Printf(0,50,OLED_6X8,"ADC4=%d",RP_GetValue(4)) ;
}

// 2. Motor≤‚ ‘
int PWM_Check = 0 ;
void Mode1_Motor_Check(void)
{
	OLED_Printf(0,20,OLED_6X8,"PWM=%d",PWM_Check) ;
	Motor_SetPWM(PWM_Check) ;
}

// 3. ±‡¬Î∆˜≤‚ ‘
void Mode1_Encoder_Check(void)
{
	OLED_Printf(0,20,OLED_6X8,"Encoder:%d",Encoder_Get()) ;
}

// ¥˙¬Î≤‚ ‘
void Mode_1_Loop(void)
{
	
	OLED_Printf(0,0,OLED_8X16,"===Mode1===") ;
	// 1. ADC≤‚ ‘
//	Mode1_RP_Check() ;
	// 2. Motor≤‚ ‘
//	Mode1_Motor_Check() ;
	// 3. ±‡¬Î∆˜≤‚ ‘
	Mode1_Encoder_Check() ;
}

void Mode_1_Tick(void)
{
	
}

void Mode_1_Exit(void)
{
	
}
