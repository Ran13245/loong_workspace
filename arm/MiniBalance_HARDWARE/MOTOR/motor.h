#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define INA   PBout(4)  
#define INB   PBout(5)  
#define INC   PDout(2)  
#define ST    PCout(1) 
//////以下是机械爪控制接口
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
