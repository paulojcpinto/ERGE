/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

#include "init.h"
#include "usart.h"
#define 		line_mask 			GPIOE->ODR & 0x03C

uint8_t NumPad [4][4]  =		{'1',			'2',		 '3',			'A',
	
														 '4',			'5',		 '6',			'B',
	
														 '7',			'8',		 '9',			'C',
	
														 '*',			'0',		 '#',			'D'	};



/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, NumPad_1Lin_Pin|NumPad_2Lin_Pin|NumPad_3Lin_Pin|NumPad_4Lin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EmbLED_Green_Pin|LedGreen_Pin|EmbLED_Red_Pin|LedRed_Pin 
                          |EmbLED_Blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = NumPad_1Lin_Pin|NumPad_2Lin_Pin|NumPad_3Lin_Pin|NumPad_4Lin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin */
  GPIO_InitStruct.Pin = EmbLED_Green_Pin|LedGreen_Pin|EmbLED_Red_Pin|LedRed_Pin 
                          |EmbLED_Blue_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MotionSensor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MotionSensor_GPIO_Port, &GPIO_InitStruct);


  /*Configure GPIO pins : PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = NumPad_1Col_Pin|NumPad_2Col_Pin|NumPad_3Col_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = NumPad_3Col_Pin|NumPad_4Col_Pin|NumPad_2Col_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);

  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_PIN )
{
	switch ( GPIO_PIN )
	{		
		case NumPad_2Col_Pin:
		{	
			
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Green_Pin);
			
			switch ( line_mask  )
			{
				case NumPad_1Lin_Pin:
				{
					//2
					choose_pin[code_number++] = '2';
						HAL_UART_Transmit(&huart4, "<K>",3, 1000);
					
				}; break;
				
				case NumPad_2Lin_Pin:
				{
					choose_pin[code_number++] = '5';//5	
					HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
				
				case NumPad_3Lin_Pin:
				{
					choose_pin[code_number++] = '8';//8	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
				
				case NumPad_4Lin_Pin:
				{
					choose_pin[code_number++] = '0';//0	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
			}			
			
		}; break;
	
		case NumPad_3Col_Pin:
		{			
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
			
			switch ( line_mask  )
			{
				case NumPad_1Lin_Pin:
				{
					choose_pin[code_number++] = '1';//1	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
				
				case NumPad_2Lin_Pin:
				{
					choose_pin[code_number++] = '4';//4	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
				
				case NumPad_3Lin_Pin:
				{
					choose_pin[code_number++] = '7';//7	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;

			}
		} break;
		
		case NumPad_4Col_Pin:
		{			
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
			
			switch ( line_mask  )
			{
				case NumPad_1Lin_Pin:
				{
					choose_pin[code_number++] = '3';//3	
HAL_UART_Transmit(&huart4, "<K>",3, 1000);

				}; break;
				
				case NumPad_2Lin_Pin:
				{
					choose_pin[code_number++] = '6';//6
						HAL_UART_Transmit(&huart4, "<K>",3, 1000);
				}; break;
				
				case NumPad_3Lin_Pin:
				{
					choose_pin[code_number++] = '9';//9
						HAL_UART_Transmit(&huart4, "<K>",3, 1000);
				}; break;
			}
		} break;
	}

}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
