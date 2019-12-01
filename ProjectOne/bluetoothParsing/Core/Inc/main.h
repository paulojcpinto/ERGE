/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NumPad_1Lin_Pin GPIO_PIN_2
#define NumPad_1Lin_GPIO_Port GPIOE
#define NumPad_2Lin_Pin GPIO_PIN_3
#define NumPad_2Lin_GPIO_Port GPIOE
#define NumPad_3Lin_Pin GPIO_PIN_4
#define NumPad_3Lin_GPIO_Port GPIOE
#define NumPad_4Lin_Pin GPIO_PIN_5
#define NumPad_4Lin_GPIO_Port GPIOE
#define EmbLED_Green_Pin GPIO_PIN_0
#define EmbLED_Green_GPIO_Port GPIOB
#define MotionSensor_Pin GPIO_PIN_1
#define MotionSensor_GPIO_Port GPIOG
#define FingerPrint_Uart_Rx_Pin GPIO_PIN_7
#define FingerPrint_Uart_Rx_GPIO_Port GPIOE
#define FingerPrint_Uart_Tx_Pin GPIO_PIN_8
#define FingerPrint_Uart_Tx_GPIO_Port GPIOE
#define LedGreen_Pin GPIO_PIN_13
#define LedGreen_GPIO_Port GPIOB
#define EmbLED_Red_Pin GPIO_PIN_14
#define EmbLED_Red_GPIO_Port GPIOB
#define LedRed_Pin GPIO_PIN_15
#define LedRed_GPIO_Port GPIOB
#define NumPad_4Col_Pin GPIO_PIN_3
#define NumPad_4Col_GPIO_Port GPIOG
#define NumPad_4Col_EXTI_IRQn EXTI3_IRQn
#define Bluetooth_Uart_Tx_Pin GPIO_PIN_10
#define Bluetooth_Uart_Tx_GPIO_Port GPIOC
#define Bluetooth_Uart_Rx_Pin GPIO_PIN_11
#define Bluetooth_Uart_Rx_GPIO_Port GPIOC
#define WIFI_Uart_Tx_Pin GPIO_PIN_12
#define WIFI_Uart_Tx_GPIO_Port GPIOC
#define WIFI_Uart_Rx_Pin GPIO_PIN_2
#define WIFI_Uart_Rx_GPIO_Port GPIOD
#define NumPad_1Col_Pin GPIO_PIN_4
#define NumPad_1Col_GPIO_Port GPIOD
#define NumPad_1Col_EXTI_IRQn EXTI4_IRQn
#define NumPad_2Col_Pin GPIO_PIN_5
#define NumPad_2Col_GPIO_Port GPIOD
#define NumPad_2Col_EXTI_IRQn EXTI9_5_IRQn
#define NumPad_3Col_Pin GPIO_PIN_6
#define NumPad_3Col_GPIO_Port GPIOD
#define NumPad_3Col_EXTI_IRQn EXTI9_5_IRQn
#define GSM_Usart_Rx_Pin GPIO_PIN_9
#define GSM_Usart_Rx_GPIO_Port GPIOG
#define GSM_Usart_Tx_Pin GPIO_PIN_14
#define GSM_Usart_Tx_GPIO_Port GPIOG
#define EmbLED_Blue_Pin GPIO_PIN_7
#define EmbLED_Blue_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
