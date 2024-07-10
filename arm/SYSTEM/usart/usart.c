#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
// // ���ʹ��ucos,����������ͷ�ļ�����.
// #if SYSTEM_SUPPORT_OS
// #include "includes.h" //ucos ʹ��
// #endif

//////////////////////////////////////////////////////////////////
// �������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// ��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
	x = x;
}
// �ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		;
	USART1->DR = (u8)ch;
	return ch;
}
#endif

int Usart_Receive;
/**************************ʵ�ֺ���**********************************************
 *��    ��:		usart1����һ���ֽ�
 *********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while ((USART1->SR & 0x40) == 0)
		;
}
// ����1�жϷ������
// bound:������
void uart_init(u32 bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // ʹ��USART1ʱ��
	// ����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // GPIOA10����ΪUSART1
	// USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// ��ʼ��PA9��PA10
	// USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										// ��ʼ������1

	USART_Cmd(USART1, ENABLE);					   // ʹ�ܴ���1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ��������ж�
	// Usart NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // �����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

/**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //
	{
		u8 temp;
		static u8 count, last_data, last_last_data, Usart_ON_Count;
		// if (Usart_ON_Flag == 0)
		// {
		// 	if (++Usart_ON_Count > 10)
				Usart_ON_Flag = 1;
		// }
		temp = USART_ReceiveData(USART1);
		if (Usart_Flag == 0)
		{
			if (last_data == 0xfe && last_last_data == 0xff)
				Usart_Flag = 1, count = 0;
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

/**************************************************************************
�������ܣ�ͨ�����ڰ������Ĵ��������ͳ�ȥ
**************************************************************************/
void USART_TX(void)
{
	//        u8 Direction_A,Direction_B,Direction_C;
	//	      u16 Temp_GZ;
	//	           if(Encoder_A>0) Direction_A=0;
	//        else if(Encoder_A<0) Direction_A=2;
	//	      else                 Direction_A=1;
	//		         if(Encoder_B>0) Direction_B=0;
	//        else if(Encoder_B<0) Direction_B=2;
	//	      else                 Direction_B=1;
	//		         if(Encoder_C>0) Direction_C=0;
	//        else if(Encoder_C<0) Direction_C=2;
	//	      else                 Direction_C=1;
	//      	Temp_GZ=Gryo_Z+32768;
	//
	//				usart1_send(0xff);
	//				usart1_send(0xfe);
	//				usart1_send(abs(Encoder_A));
	//				usart1_send(Direction_A);
	//				usart1_send(abs(Encoder_B));
	//				usart1_send(Direction_B);
	//				usart1_send(abs(Encoder_C));
	//				usart1_send(Direction_C);
	//				usart1_send(Temp_GZ>>8);
	//				usart1_send(Temp_GZ&0x00ff);
	//
	//				usart3_send(0xff);
	//				usart3_send(0xfe);
	//				usart3_send(abs(Encoder_A));
	//				usart3_send(Direction_A);
	//				usart3_send(abs(Encoder_B));
	//				usart3_send(Direction_B);
	//				usart3_send(abs(Encoder_C));
	//				usart3_send(Direction_C);
	//				usart3_send(Temp_GZ>>8);
	//				usart3_send(Temp_GZ&0x00ff);
}
