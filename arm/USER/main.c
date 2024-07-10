#include "stm32f4xx.h"
#include "sys.h"

u8 Way_Angle = 1;														// 获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波
u8 Flag_Stop = 1, Flag_Show = 0;										// 停止标志位和 显示标志位 默认停止 显示打开
int Motor_A, Motor_B, Motor_C;											// 电机PWM变量
int Voltage;															// 电池电压采样相关的变量
float Position_KP = 20, Position_KI = 0, Position_KD = 20;				// 位置控制PID参数
int Encoder_A, Encoder_B, Encoder_C;									// 编码器的脉冲计数
float Position_A = 8000, Position_B = 3660, Position_C;					// PID控制相关变量
float Target_A, Target_B, Target_C, Rate_A = 1, Rate_B = 1, Rate_C = 1; // 电机目标值
int16_t Target_X = 220, Target_Y = 80, Target_Z = 0;
int Max_Velocity = 1500, RC_Velocity = 1300;
u8 rxbuf[6], Urxbuf[6], CAN_ON_Flag = 0, Usart_ON_Flag = 0, Usart_Flag, Usart_ON_Flag2; // CAN和串口控制相关变量
u8 txbuf[8], txbuf2[8], Menu = 1;														// CAN发送相关变量
char Manipulator_Flag = 1;
float Pitch, Roll;

TaskHandle_t start_task_handle;
TaskHandle_t show_handle;

void start_task(void *arg);
void show_task(void *arg);

int main(void)
{
	delay_init(168);								//=====主频168M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //=====设置系统中断优先级分组2
	LED_Init();										//=====LED初始化
	KEY_Init();										//=====按键初始化
	uart_init(115200);								//=====延时初始化
	uart3_init(115200);								//=====串口初始化
	Encoder_Init_TIM2();							//=====编码器初始化
	Encoder_Init_TIM3();							//=====编码器初始化
	Encoder_Init_TIM4();							//=====编码器初始化
	Adc_Init();										//=====模拟量采集初始化
	delay_ms(500);								//=====延时等待系统稳定
	IIC_Init();										//=====IIC初始化
	MPU6050_initialize();							//=====MPU6050初始化
	DMP_Init();										//=====初始化DMP
	OLED_Init();									//=====OLED初始化
	CAN1_Mode_Init(1, 7, 6, 3, CAN_Mode_Normal);	//=====CAN初始化
	MiniBalance_EXTI_Init();						//=====外部中断初始化
	MiniBalance_PWM_Init(14);						//=====PWM初始化

	xTaskCreate(start_task, "start_task", 128, NULL, 1, &start_task_handle); //=====创建开始任务
	vTaskStartScheduler();
}

void start_task(void *arg)
{
	taskENTER_CRITICAL();
	xTaskCreate(show_task, "show_task", 1024, NULL, 1, &show_handle); //=====创建oled_show任务
	
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}

void show_task(void *arg)
{
	while (1)
	{
		oled_show(); //===显示屏打开
	}
}
