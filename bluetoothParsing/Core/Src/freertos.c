/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "esp8266.h"
#include "usart.h"
#include "init.h"
#include "user.h"
#include <stdio.h>

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTaskHandle;
osThreadId simTaskHandle;
osThreadId releaseTaskHandle;


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartmyTask(void const * argument);
void StartsimTask(void const * argument);
//void StartreleaseTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartmyTask(void const * argument);
void StartsimTask(void const * argument);
//void StartreleaseTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask),  (void*)pp);
	
	osThreadDef(myTask, StartmyTask, osPriorityNormal, 0, 1024);
  myTaskHandle = osThreadCreate(osThread(myTask),  (void*)pp);
	
	osThreadDef(simTask, StartsimTask, osPriorityNormal, 0, 1024);
  simTaskHandle = osThreadCreate(osThread(simTask),  (void*)pp);
	
//	osThreadDef(releaseTask, StartreleaseTask, osPriorityNormal, 0, 1024);
//  releaseTaskHandle = osThreadCreate(osThread(releaseTask),  (void*)pp);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
parsing_gsm11();
		vTaskDelay(100);
//		if(xSemaphoreTake(finger_signal, 99999))
//		{
//			if (*okp == 5)
//				HAL_UART_Transmit(&huart3, "5",1 ,10000);
//				HAL_UART_Transmit(&huart3, "\r\nokok\r\n",8 ,10000);		
		}
  /* USER CODE END StartDefaultTask */
}


void StartmyTask(void const * argument)
{
	int i =0;
	if(xSemaphoreTake(finger_signal, 99999))
	{
		printf("AT+CIPCLOSE\r\n");
			vTaskDelay(100);
		wait1();
	}
	while(1){
	if(xSemaphoreTake(finger_signal, 99999))
	{

		publish_twitter(i++);
		
		//send_SMS ("+351916201643", "amo", 3);
	}
}	

}

void StartsimTask(void const * argument)
{

	if(xSemaphoreTake(sim1, 99999))
	{
		;
	}
	while(1){
	if(xSemaphoreTake(sim1, 99999))
	{
		stmtime.updated = 1;
		//printf("AT+CIPCLOSE\r\n");
		//publish_twitter(i++);

		wait1();
		HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
		//xSemaphoreGive(finger_signal);
		//send_SMS ("+351916201643", "amo", 3);
		while( stmtime.updated )
			vTaskDelay(100);
		verify_release_time ();
			
		HAL_UART_Transmit(&huart3, "\r\nyap\r\n", 7,1000);
	}
		vTaskDelay(100);
}
}

//void StartreleaseTask(void const * argument)
//{
//	if(xSemaphoreTake(release_signal, 99999))
//	{
//	;
//	}
////	while(1){
//	//if(xSemaphoreTake(release_signal, 99999))
//	{
//			//send_SMS ("+351916201643", "amo", 3);
//	}
//		vTaskDelay(10000);
//}




/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
