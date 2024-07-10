#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#define KEY_A PBin(0)
#define KEY_B PBin(1)
#define KEY_C PBin(12)

#define KEY_P PCin(12)
#define KEY_J PAin(11)
#define KEY_M PAin(4)
#define KEY_X PAin(12)

#define KEY PBin(14)
void KEY_Init(void);          //按键初始化
u8 click_N_Double (u8 time);  //单击按键扫描和双击按键扫描
u8 click(void);               //单击按键扫描
u8 Long_Press(void);

#endif 
