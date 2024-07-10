#include "control.h"

/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
int Balance_Pwm, Velocity_Pwm, Turn_Pwm;
u8 Flag_Target, Step = 0;
u32 Flash_R_Count;
int Voltage_Temp, Voltage_Count, Voltage_All;
float Last_Position_A, Last_Position_B, Last_Position_C;
/***************************************************
**************************************************************************/
#define l1 (0.14f)
#define l2 (0.16f)
#define h (0.097f)
#define L (0.06f)
/**************************************************************************
�������ܣ��˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/

void Kinematic_Analysis(int Px, int Py, int Pz)
{
	float target_x, target_y, target_z;
	float c, d, e;
	target_x = (float)Px / 1000;
	target_y = (float)Py / 1000;
	target_z = (float)Pz / 180 * PI;
	c = target_x - L;
	d = target_y - h;
	e = sqrt(pow(d, 2) + pow(c, 2));
	Target_A = acos((l1 * l1 + e * e - l2 * l2) / 2 / l1 / e) + atan(d / c);
	Target_B = Target_A - PI + acos((l1 * l1 + l2 * l2 - e * e) / 2 / l1 / l2);
	Target_C = -target_z;

	Target_A *= -RATIO;
	Target_B *= -RATIO;
	Target_C *= -RATIO;
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
		 5ms��ʱ�ж���MPU6050��INT���Ŵ���
		 �ϸ�֤���������ݴ�����ʱ��ͬ��
**************************************************************************/
int EXTI15_10_IRQHandler(void)
{
	if (INT == 0)
	{
		EXTI->PR = 1 << 15; // ����жϱ�־λ
		Flag_Target = !Flag_Target;
		Key();									  //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬
		Position_A = -Read_Encoder(3);			  //===��ȡ��������ֵ
		Position_B = Read_Encoder(2);			  //===��ȡ��������ֵ
		Position_C = Read_Encoder(4);			  //===��ȡ��������ֵ
		Encoder_A = Position_A - Last_Position_A; //===һ���ֵõ��ٶ�ֵ
		Encoder_B = Position_B - Last_Position_B; //===һ���ֵõ��ٶ�ֵ
		Encoder_C = Position_C - Last_Position_C; //===һ���ֵõ��ٶ�ֵ
		if (Usart_Flag == 0 && Usart_ON_Flag == 1)
		{
			memcpy(rxbuf, Urxbuf, 6 * sizeof(uint8_t)); // ��������˴��ڿ��Ʊ�־λ�����봮�ڿ���ģʽ
			Usart_ON_Flag2 = 1;
		}
		Get_Angle(Way_Angle); //===������̬
		if (CAN_ON_Flag == 1 || Usart_ON_Flag2 == 1)
		{
			CAN_N_Usart_Control(); // �ӵ����ڻ���CANң�ؽ���ָ��֮��ʹ��CAN�ʹ��ڿ�������
			Usart_ON_Flag = 0;
			Usart_ON_Flag2 = 0;
		}
		if (Xianfu_XYZ()) // �������˶���Χ�Ҳ����϶�ʾ��ģʽ�£�ִ���˶�ѧ����
		{
			Kinematic_Analysis(Target_X, Target_Y, Target_Z); // ���ݸ������꣬�����ÿ�������λ��
		}
		Motor_A = Position_PID_A(Position_A, Target_A) >> 5;  // A���λ�ÿ���
		Motor_B = Position_PID_B(Position_B, Target_B) >> 5;  // B���λ�ÿ���
		Motor_C = -Position_PID_C(Position_C, Target_C) >> 5; // C���λ��
		Count_Velocity();									  // �ٶ�ͬ������
		if ((Position_A - Position_B > -3120) && myabs(Encoder_A) > myabs(Encoder_B))
			Motor_A = Motor_B; // ��ֹ���ָ���

		Xianfu_Velocity(2 * Max_Velocity, 2 * Max_Velocity, 2 * Max_Velocity); // �ٶ��޷�
		if (Turn_Off() == 0)												   // �������쳣
			Set_Pwm(Motor_A, Motor_B, Motor_C);								   // ���Ƶ��
		// Manipulator();														   // ��е�ۿ���
		Last_Position_A = Position_A;	   // ������ʷ����
		Last_Position_B = Position_B;	   // ������ʷ����
		Last_Position_C = Position_C;	   // ������ʷ����
		Voltage_Temp = Get_battery_volt(); //=====��ȡ��ص�ѹ
		Voltage_Count++;				   //=====ƽ��ֵ������
		Voltage_All += Voltage_Temp;	   //=====��β����ۻ�
		if (Voltage_Count == 100)
			Voltage = Voltage_All / 100, Voltage_All = 0, Voltage_Count = 0; //=====��ƽ��ֵ
		Led_Flash(100);														 //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬
	}
	return 0;
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a, int motor_b, int motor_c)
{
	int Final_Motor_A, Final_Motor_B, Final_Motor_C;
	if (motor_a < 0)
		INA = 1; // ���A�������
	else
		INA = 0;
	if (motor_b < 0)
		INB = 0; // ���B�������
	else
		INB = 1;
	if (motor_c < 0)
		INC = 1; // ���C�������
	else
		INC = 0;
	Final_Motor_A = Linear_Conversion(motor_a); // ���Ի�
	Final_Motor_B = Linear_Conversion(motor_b);
	Final_Motor_C = Linear_Conversion(motor_c);

	Set_PWM_End(Final_Motor_A, Final_Motor_B, Final_Motor_C);
}
/**************************************************************************
�������ܣ��Կ��������PWM���Ի�,���ڸ�ϵͳ�Ĵ�����ֵ
��ڲ�����PWM
����  ֵ�����Ի����PWM
**************************************************************************/
u16 Linear_Conversion(int motor)
{
	u32 temp;
	u16 Linear_Moto;
	temp = 1000000 / myabs(motor); // 1000000�Ǿ���ֵ
	if (temp > 65535)
		Linear_Moto = 65535;
	else
		Linear_Moto = (u16)temp;
	return Linear_Moto;
}
/**************************************************************************
�������ܣ�λ��PID���ƹ������ٶȵ�����
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Velocity(float amplitude_A, float amplitude_B, float amplitude_C)
{
	if (Motor_A < -amplitude_A)
		Motor_A = -amplitude_A; // λ�ÿ���ģʽ�У�A����������ٶ�
	if (Motor_A > amplitude_A)
		Motor_A = amplitude_A; // λ�ÿ���ģʽ�У�A����������ٶ�
	if (Motor_B < -amplitude_B)
		Motor_B = -amplitude_B; // λ�ÿ���ģʽ�У�B����������ٶ�
	if (Motor_B > amplitude_B)
		Motor_B = amplitude_B; // λ�ÿ���ģʽ�У�B����������ٶ�
	if (Motor_C < -amplitude_C)
		Motor_C = -amplitude_C; // λ�ÿ���ģʽ�У�C����������ٶ�
	if (Motor_C > amplitude_C)
		Motor_C = amplitude_C; // λ�ÿ���ģʽ�У�C����������ٶ�
}

/**************************************************************************
�������ܣ��������뷶Χ
��ڲ�������
����  ֵ����
**************************************************************************/
u8 Xianfu_XYZ(void)
{
	u8 temp;
	float S1, S2, B1, B2, B3;
	float Temp_Target_X, Temp_Target_Y;

	static float Last_Target_A1, Last_Target_B1;

	Temp_Target_X = (float)Target_X / 1000 - L;
	Temp_Target_Y = (float)Target_Y / 1000 - h;
	if (Target_Y / 1000 < -0.11952f)
	{
		S1 = Temp_Target_X * Temp_Target_X + (Temp_Target_Y + 0.16f) * (Temp_Target_Y + 0.16f);
		S2 = Temp_Target_Y * Temp_Target_Y + (-Temp_Target_X - 0.14f) * (-Temp_Target_X - 0.14f);
		if (S1 > 0.0196f && S2 < 0.0256f)
		{
			temp = 1;
			Last_Target_A1 = Target_A;
			Last_Target_B1 = Target_B;
		}
		else
		{
			temp = 0;
			Target_A = Last_Target_A1;
			Target_B = Last_Target_B1;
		}
	}
	else
	{
		B1 = Temp_Target_X * Temp_Target_X + (Temp_Target_Y - 0.14f) * (Temp_Target_Y - 0.14f);
		B2 = Temp_Target_X * Temp_Target_X + Temp_Target_Y * Temp_Target_Y;
		B3 = Temp_Target_X * Temp_Target_X + (Temp_Target_Y + 0.16f) * (Temp_Target_Y + 0.16f);
		if (B1 >= 0.0256f && B2 >= 0.00391876f && B3 >= 0.0196f && B2 <= 0.07498287f)
		{
			temp = 1;
			Last_Target_A1 = Target_A;
			Last_Target_B1 = Target_B;
		}
		else
		{
			temp = 0;
			Target_A = Last_Target_A1;
			Target_B = Last_Target_B1;
		}
	}
	return temp;
}

