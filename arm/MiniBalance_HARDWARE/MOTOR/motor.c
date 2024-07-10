#include "motor.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE); // 使能GPIOB C D时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; // 对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																			// 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																		// 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;																			// 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);																					// 初始化GPIO
	GPIO_SetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);			// GPIOC1-7设置高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; // 对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // 上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   // 初始化GPIO
	GPIO_SetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);		   // GPIOB0设置设置高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		  // 对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  // 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);			  // 初始化GPIO
	GPIO_SetBits(GPIOD, GPIO_Pin_2);				  // GPIOD0设置设置高
}
void MiniBalance_PWM_Init(u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// TIM8时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		// 使能PORTC时钟
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);		// GPIOC9复用为定时器8 CH4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// 初始化PC口
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 16799;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				  // 初始化定时器8
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);							  // MOE 主输出使能
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);			  // 使能TIM8在CCR4上的预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM8, ENABLE);										  // 使能TIM8
	TIM_SetCompare4(TIM8, 4200);

	////////////初始化PA1////////////////////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		// TIM5时钟使能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);		// GPIOCA复用为定时器1 CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// 初始化PA口
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 8399;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // 初始化定时器5
	// 初始化TIM5   PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM2
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);			  // 使能TIM5在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM5, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM5, ENABLE);										  // 使能TIM5
	TIM_SetCompare2(TIM5, 4200);

	////////////初始化PA8////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		// TIM1时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		// 使能PORTA时钟
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);		// GPIOCA复用为定时器1 CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// 初始化PC口
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 16799;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 初始化定时器1
	// 初始化TIM1   PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);							  // MOE 主输出使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);			  // 使能TIM1在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM1, ENABLE);										  // 使能TIM1
	TIM_SetCompare1(TIM1, 4200);

	MiniBalance_Motor_Init();
}

void Set_PWM_End(u16 arr1, u16 arr2, u16 arr3)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// TIM8时钟使能
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);		// GPIOC9复用为定时器8 CH4
	TIM_TimeBaseStructure.TIM_Prescaler = 29;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr1;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				  // 初始化定时器8
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);							  // MOE 主输出使能
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);			  // 使能TIM8在CCR4上的预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM8, ENABLE);										  // 使能TIM8
	TIM_SetCompare4(TIM8, arr1 >> 1);

	////////////初始化PA1////////////////////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		// TIM5时钟使能
	TIM_TimeBaseStructure.TIM_Prescaler = 14;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr2;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // 初始化定时器5
	// 初始化TIM5   PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM2
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);			  // 使能TIM5在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM5, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM5, ENABLE);										  // 使能TIM5
	TIM_SetCompare2(TIM5, arr2 >> 1);

	////////////初始化PA8////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		// TIM1时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		// 使能PORTA时钟
	TIM_TimeBaseStructure.TIM_Prescaler = 29;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr3;					// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // 初始化定时器1
	// 初始化TIM1   PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);					  // 根据T指定的参数初始化外设TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);							  // MOE 主输出使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);			  // 使能TIM1在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);							  // ARPE使能
	TIM_Cmd(TIM1, ENABLE);										  // 使能TIM1
	TIM_SetCompare1(TIM1, arr3 >> 1);
}
