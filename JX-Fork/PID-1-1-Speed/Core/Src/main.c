/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// *******************库/函数导入*******************
// 系统库
#include <stdlib.h>
#include "string.h"
#include <stdio.h>
// 自设库
#include "OLED.h"
#include "Key.h"

// 江协PID额外需要的库
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "PID.h"
// *******************全局变量*******************
// 串口数据包
extern Serial_HEX_Data_Typedef   Serial_Hex_Data ;			// 解析好的HEX数据包
extern Serial_ABC_Data_Typedef   Serial_ABC_Data ;			// 解析好的ABC数据包

// PID相关代码
// 我的理解:setSpeed是设定的PWM值,这在PID调控中是幕后控制者,受到输出限幅,不会超过TIM的ARR值
// 而realSpeed是单位时间的脉冲数(在该份代码),所以并不是PWM,但是与PWM有很复杂的关系,所以只能通过PID调控
// goalSpeed是接口,使用者可以调节,其代表期待的单位时间的脉冲数

Pid_Typedef PID_p ;
int goalSpeed = 50 ;				// 目标速度
int setSpeed  ;		// 设定速度
int realSpeed ;				// 实际速度
float Kp = 0.30 ;		// 先设置Kp,等到波形在tar出现突变时没有出现set波形振动的临界值开始调节Ki
float Ki = 0.20 ;		// 结合Kp,使得set与tar越接近越好
float Kd = 0.00 ;			// 如果出现不正常超调曲线(趋势不正确):过冲,才开始调节Kd

// *******************实验区域*******************
int check1 ;
int check2 = 160 ;
int check[50] ;
extern float C ;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// ******************* setup *******************
	// 启动Systick时钟
	HAL_SYSTICK_Config(SystemCoreClock / 1000);
	// 初始化OLED
	OLED_Init() ;
	// 电机初始化
	Motor_Init() ;
	// 编码器初始化
	Encoder_Init() ;
	// 串口初始化
	Serial_Init(&Serial_huart) ;
	// PID_s初始化
	PID_Init(&PID_p , Kp , Ki , Kd , goalSpeed) ;
	// ******************* 实验区域 *******************

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// ******************* while *******************
		// 测试按键功能
		if (Key_Check(KEY_1 , KEY_SINGLE))
		{
			check1++ ;
		}
		else if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			goalSpeed = 160 ;
		}
		else if (Key_Check(KEY_3 , KEY_SINGLE))
		{
			goalSpeed = 0 ;
		}
		// 串口功能
		if (Serial_GetNewPackageFlag_ABC() == 1)
		{
			// 文本包调试程序
//			Serial_SetIntData("PWM" , "PWM=%d" , &setSpeed) ;
//			Serial_SetIntData("checkb" , "checkb=%d" , &check2) ;
			Serial_SetFloatData("Kp" , "Kp=%f" , &Kp) ;
			Serial_SetFloatData("Ki" , "Ki=%f" , &Ki) ;
			Serial_SetFloatData("Kd" , "Kd=%f" , &Kd) ;
			Serial_SetIntData("goalSpeed" , "goalSpeed=%d" , &goalSpeed) ;
			Serial_SetIntData("checkb" , "checkb=%d" , &check2) ;
		}
		
		// ******************* 实验区域 *******************
		OLED_Printf(0 ,0  , OLED_8X16 , "P:%.2f" , Kp) ;
		OLED_Printf(0 ,20 , OLED_8X16 , "I:%.2f" , Ki) ;
		OLED_Printf(0 ,40 , OLED_8X16 , "D:%.2f" , Kd) ;
		
		OLED_Printf(50 ,0  , OLED_8X16 , "Tar:%+03d" , goalSpeed) ;
		OLED_Printf(50 ,20 , OLED_8X16 , "Act:%+03d" , realSpeed) ;
		OLED_Printf(50 ,40 , OLED_8X16 , "Out:%+03d" , setSpeed ) ;
		
		PID_Set(&PID_p , Kp , Ki , Kd , goalSpeed) ;
		
		// 调节PWM
		if (check1 % 2 == 0)
		{
			Motor_SetPWM(setSpeed) ;
		}
		else
		{
		  Motor_SetPWM(goalSpeed) ;
		}
		
		// 发送数据
		Set_Current_USART(USART1_IDX) ;
		printf("%d,%d,%d,%d,%d,%d,%f\n",setSpeed,goalSpeed,realSpeed,check[0]*10,check[1]*10,check[2]*10,C*100) ;
		
		// 必须存在:OLED更新
		OLED_Update() ;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// Systick定时中断
void HAL_SYSTICK_Callback(void)
{
	// 计时
	static int count_sys = 0 ;
	count_sys ++ ;
	// 功能1:
	if (count_sys % 1000 == 0)
	{
//		HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin ) ;
	}
	// 功能2: 按键
	Key_Tick() ;
	// 功能3: 测速
	if (count_sys % 40 == 0)
	{
		realSpeed  = Encoder_Get() ;
		setSpeed   = PID_Cal(&PID_p , realSpeed , -100 , 100) ;
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
