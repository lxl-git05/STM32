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
#include "Serial.h"
#include "PID.h"
// *******************全局变量*******************
// PID + VOFA+调试代码
//#define PID_VOFA	// 记得打开文本模式的函数哦
// 接收状态和缓冲区
extern uint8_t USART_RX_BUF[] ;
extern int DataArr[] ;
// *******************实验区域*******************
int check1 ;
int check2 ;
int check[100] ;

// 江协版 文本:
extern char Serial_RxPacket[] ;

#ifdef PID_VOFA
// 实验:PID + VOFA
Pid_Typedef PID ;
float Kp;			// 比例系数				0.7
float Ki;			// 积分系数				0.05
float Kd;			// 微分系数				0.01
float goalPoint = 5.0 ;
float setPoint ;
float realPoint ;
#endif
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// ******************* setup *******************
	// 启动Systick时钟
	HAL_SYSTICK_Config(SystemCoreClock / 1000);
	// 初始化OLED
	OLED_Init() ;
	OLED_ShowString(0 , 0 , "Hello World" , OLED_8X16 ) ;
	
//  // 接收中断初始化
//	HAL_UARTEx_ReceiveToIdle_IT(&huart1, USART_RX_BUF, USART_RX_BUF_LEN);
	
	// 单字符接收中断初始化
	HAL_UART_Receive_IT(&huart1, USART_RX_BUF, 1);  // 每次接收1字节
	
	// ******************* 实验区域 *******************
	#ifdef PID_VOFA
	PID_Init(&PID , Kp , Ki , Kd , goalPoint ) ;
	#endif
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
			HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
			// 一旦数据发送失败,可以按按键检查出错原因
			printf("\r\nError : %d \r\n" , Serial_GetError() ) ;
		}
		
		// ******************* 实验区域 *******************
		#ifdef PID_VOFA
		realPoint = setPoint * 1.2f ;
		#endif
		
		// 展示接收的数据
		if (Serial_GetRxFlag() == 1)
		{
			// 自己版测试:
//			printf("\r\nDataNum: %d \r\n" , DataArr[0]) ;
//			int i ;
//			for(i = 1 ; i < 8 ; i ++)
//			{
//				printf("%d " , DataArr[i]) ;
//			}
//			printf("%d \n" , DataArr[i]) ;
			// 江协 文本版 测试
//			printf("Text \n") ;
//			if (strcmp(Serial_RxPacket, "LED_Change") == 0)
//			{
//				HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
//			}
//			printf("%s\n" , Serial_RxPacket) ;
			// 江协 文本版 PID测试
				#ifdef PID_VOFA
				if ( strstr(Serial_RxPacket , "Kp") != NULL )
				{
					sscanf(Serial_RxPacket, "Kp=%f\n", &Kp);
					printf("Kp = %.2f\n" , Kp) ;
				}
				else if ( strstr(Serial_RxPacket , "Ki") != NULL )
				{
					sscanf(Serial_RxPacket, "Ki=%f", &Ki);
					printf("Ki = %.2f\n" , Ki) ;
				}
				else if ( strstr(Serial_RxPacket , "Kd") != NULL )
				{
					sscanf(Serial_RxPacket, "Kd=%f", &Kd);
					printf("Kd = %.2f\n" , Kd) ;
				}
				#endif
			// 江协 HEX 测试
				
		}
		// OLED显示
//		OLED_ShowHexNum(0  , 20 , DataArr[0] , 2 , OLED_6X8 ) ;
//		
//		OLED_ShowNum(0  , 40 , DataArr[1] , 2 , OLED_6X8 ) ;
//		OLED_ShowNum(20 , 40 , DataArr[2] , 2 , OLED_6X8 ) ;
//		OLED_ShowNum(40 , 40 , DataArr[3] , 2 , OLED_6X8 ) ;
//		OLED_ShowNum(60 , 40 , DataArr[4] , 2 , OLED_6X8 ) ;


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
	// 实验
	#ifdef PID_VOFA
	if (count_sys % 40 == 0) 
	{
		PID_Set(&PID , Kp , Kd , Ki , goalPoint ) ;
		setPoint = PID_Cal(&PID , realPoint , -5 , 15 ) ;
		printf("%f , %f , %f \n" , goalPoint , setPoint , realPoint ) ;
	}
	#endif
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
