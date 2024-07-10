#include "main.h"
#include "L610.h"
#include "usart.h"
char *strx,*extstrx,*Readystrx;
char RxBuffer[1024],Rxcouter;
char *strstr(const char *, const char *);
uint8_t Res;

int len;
int number;

uint8_t str_buff[64];


void Clear_Buffer(void)
{
	uint8_t i;
	for(i=0;i<Rxcouter;i++)
		RxBuffer[i]=0;
	Rxcouter=0;
}

void L610_init(void)
{
	HAL_UART_Receive_IT(&huart3,(uint8_t *)&Res, 1);
	HAL_Delay(1000);
	
	//�ȴ�L610��ʼ�����
	strx=strstr((const char*)RxBuffer ,(const char *)"+SIM READY");
	while(strx==NULL)
	{
		//printf("���ڳ�ʼ����\r\n");
		Clear_Buffer();
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+SIM READY");
	}
	//printf("��ʼ�����\r\n");
	
	//��ѯ�汾��Ϣ
	printf("ATI\r\n");
	HAL_Delay(1000);
	strx=strstr((const char*)RxBuffer ,(const char *)"Fibocom");
	while(strx==NULL)
	{
		printf("��ѯ��Ϣʧ��\r\n");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("ATI\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"Fibocom");

	}
	printf("�汾��Ϣ��ȷ\r\n");
	HAL_Delay(1000);
	
	//����IP
	printf("AT+MIPCALL?\r\n");
	HAL_Delay(1000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+MIPCALL:1");
	while(strx==NULL)
	{
		printf("��δ��ȡ��IP");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+MIPCALL=1\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+MIPCALL:");
	}
	Clear_Buffer();
	printf("��ȡIP�ɹ�");
	HAL_Delay(1000);
	
	//��վ��λ
	printf("AT+GTGIS=6\r\n");
	HAL_Delay(2000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+GTGIS:");
	while(strx==NULL)
	{
		printf("��վ��λʧ��");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+GTGIS=6\r\n");
		HAL_Delay(2000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+GTGIS:");
	}
	Clear_Buffer();
	printf("��վ��λ�ɹ�");
	HAL_Delay(1000);
	
	//�������Ӳ���������
	printf("AT+HMCON=0,60,\"3cc394af05.st1.iotda-device.cn-north-4.myhuaweicloud.com\",\"1883\",\"667e74746bc31504f070fbfb_L610_1\",\"11111111\",0\r\n");
	HAL_Delay(2000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+HMCON OK");
	while(strx==NULL)
	{
		printf("����ʧ��");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+HMCON=0,60,\"3cc394af05.st1.iotda-device.cn-north-4.myhuaweicloud.com\",\"1883\",\"667e74746bc31504f070fbfb_L610_1\",\"11111111\",0\r\n");
		HAL_Delay(2000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+HMCON OK");
	}
	Clear_Buffer();
	printf("���ӳɹ�");
	HAL_Delay(1000);
	printf("AT+GTTS=1,\"��ʼ����ɣ����ӳɹ�\",1\r\n");
}


