/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "string.h"	 
#define USART_REC_LEN 250
#define GPS_BUFFER_LENGTH 250
typedef struct 
{
	uint8_t GPS_Buffer[GPS_BUFFER_LENGTH];
	uint8_t is_get_data;
	uint16_t time_year;
  uint16_t time_month;
  uint16_t time_day;
  uint16_t time_hour;
  uint16_t time_min;
  uint16_t time_sec;
  int32_t lat;
  uint8_t lat_ns;
  int32_t lon;
  uint8_t lon_ew;
  uint16_t speed;
  uint16_t course;
	
}GPSDATA;

extern GPSDATA GPS_Data;

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

void clear_nbiot_buffer(void);//清NBIOT接收数组
extern uint8_t  USART_RX_NBIOT_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
