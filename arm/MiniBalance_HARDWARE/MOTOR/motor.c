#include "motor.h"
/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE); // ʹ��GPIOB C Dʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; // ��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																			// ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																		// 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;																			// ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);																					// ��ʼ��GPIO
	GPIO_SetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);			// GPIOC1-7���ø�

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; // ��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   // ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   // ����
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   // ��ʼ��GPIO
	GPIO_SetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);		   // GPIOB0�������ø�

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		  // ��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  // ��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	  // ����
	GPIO_Init(GPIOD, &GPIO_InitStructure);			  // ��ʼ��GPIO
	GPIO_SetBits(GPIOD, GPIO_Pin_2);				  // GPIOD0�������ø�
}
void MiniBalance_PWM_Init(u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// TIM8ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		// ʹ��PORTCʱ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);		// GPIOC9����Ϊ��ʱ��8 CH4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// ��ʼ��PC��
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 16799;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				  // ��ʼ����ʱ��8
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);							  // MOE �����ʹ��
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);			  // ʹ��TIM8��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM8, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM8, ENABLE);										  // ʹ��TIM8
	TIM_SetCompare4(TIM8, 4200);

	////////////��ʼ��PA1////////////////////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		// TIM5ʱ��ʹ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);		// GPIOCA����Ϊ��ʱ��1 CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��PA��
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 8399;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // ��ʼ����ʱ��5
	// ��ʼ��TIM5   PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM2
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);			  // ʹ��TIM5��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM5, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM5, ENABLE);										  // ʹ��TIM5
	TIM_SetCompare2(TIM5, 4200);

	////////////��ʼ��PA8////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		// TIM1ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		// ʹ��PORTAʱ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);		// GPIOCA����Ϊ��ʱ��1 CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					// GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��PC��
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 16799;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // ��ʼ����ʱ��1
	// ��ʼ��TIM1   PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);							  // MOE �����ʹ��
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);			  // ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM1, ENABLE);										  // ʹ��TIM1
	TIM_SetCompare1(TIM1, 4200);

	MiniBalance_Motor_Init();
}

void Set_PWM_End(u16 arr1, u16 arr2, u16 arr3)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// TIM8ʱ��ʹ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);		// GPIOC9����Ϊ��ʱ��8 CH4
	TIM_TimeBaseStructure.TIM_Prescaler = 29;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr1;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				  // ��ʼ����ʱ��8
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM8
	TIM_CtrlPWMOutputs(TIM8, ENABLE);							  // MOE �����ʹ��
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);			  // ʹ��TIM8��CCR4�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM8, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM8, ENABLE);										  // ʹ��TIM8
	TIM_SetCompare4(TIM8, arr1 >> 1);

	////////////��ʼ��PA1////////////////////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		// TIM5ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Prescaler = 14;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr2;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // ��ʼ����ʱ��5
	// ��ʼ��TIM5   PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM2
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);			  // ʹ��TIM5��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM5, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM5, ENABLE);										  // ʹ��TIM5
	TIM_SetCompare2(TIM5, arr2 >> 1);

	////////////��ʼ��PA8////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		// TIM1ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		// ʹ��PORTAʱ��
	TIM_TimeBaseStructure.TIM_Prescaler = 29;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr3;					// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // ��ʼ����ʱ��1
	// ��ʼ��TIM1   PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // �������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);					  // ����Tָ���Ĳ�����ʼ������TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);							  // MOE �����ʹ��
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);			  // ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);							  // ARPEʹ��
	TIM_Cmd(TIM1, ENABLE);										  // ʹ��TIM1
	TIM_SetCompare1(TIM1, arr3 >> 1);
}