/**************************************************************************
�������ܣ������޸�����״̬
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{
	u8 temp;
	static u8 count;
	float Step = 10, Threshold_Angle = 10, Threshold_Gyro = 600;
	temp = click(); // �����������
	if (temp == 1)
		Flag_Stop = !Flag_Stop; // ��������С������ͣ///S����
	else if (temp == 5)			/// Menu
	{
		if (Menu++ == 4)
			Menu = 1;
	}
	else if (temp == 2) //++
	{
		if (Menu == 1)
			Target_X += Step;
		else if (Menu == 2)
			Target_Y += Step;
		else if (Menu == 3)
			Target_Z += Step;
		else if (Menu == 4)
			Manipulator_Flag++;
		if (Manipulator_Flag > 3)
			Manipulator_Flag = 1;
	}
	else if (temp == 4) //--
	{
		if (Menu == 1)
			Target_X -= Step;
		else if (Menu == 2)
			Target_Y -= Step;
		else if (Menu == 3)
			Target_Z -= Step;
		else if (Menu == 4)
			Manipulator_Flag--;
		if (Manipulator_Flag < 1)
			Manipulator_Flag = 3;
	}
	if (++count == 10)
	{
		if (KEY_X == 0) // ��̬��Ӧ����
		{
			if (Pitch > Threshold_Angle)
				Target_X += Step / 5;
			if (Pitch < -Threshold_Angle)
				Target_X -= Step / 5;

			if (Roll > Threshold_Angle)
				Target_Y += Step / 5;
			if (Roll < -Threshold_Angle)
				Target_Y -= Step / 5;

			if (gyro[2] > Threshold_Gyro && myabs(Pitch) < Threshold_Angle && myabs(Roll) < Threshold_Angle)
				Target_Z += Step / 2;
			if (gyro[2] < -Threshold_Gyro && myabs(Pitch) < Threshold_Angle && myabs(Roll) < Threshold_Angle)
				Target_Z -= Step / 2;
		}
		count = 0; // ����������
	}
	if (Target_X < 130)
		Target_X = 130;
	else if (Target_X > 300)
		Target_X = 300;
	if (Target_Y < -90)
		Target_Y = -90;
	else if (Target_Y > 160)
		Target_Y = 160;
	if (Target_Z < -160)
		Target_Z = -160;
	else if (Target_Z > 180)
		Target_Z = 180;

	if (Target_Y < 16)
	{
		if (Target_X < 200)
			Target_X = 200;
		else if (Target_X > 270)
			Target_X = 270;
	}
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(void)
{
	u8 temp;
	if (1 == Flag_Stop) //===Flag_Stop��1�رյ��
	{
		temp = 1;
		Motor_A = 0;
		Motor_B = 0;
		Motor_C = 0;
		ST = 0; // ���ʧ��
	}
	else
		temp = 0,
		ST = 1;
	return temp;
}

/**************************************************************************
�������ܣ���ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ�����
��ڲ�������ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/
void Get_Angle(u8 way)
{
	if (way == 1)	//===DMP�Ķ�ȡ�����ݲɼ��ж϶�ȡ���ϸ���ѭʱ��Ҫ��
		Read_DMP(); //===��ȡ���ٶȡ����ٶȡ����
}
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int input_temp)
{
	int temp;
	if (input_temp < 0)
		temp = -input_temp;
	else
		temp = input_temp;
	return temp;
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)��������ƫ��
e(k-1)������һ�ε�ƫ��
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID_A(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;																   // ����ƫ��
	Integral_bias += Bias;																	   // ���ƫ��Ļ���
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias); // λ��ʽPID������
	Last_Bias = Bias;																		   // ������һ��ƫ��
	return Pwm;																				   // �������
}
int Position_PID_B(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;																   // ����ƫ��
	Integral_bias += Bias;																	   // ���ƫ��Ļ���
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias); // λ��ʽPID������
	Last_Bias = Bias;																		   // ������һ��ƫ��
	return Pwm;																				   // �������
}
int Position_PID_C(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;																   // ����ƫ��
	Integral_bias += Bias;																	   // ���ƫ��Ļ���
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias); // λ��ʽPID������
	Last_Bias = Bias;																		   // ������һ��ƫ��
	return Pwm;																				   // �������
}
/***************************************************uos***********************
�������ܣ�ÿ�����λ�ÿ��ƹ����ٶȼ���
��ڲ�������
����  ֵ����
**************************************************************************/
void Count_Velocity(void)
{
	static double Last_Target_A = 0, Last_Target_B = 0, Divider = 1;
	double Bias_A, Bias_B;
	Bias_A = (Target_A - Last_Target_A); // ��X��λ����
	Bias_B = (Target_B - Last_Target_B); // ��Y��λ����
	//	Bias_C=(Target_C-Last_Target_C);	//��Z��λ����

	if (Bias_A != 0 || Bias_B != 0)
	{
		Divider = sqrt(Bias_A * Bias_A + Bias_B * Bias_B);
		Rate_A = myabs(Bias_A);
		Rate_B = myabs(Bias_B);
		// Rate_C=myabs(Bias_C);
	}
	Xianfu_Velocity(Rate_A * RC_Velocity / Divider, Rate_B * RC_Velocity / Divider, RC_Velocity);
	Last_Target_A = Target_A; // ����X����һ�ε�λ����Ϣ�����ڵ���
	Last_Target_B = Target_B; // ����Y����һ�ε�λ����Ϣ�����ڵ���
							  // Last_Target_C=Target_C;   //����Z����һ�ε�λ����Ϣ�����ڵ���
}

