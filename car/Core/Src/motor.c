#include "main.h"
#include "motor.h"

extern TIM_HandleTypeDef htim4;
int led=0;
int motor_speed=0;
int speed_change_flag=0;
int dir_1=1;
int dir_2=1;
int final_motor1,final_motor2;
int motor1_stop=1;
int motor2_stop=1;


void control(int motor, int speed, int direction)
{
	if (motor == 1)
	{
		Motor_direction_1(direction);
		if(speed==0)
		{
			//Motor_disable();
			HAL_TIM_OC_Stop(&htim4,TIM_CHANNEL_1);
		  motor1_stop=1;
		}
//		if((speed!=0) && (motor1_stop == 1))
//		{
//			Motor_speed_Set(speed);
//			HAL_TIM_OC_Start(&htim4,TIM_CHANNEL_1);
//			motor1_stop =0;
//		}
		else//((speed!=0) && (motor1_stop == 0))
		{
			//Motor_enable();
			HAL_TIM_OC_Start(&htim4,TIM_CHANNEL_1);
			Motor_speed_Set(speed);
		}
	}
	
	if (motor == 2)
	{
		Motor_direction_2(direction);
		if(speed==0)
		{
			//Motor_disable();
			HAL_TIM_OC_Stop(&htim4,TIM_CHANNEL_2);
			motor2_stop=1;
		}
//		if((speed!=0) && (motor2_stop == 1))
//		{
//			Motor_speed_Set(speed);
//			HAL_TIM_OC_Start(&htim4,TIM_CHANNEL_2);
//			motor2_stop =0;
//		}
		else //((speed!=0) && (motor2_stop == 0))
		{
			//Motor_enable();
			HAL_TIM_OC_Start(&htim4,TIM_CHANNEL_2);
			Motor_speed_Set(speed);
		}
	}
}
//电机使能
void Motor_enable(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
void Motor_disable(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}

//电机脉冲设置
void Motor_speed_Set(int motor_speed)
{
	HAL_TIM_Base_Stop(&htim4);
	htim4.Instance->ARR=(200-motor_speed);
	HAL_TIM_Base_Start(&htim4);
}

//电机方向
void Motor_direction_1(int dir)
{
	if(dir==1) 
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else if(dir==0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}
void Motor_direction_2(int dir)
{
	if(dir==1) 
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	}
	else if(dir==0)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}
