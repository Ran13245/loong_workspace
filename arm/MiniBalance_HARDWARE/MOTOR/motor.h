#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define INA   PBout(4)  
#define INB   PBout(5)  
#define INC   PDout(2)  
#define ST    PCout(1) 
//////�����ǻ�еצ���ƽӿ�
#define AIN1 PCout(2)
#define AIN2 PCout(3)
#define BIN1 PCout(4)
#define BIN2 PCout(5)
#define CIN1 PCout(6)
#define CIN2 PCout(7) 

void MiniBalance_PWM_Init(u16 psc);
void MiniBalance_Motor_Init(void);
void Set_PWM_End(u16 arr1,u16 arr2,u16 arr3);

#endif
