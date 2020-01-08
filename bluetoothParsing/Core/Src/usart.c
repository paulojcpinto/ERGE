/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "FingerPrint.h"
#include "FingerPrintConfig.h"
uint8_t UART3Rx_Buffer[128];
uint8_t Rx_Buffer[128];
volatile uint16_t UART3Tx_index;
volatile uint16_t UART3Rx_index;

uint8_t UART4Rx_Buffer[1024];
uint8_t Rx4_Buffer[256];
volatile uint16_t UART4Tx_index;
volatile uint16_t UART4Rx_index;

uint8_t UART5Rx_Buffer[1024];
uint8_t Rx5_Buffer[128];
volatile uint16_t UART5Tx_index;
volatile uint16_t UART5Rx_index;


uint8_t UART6Rx_Buffer[512];
uint8_t Rx6_Buffer[128];
volatile uint16_t UART6Tx_index;
volatile uint16_t UART6Rx_index;

uint8_t UART7Rx_Buffer[128];
uint8_t Rx7_Buffer[128];
volatile uint8_t UART7Rx_index;
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* UART4 init function */
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

}
/* UART5 init function */
void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 256000;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = Bluetooth_Uart_Tx_Pin|Bluetooth_Uart_Rx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    GPIO_InitStruct.Pin = WIFI_Uart_Tx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(WIFI_Uart_Tx_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = WIFI_Uart_Rx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(WIFI_Uart_Rx_GPIO_Port, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = FingerPrint_Uart_Tx_Pin|FingerPrint_Uart_Rx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();
  
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**USART6 GPIO Configuration    
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
    */
    GPIO_InitStruct.Pin = GSM_Usart_Rx_Pin|GSM_Usart_Tx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, Bluetooth_Uart_Tx_Pin|Bluetooth_Uart_Rx_Pin);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    HAL_GPIO_DeInit(WIFI_Uart_Tx_GPIO_Port, WIFI_Uart_Tx_Pin);

    HAL_GPIO_DeInit(WIFI_Uart_Rx_GPIO_Port, WIFI_Uart_Rx_Pin);

    /* UART5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, FingerPrint_Uart_Tx_Pin|FingerPrint_Uart_Rx_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();
  
    /**USART6 GPIO Configuration    
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
    */
    HAL_GPIO_DeInit(GPIOG, GSM_Usart_Rx_Pin|GSM_Usart_Tx_Pin);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */



//implemantation of UART ISR
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart->Instance == USART2)
	{
		FingerPrint_RxCallback();
		//HAL_UART_Transmit_IT(&huart3, &UART3Rx_index, 1);
	}
	
	if (huart->Instance == UART4)
		{ //current UART?
			UART4Rx_index++;
			UART4Rx_index &= ~(1<<10); //keep index inside the limits

			// set the interrupt for UART3 Rx again
			HAL_UART_Receive_IT(&huart4, &UART4Rx_Buffer[UART4Rx_index], 1);
		
//		if (UART3Rx_Buffer[UART3Rx_index-1] == '>')
	//		HAL_UART_Transmit_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index]-1, 1);
		}
		
		if (huart->Instance == UART5)
		{ //current UART?
			UART5Rx_index++;
			UART5Rx_index &= ~(1<<10); //keep index inside the limits
//			if (UART5Rx_index == UART5Tx_index)
//			{
//				UART5Tx_index++;
//			UART5Tx_index &= ~(1<<10);
//			}
			// set the interrupt for UART3 Rx again
			HAL_UART_Receive_IT(&huart5, &UART5Rx_Buffer[UART5Rx_index], 1);
		
//		if (UART3Rx_Buffer[UART3Rx_index-1] == '>')
			//HAL_UART_Transmit_IT(&huart3, &UART5Rx_Buffer[UART3Rx_index]-1, 1);
		}
				if (huart->Instance == USART6)
		{ //current UART?
			UART6Rx_index++;
			UART6Rx_index &= ~(1<<9); //keep index inside the limits
			if (UART6Rx_index == UART6Tx_index)
			{
				UART6Tx_index++;
			UART6Tx_index &= ~(1<<9);
			}
			// set the interrupt for UART3 Rx again
			HAL_UART_Receive_IT(&huart6, &UART6Rx_Buffer[UART6Rx_index], 1);
		
//		if (UART3Rx_Buffer[UART3Rx_index-1] == '>')
			//HAL_UART_Transmit_IT(&huart3, &UART5Rx_Buffer[UART3Rx_index]-1, 1);
		}
	if (huart->Instance == USART3){ //current UART?
		UART6Rx_index++;
		UART6Rx_index &= ~(1<<7); //keep index inside the limits
		
		// set the interrupt for UART3 Rx again
		HAL_UART_Receive_IT(&huart3, &UART6Rx_Buffer[UART6Rx_index], 1);
		HAL_UART_Transmit_IT(&huart4, &UART6Rx_Buffer[UART6Rx_index]-1, 1);
	}

}

int messageReceived(int *c){
	static int out_index = 0;
//	int return_value = 0;
	while(UART3Tx_index != UART3Rx_index){
		if(UART3Rx_Buffer[UART3Tx_index] == '<'){
			*c = 3;
			UART3Tx_index ++;
//			Rx_Buffer[out_index - 1] = '\0';
		}
		else if (*c == 1)
		{
		if(UART3Rx_Buffer[UART3Tx_index] == '>') {	
			*c = 2;
	//		HAL_UART_Transmit_IT(&huart4, "pp", 2);
			Rx_Buffer[out_index] = UART3Rx_Buffer[UART3Tx_index++];
			UART3Tx_index &= ~(1<<7);
			out_index=0;
			return *c-2;

		}
		else
		Rx_Buffer[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
	}
		else if (*c == 3 && UART3Rx_Buffer[UART3Tx_index] == 'N')
		{
			*c=1;
			HAL_UART_Transmit_IT(&huart4, "okoko", 5);
			Rx_Buffer[out_index++] = '<';
			Rx_Buffer[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
		}
		else 
		{
			*c= 7;
			UART3Tx_index++;
		}
		//UART3Tx_index++;
		UART3Tx_index &= ~(1<<7);
	}
	return *c-2;
}

void init_UARTs(){
	// set the interrupt for UART3 Rx
	HAL_UART_Receive_IT(&huart3, &UART3Rx_Buffer[UART3Rx_index], 1);
	HAL_UART_Receive_IT(&huart4, &UART4Rx_Buffer[UART4Rx_index], 1);
	HAL_UART_Receive_IT(&huart5, &UART5Rx_Buffer[UART5Rx_index], 1);
	HAL_UART_Receive_IT(&huart6, &UART6Rx_Buffer[UART6Rx_index], 1);
	FingerPrint_RxCallback();
	//HAL_UART_Receive_IT(&huart2, &UART7Rx_Buffer[UART7Rx_index], 1);
}
int fputc(int ch, FILE *f){
	HAL_UART_Transmit(&huart5, (uint8_t*)&ch, 1, 100);
	return ch;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
