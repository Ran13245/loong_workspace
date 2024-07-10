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
	
	//等待L610初始化完成
	strx=strstr((const char*)RxBuffer ,(const char *)"+SIM READY");
	while(strx==NULL)
	{
		//printf("正在初始化中\r\n");
		Clear_Buffer();
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+SIM READY");
	}
	//printf("初始化完成\r\n");
	
	//查询版本信息
	printf("ATI\r\n");
	HAL_Delay(1000);
	strx=strstr((const char*)RxBuffer ,(const char *)"Fibocom");
	while(strx==NULL)
	{
		printf("查询信息失败\r\n");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("ATI\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"Fibocom");

	}
	printf("版本信息正确\r\n");
	HAL_Delay(1000);
	
	//请求IP
	printf("AT+MIPCALL?\r\n");
	HAL_Delay(1000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+MIPCALL:1");
	while(strx==NULL)
	{
		printf("还未获取到IP");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+MIPCALL=1\r\n");
		HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+MIPCALL:");
	}
	Clear_Buffer();
	printf("获取IP成功");
	HAL_Delay(1000);
	
	//基站定位
	printf("AT+GTGIS=6\r\n");
	HAL_Delay(2000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+GTGIS:");
	while(strx==NULL)
	{
		printf("基站定位失败");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+GTGIS=6\r\n");
		HAL_Delay(2000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+GTGIS:");
	}
	Clear_Buffer();
	printf("基站定位成功");
	HAL_Delay(1000);
	
	//设置连接参数并连接
	printf("AT+HMCON=0,60,\"3cc394af05.st1.iotda-device.cn-north-4.myhuaweicloud.com\",\"1883\",\"667e74746bc31504f070fbfb_L610_1\",\"11111111\",0\r\n");
	HAL_Delay(2000);
	strx=strstr((const char*)RxBuffer ,(const char *)"+HMCON OK");
	while(strx==NULL)
	{
		printf("连接失败");
		Clear_Buffer();
		HAL_Delay(1000);
		printf("AT+HMCON=0,60,\"3cc394af05.st1.iotda-device.cn-north-4.myhuaweicloud.com\",\"1883\",\"667e74746bc31504f070fbfb_L610_1\",\"11111111\",0\r\n");
		HAL_Delay(2000);
		strx=strstr((const char*)RxBuffer ,(const char *)"+HMCON OK");
	}
	Clear_Buffer();
	printf("连接成功");
	HAL_Delay(1000);
	printf("AT+GTTS=1,\"初始化完成，连接成功\",1\r\n");
}


