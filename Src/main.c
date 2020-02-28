/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gps_handle.h"
#include "dht11.h"
#include "delay.h"
#include "BC28.h"
#include <stdarg.h>
uint8_t g_receive_data[10];
uint8_t g_receive_nbiot_data[10];
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

char g_sendata[100];

void udp_hex_to_string(uint8_t a_value, uint8_t b_value, uint8_t c_value){
		char tempstr[10];
		char sendata[10];
		memset(tempstr,0,10);
		memset(sendata,0,10);
		memset(g_sendata,0,100);
	
		sprintf(tempstr,"%x",a_value/10+0x30); //装十位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		sprintf(tempstr,"%x",(a_value%10+0x30));//装个位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		strcat(g_sendata,sendata);
		strcat(g_sendata,"41");
	
		memset(sendata,0,10);
		sprintf(tempstr,"%x",b_value/10+0x30); //装十位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		sprintf(tempstr,"%x",(b_value%10+0x30));//装个位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		strcat(g_sendata,sendata);
		strcat(g_sendata,"41");
		
		memset(sendata,0,10);
		sprintf(tempstr,"%x",c_value/10+0x30); //装十位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		sprintf(tempstr,"%x",(c_value%10+0x30));//装个位数
		strcat(sendata,tempstr);
		memset(tempstr,0,10);
		strcat(g_sendata,sendata);
	
		printf("%s", g_sendata);
}


char g_sendata_temp[10];
char g_sendata_humi[10];
char g_sendata_voltage[10];
void aliyun_data_handle1(uint8_t a_value, uint8_t b_value, uint8_t c_value){
		memset(g_sendata_temp,0,10);
		memset(g_sendata_humi,0,10);
		memset(g_sendata_voltage,0,10);
	
		char tempstr[10];
	
	  memset(tempstr,0,10);
    sprintf(tempstr,"%d",a_value);
    strcat(g_sendata_temp,tempstr);
	
		memset(tempstr,0,10);
    sprintf(tempstr,"%d",b_value);
    strcat(g_sendata_humi,tempstr);

		memset(tempstr,0,10);
    sprintf(tempstr,"%d",c_value);
    strcat(g_sendata_voltage,tempstr);
}

char g_sendata_longtitude[20];
char g_sendata_latitude[20];
void aliyun_data_handle2(int a_value, int b_value){
		memset(g_sendata_longtitude,0,20);
		memset(g_sendata_latitude,0,20);
		char tempstr[20];

	  memset(tempstr,0,20);
    sprintf(tempstr,"%d",a_value);
    strcat(g_sendata_longtitude,tempstr);
	
		memset(tempstr,0,20);
    sprintf(tempstr,"%d",b_value);
    strcat(g_sendata_latitude,tempstr);
}

void hex_to_string(uint8_t a_value, uint8_t b_value, uint8_t c_value){
		memset(g_sendata,0,100);
	
		char tempstr1[10];
		char tempstr2[10];
	
		memset(tempstr1,0,10);
		memset(tempstr2,0,10);
		sprintf(tempstr1, "%d", a_value); 
		strncat(tempstr2,"00",2);
		strcat(tempstr2,tempstr1);
		strcat(g_sendata,tempstr2);
	
		memset(tempstr1,0,10);
		memset(tempstr2,0,10);
		sprintf(tempstr1, "%d", b_value);
		strncat(tempstr2,"00",2);
		strcat(tempstr2,tempstr1);
		strcat(g_sendata,tempstr2);
	
		memset(tempstr1,0,10);
		memset(tempstr2,0,10);
		sprintf(tempstr1, "%d", c_value);
		strncat(tempstr2,"00",2);
		strcat(tempstr2,tempstr1);
		strcat(g_sendata,tempstr2);
		printf("%s", g_sendata);
}



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
	uint8_t temp;
	uint8_t humi;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2); //开启定时器中断，1ms中断一次。
	HAL_UART_Receive_IT(&huart1, g_receive_data, 1); //开启gps接收中断
  HAL_UART_Receive_IT(&huart3, g_receive_nbiot_data, 1); //开启nbiot接收中断
	HAL_ADC_Start_IT(&hadc1);//开启adc中断
	while(DHT11_Init());//初始化DHT11
	uint32_t g_voltage_value;//获取电压值
	//while(BC28_Init());
	//while(BC28_UDP_Init());
	while(BC28_ALIYUN_Init());
	printf("初始化成功");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(	Time_Tick.time_15s == 1)
    {
			get_gps_data(); //发送GPS数据
			printf("GPS_Data.lat:%d,GPS_Data.lon:%d",GPS_Data.lat,GPS_Data.lon);
      aliyun_data_handle2(GPS_Data.lat, GPS_Data.lon);
			BC28_ALIYUN_Senddata2(g_sendata_latitude, g_sendata_longtitude);
			printf("g_sendata_latitude:%s,g_sendata_longtitude:%s",g_sendata_latitude,g_sendata_longtitude);
			Time_Tick.time_15s = 0;
		}
		if(Time_Tick.time_20s == 1)
		{
			g_voltage_value = get_voltage_value();//读取电压数据
			DHT11_Read_Data(&temp,&humi);//读取温湿度数据

			printf("Temp:%d\n",temp);
      printf("Humi:%d\n",humi);
			printf("g_voltage_value:%d\n", g_voltage_value);
      //打印串口

			//udp_hex_to_string(temp, humi, g_voltage_value/100);
			//BC28_UDP_Senddata("8",g_sendata);
			//hex_to_string(temp, humi, g_voltage_value/100);
			//BC28_Senddata("8",g_sendata);
			
			aliyun_data_handle1(temp, humi, g_voltage_value/100);
			BC28_ALIYUN_Senddata1(g_sendata_temp, g_sendata_humi, g_sendata_voltage);
			Time_Tick.time_20s = 0;
		}
		

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
