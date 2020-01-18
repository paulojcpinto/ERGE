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

/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */
extern uint8_t Rx_Buffer[128];
extern volatile uint16_t UART3Tx_index;
extern volatile uint16_t UART3Rx_index;
extern uint8_t UART3Rx_Buffer[128];

extern uint8_t UART4Rx_Buffer[1024];
extern uint8_t Rx4_Buffer[256];
extern volatile uint16_t UART4Tx_index;
extern volatile uint16_t UART4Rx_index;

extern uint8_t UART5Rx_Buffer[1024];
extern uint8_t Rx5_Buffer[128];
extern volatile uint16_t UART5Tx_index;
extern volatile uint16_t UART5Rx_index;

extern uint8_t UART6Rx_Buffer[512];
extern uint8_t Rx6_Buffer[128];
extern volatile uint16_t UART6Tx_index;
extern volatile uint16_t UART6Rx_index;
/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/****************************************************** 	
 *		
 *		Function: messageReceived()
 *
 *		Description: check for incoming messages
 *		
 *		Notes: 
 *
 *		Returns: - return 0 if don't have any message
 *						 - return the size of the message
 *							 if there are some message
 *
 *****************************************************/
int messageReceived(int *c);

/****************************************************** 	
 *		
 *		Function: init_UART3()
 *
 *		Description: set the interrupt
 *		
 *		Notes: shouldn't be used in the while cycle
 *
 *		Returns: don't return anything
 *
 *****************************************************/
void init_UARTs(void);


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
