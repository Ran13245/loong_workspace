#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265f
#define RATIO REDUCTION * 1000 / PI
#define DIFFERENCE 100
extern int Balance_Pwm, Velocity_Pwm, Turn_Pwm;
void Kinematic_Analysis(int Px, int Py, int Pz);
void Kinematic_Analysis2(int Px, int Py, int Pz);
int EXTI15_10_IRQHandler(void);
int balance(float angle, float gyro);
int velocity(int encoder_left, int encoder_right);
int turn(int encoder_left, int encoder_right, float gyro);
void Set_Pwm(int motor_a, int motor_b, int motor_c);
void Key(void);
void Xianfu_Velocity(float amplitude_A, float amplitude_B, float amplitude_C);
u8 Turn_Off(void);
void Get_Angle(u8 way);
int myabs(int input_temp);
u16 Linear_Conversion(int motor);
int Position_PID_A(int Encoder, int Target);
int Position_PID_B(int Encoder, int Target);
int Position_PID_C(int Encoder, int Target);
int Incremental_PI_B(int Encoder, int Target);
void Count_Velocity(void);
u8 Xianfu_XYZ(void);
void CAN_N_Usart_Control(void);
void Manipulator(void);
#endif