/**************************************************************************
�������ܣ�����CAN���ߴ��ڿ���ָ����д���
��ڲ�������
����  ֵ����
**************************************************************************/
void CAN_N_Usart_Control(void)
{
	Target_X = (int)(rxbuf[0] *256 + rxbuf[1]);
	Target_Y = (int)(rxbuf[2] *256 + rxbuf[3]);
	Target_Z = (int)(rxbuf[4] *256 + rxbuf[5]);
	// RC_Velocity = (rxbuf[6] & 0x0f) * 256 + rxbuf[7];
	// Manipulator_Flag = rxbuf[6] >> 4;
}
/**************************************************************************
�������ܣ�����CAN���ߴ��ڿ���ָ����д���
��ڲ�������
����  ֵ����
**************************************************************************/
void Manipulator(void)
{
	if (1 == Manipulator_Flag)
	{
		AIN1 = 0; // A��ŷ� �ر�
		AIN2 = 0;
		BIN1 = 0; // B�����ùر�
		BIN2 = 0;
		CIN1 = 0; // C������ �ر�
		CIN2 = 0;
	}
	if (2 == Manipulator_Flag) // ����
	{
		AIN1 = 1; // A��ŷ� ��
		AIN2 = 0;
		BIN1 = 0; // B������ �ر�
		BIN2 = 0;
		CIN1 = 1; // C������ ��
		CIN2 = 0;
	}
	if (3 == Manipulator_Flag) // ����
	{
		AIN1 = 0; // A��ŷ� �ر�
		AIN2 = 0;
		BIN1 = 1; // B������ ��
		BIN2 = 0;
		CIN1 = 0; // C������ �ر�
		CIN2 = 0;
	}
}
