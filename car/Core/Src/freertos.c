/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "motor.h"
#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <stdio.h>
#include "L610.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern float pitch,roll,yaw;
extern short aacx,aacy,aacz;
extern short gyrox,gyroy,gyroz;
extern float temp;
extern UART_HandleTypeDef huart3;
int wheel_ctr_flag=0;
int u2_buf;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void check_movements(void);

void process_wheel_mov(int i);

void process_arm_mov(int i);

#define START_TASK_PRIO             1
#define START_TASK_STACK_SIZE       128
osThreadId start_TaskHandle;
void Start_task( void * pvParameters );

#define MPU6050_TASK_PRIO           2
#define MPU6050_TASK_STACK_SIZE     128
osThreadId mpu6050_TaskHandle;
void mpu6050_task( void * pvParameters );

#define TIM4_TASK_PRIO              3
#define TIM4_STACK_SIZE             128
osThreadId TIM4_TaskHandle;
void TIM4_task( void * pvParameters );
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId uart_recvHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Start_task( void * pvParameters )
{
	 taskENTER_CRITICAL();
//	 xTaskCreate((TaskFunction_t         )  mpu6050_task,
//							 (const char * const     )  "mpu6050_Task",
//							 (configSTACK_DEPTH_TYPE )  MPU6050_TASK_STACK_SIZE,
//							 (void *                 )  NULL,
//							 (UBaseType_t            )  MPU6050_TASK_PRIO,
//							 (TaskHandle_t *         )  &mpu6050_TaskHandle);
							 
	 xTaskCreate((TaskFunction_t         )  TIM4_task,
							 (const char * const     )  "TIM4_task",
							 (configSTACK_DEPTH_TYPE )  TIM4_STACK_SIZE,
							 (void *                 )  NULL,
							 (UBaseType_t            )  TIM4_TASK_PRIO,
							 (TaskHandle_t *         )  &TIM4_TaskHandle);
							 
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}

void mpu6050_task( void * pvParameters )
{
	 	uint8_t str_buf[10]={0};
	  while(1)
		{
			vTaskDelay(500);
			while(mpu_dmp_get_data(&pitch, &roll, &yaw));
			MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
			MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
			temp=MPU_Get_Temperature();
//			sprintf((char*)str_buf,"X;%.1f\r\n",roll);
//			HAL_UART_Transmit(&huart3,str_buf,sizeof(str_buf),10000);
		}
}


void TIM4_task( void * pvParameters )
{
	while(1) 
	{
		check_movements();
		vTaskDelay(1000); // 等待 1 秒
	}

}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void uart_recv_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  	 xTaskCreate((TaskFunction_t         )  Start_task,
							 (const char * const     )  "Start_task",
							 (configSTACK_DEPTH_TYPE )  START_TASK_STACK_SIZE,
							 (void *                 )  NULL,
						   (UBaseType_t            )  START_TASK_PRIO,
						   (TaskHandle_t *         )  &start_TaskHandle);
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of uart_recv */
  osThreadDef(uart_recv, uart_recv_Task, osPriorityHigh, 0, 128);
  uart_recvHandle = osThreadCreate(osThread(uart_recv), NULL);

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
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_uart_recv_Task */
/**
* @brief Function implementing the uart_recv thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_recv_Task */
void uart_recv_Task(void const * argument)
{
  /* USER CODE BEGIN uart_recv_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END uart_recv_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void process_wheel_mov(int i) {
		uint8_t u2_buf;
    printf("wheel_mov=%d\n", i);
    wheel_ctr_flag = 1;
    Clear_Buffer();
    switch (i) {
        case 1: Motor_enable();control(1, 10, 1); control(2, 10, 1); break;
        case 2: Motor_disable(); break;
        case 3: control(1, 80, 0); control(2, 80, 0); break;
        case 4: control(1, 10, 1); control(2, 0, 1); break;
        case 5: control(1, 0, 1); control(2, 10, 1); break;
				case 6: control(1, 40, 1); break;
        case 7: control(1, 10, 1); control(2, 10, 1); break;
				case 8: 
					HAL_UART_Receive_IT(&huart2,(uint8_t *)&Re_buff, 1);
					control(1,10,1);
					control(2,10,1);
				  
					u2_buf = 9;
					HAL_UART_Transmit(&huart2, &u2_buf, 1, 0xffff);				
					work_finish=0; 
					break;
		  	case 9: 
					work_finish=1; 
					u2_buf = 10;
					HAL_UART_Transmit(&huart2, &u2_buf, 1, 0xffff);		
					break;
        default: break;
    }
}


void process_arm_mov(int i) {
    printf("arm_mov=%d\n", i);
    uint8_t u2_buf = i;
    HAL_UART_Transmit(&huart2, &u2_buf, 1, 0xffff);
    Clear_Buffer();
}

void check_movements(void) 
{
	if(strstr((const char*)RxBuffer, (const char *)"+HMREC") != NULL) 
	{
		printf("成功接收\n");

		// 处理 wheel_mov
		wheel_ctr_flag = 0;
		for (int i = 1; i <= 9; i++) 
		{
			char search_str[16];
			snprintf(search_str, sizeof(search_str), "\"wheel_mov\":%d", i);
			if (strstr((const char*)RxBuffer, (const char*)search_str) != NULL) 
			{
				process_wheel_mov(i);
				break;
			}
		}

		// 如果未检测到 wheel_mov，则处理 arm_mov
		if (wheel_ctr_flag == 0) 
		{
			for (int i = 1; i <= 9; i++) 
			{
				char search_str[16];
				snprintf(search_str, sizeof(search_str), "\"arm_mov\":%d", i);
				if (strstr((const char*)RxBuffer, (const char*)search_str) != NULL) 
				{
					process_arm_mov(i);
					break;
				}
			}
		}
	}
}

/* USER CODE END Application */

