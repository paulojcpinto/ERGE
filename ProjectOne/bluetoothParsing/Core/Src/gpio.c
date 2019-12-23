/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, NumPad_1Lin_Pin|NumPad_2Lin_Pin|NumPad_3Lin_Pin|NumPad_4Lin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EmbLED_Green_Pin|LedGreen_Pin|EmbLED_Red_Pin|LedRed_Pin 
                          |EmbLED_Blue_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(fingerPrint_output_GPIO_Port, fingerPrint_output_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = NumPad_1Lin_Pin|NumPad_2Lin_Pin|NumPad_3Lin_Pin|NumPad_4Lin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
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

  /*Configure GPIO pins : PGPin PGPin */
  GPIO_InitStruct.Pin = MotionSensor_Pin|NumPad_4Col_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = fingerPrint_EXTI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(fingerPrint_EXTI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = fingerPrint_output_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(fingerPrint_output_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = NumPad_1Col_Pin|NumPad_2Col_Pin|NumPad_3Col_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback ( uint16_t GPIO_PIN )
{
	switch ( GPIO_PIN )
	{
		case NumPad_1Col_Pin:
		{			
			//HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
			
			switch ( line_mask  )
			{
				case NumPad_1Lin_Pin:
				{
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Green_Pin);
				}; break;
				
				case NumPad_2Lin_Pin:
				{
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
				}; break;
				
				case NumPad_3Lin_Pin:
				{
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
				}; break;
				
				case NumPad_4Lin_Pin:
				{
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
				}; break;
			}
			
		}; break;
		
		case NumPad_2Col_Pin:
		{	
			
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Green_Pin);
			
			switch ( line_mask  )
			{
				case NumPad_1Lin_Pin:
				{
					
				}; break;
				
				case NumPad_2Lin_Pin:
				{
					
				}; break;
				
				case NumPad_3Lin_Pin:
				{
					
				}; break;
				
				case NumPad_4Lin_Pin:
				{
					
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
					
				}; break;
				
				case NumPad_2Lin_Pin:
				{
					
				}; break;
				
				case NumPad_3Lin_Pin:
				{
					
				}; break;
				
				case NumPad_4Lin_Pin:
				{
					
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
					
				}; break;
				
				case NumPad_2Lin_Pin:
				{
					
				}; break;
				
				case NumPad_3Lin_Pin:
				{
					
				}; break;
				
				case NumPad_4Lin_Pin:
				{
					
				}; break;
			}
		} break;
	}
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
