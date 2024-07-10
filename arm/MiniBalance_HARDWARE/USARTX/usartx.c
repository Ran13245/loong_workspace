#include "usartx.h"
/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

/**************************ʵ�ֺ���**********************************************
 *��    ��:		usart4����һ���ֽ�
 *********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while ((USART3->SR & 0x40) == 0)
		;
}
/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  // ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // ʹ��USART3ʱ��

	// ����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); // GPIOC10����ΪUSART3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); // GPIOC11����ΪUSART3

	// �˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // GPIOC10��GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			 // ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			 //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			 // ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 // ��ʼ��PC10 C11

	// USART3��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);										// ��ʼ������

	USART_Cmd(USART3, ENABLE);					   // ʹ�ܴ���
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ��������ж�

	// Usart NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  // �����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // ���յ�����
	{
		u8 temp;
		static char count = 0, last_data, last_last_data;
		temp = USART_ReceiveData(USART3);
		//USART_SendData(USART3, temp); // ��������
		if (Usart_Flag == 0)
		{
			if (last_data == 0xfe && last_last_data == 0xff)
				Usart_Flag = 1, count = 0, Usart_ON_Flag = 1;
		}
		if (Usart_Flag == 1)
		{
			Urxbuf[count] = temp;
			count++;
			if (count == 6)
				Usart_Flag = 0;
		}
		last_last_data = last_data;
		last_data = temp;
	}
	return 0;
}
