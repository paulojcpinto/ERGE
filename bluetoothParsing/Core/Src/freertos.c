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
#include "gsm.h"
#include <string.h>

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
osThreadId updateTimeTaskHandle;
osThreadId publishTaskHandle;
osThreadId parsingBTTaskHandle;
osThreadId parsingGSMTaskHandle;
osThreadId numPadTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartmyTask(void const * argument);
void StartsimTask(void const * argument);
//void StartreleaseTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTaskUpdateTime(void const * argument);
void StartTaskpublish(void const * argument);
void StarPparsingBT(void const * argument);
void StarPparsingGSM(void const * argument);
void StartNumpad(void const * argument);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityAboveNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of updateTimeTask */
  osThreadDef(updateTimeTask, StartTaskUpdateTime, osPriorityNormal, 0, 256);
  updateTimeTaskHandle = osThreadCreate(osThread(updateTimeTask), NULL);

  /* definition and creation of publishTask */
  osThreadDef(publishTask, StartTaskpublish, osPriorityNormal, 0, 256);
  publishTaskHandle = osThreadCreate(osThread(publishTask), NULL);

  /* definition and creation of parsingBTTask */
  osThreadDef(parsingBTTask, StarPparsingBT, osPriorityNormal, 0, 1024);
  parsingBTTaskHandle = osThreadCreate(osThread(parsingBTTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	osThreadDef(parsingGSMTask, StarPparsingGSM, osPriorityNormal, 0, 512);
  parsingGSMTaskHandle = osThreadCreate(osThread(parsingGSMTask), NULL);
	
		osThreadDef(numPadTask, StartNumpad, osPriorityNormal, 0, 512);
  numPadTaskHandle = osThreadCreate(osThread(numPadTask), NULL);
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
    parsing_esp();
		vTaskDelay(100);
		}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskUpdateTime */
/**
* @brief Function implementing the updateTimeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUpdateTime */
void StartTaskUpdateTime(void const *argument)
{
    /*USER CODE BEGIN StartTaskUpdateTime */
    if (xSemaphoreTake(sim1, 99999))
    {
    }

    /*Infinite loop */
    for (;;)
    {
        if (xSemaphoreTake(sim1, 99999))
        {
            stmtime.updated = 1;
            while (stmtime.need_update)
            {
                vTaskDelay(10);
            }
            wait1();
            HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
            while (stmtime.updated)
                vTaskDelay(100);
            verify_release_time1();
            stmtime.need_update = 0;
        }

        vTaskDelay(1000);
    }

    /*USER CODE END StartTaskUpdateTime */
}

/* USER CODE BEGIN Header_StartTaskpublish */
/**
* @brief Function implementing the publishTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskpublish */
void StartTaskpublish(void const *argument)
{
    /*USER CODE BEGIN StartTaskpublish */


    if (xSemaphoreTake(release_signal, 99999))
    {
        ini();
        HAL_UART_Transmit(&huart5, "AT+CIPCLOSE\r\n", 13, 1000);
        HAL_UART_Transmit(&huart6, "AT+CMEE=2\r\n", 11, 1000);
        vTaskDelay(100);
        wait1();
    }

    /*Infinite loop */
    for (;;)
    {
        if (xSemaphoreTake(release_signal, 99999))
        {
            while (stmtime.need_update)
            {
                HAL_UART_Transmit(&huart3, "s", 1, 100);
                vTaskDelay(100);
            }

            for (int pos = 0; pos < MAX_USERS; pos++)
            {
                if (to_release[pos].to_publish && to_release[pos].where == 0)
                {
                    publish_twitter(to_release[pos].message, to_release[pos].cardentials_twitter);
                }
                else if (to_release[pos].to_publish && to_release[pos].where == 1)
                    send_SMS(to_release[pos].phone_number, to_release[pos].message, strlen(to_release[pos].message));
                to_release[pos].to_publish = 0;
            }

            finish_twitter();
        }

        vTaskDelay(1000);
    }

    /*USER CODE END StartTaskpublish */
}

/* USER CODE BEGIN Header_StarPparsingBT */
/**
* @brief Function implementing the parsingBTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StarPparsingBT */
void StarPparsingBT(void const * argument)
{
  /* USER CODE BEGIN StarPparsingBT */
	initUser();
  /* Infinite loop */
  for(;;)
  {
		parsingBT(&cp);
		vTaskDelay(100);
  }
  /* USER CODE END StarPparsingBT */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     void StarPparsingGSM(void const * argument)
{
  /* USER CODE BEGIN StarPparsingBT */
	    

  /* Infinite loop */
  for(;;)
  {
		parsing_gsm();
		vTaskDelay(100);
  }
  /* USER CODE END StarPparsingBT */
}

   void StartNumpad(void const * argument)
{
  /* USER CODE BEGIN StarPparsingBT */
	//    if (xSemaphoreTake(numPad, 99999));
	if (xSemaphoreTake(numPad, 99999));
  /* Infinite loop */
  for(;;)
  {

		code_number = 0;
		if (xSemaphoreTake(numPad, 99999))
		{
			while ( code_number < 4 )
			{
				line_output 	|=   NumPad_1Lin_Pin;
				vTaskDelay(70);
				line_output		&=  ~NumPad_1Lin_Pin;
				vTaskDelay(100);
			
				line_output 	|=   NumPad_2Lin_Pin;
				vTaskDelay(70);
				line_output 	&=  ~NumPad_2Lin_Pin;
				vTaskDelay(100);
			
				line_output 	|=   NumPad_3Lin_Pin;
				vTaskDelay(70);
				line_output 	&=  ~NumPad_3Lin_Pin;
				vTaskDelay(100);
			
				line_output 	|=   NumPad_4Lin_Pin;
				vTaskDelay(70);
				line_output 	&=  ~NumPad_4Lin_Pin;
				vTaskDelay(100);
			}
			pin_code[4] = 0;
		  choose_pin[4] = 0;
			if (!strcmp(pin_code, choose_pin))
				HAL_UART_Transmit(&huart4, "<Q1>",4, 1000);
			else
				HAL_UART_Transmit(&huart4, "<Q5>",4, 1000);
		}
		vTaskDelay(100);
		
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
