#include "show.h"
/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;		  // ��������
unsigned char Send_Count; // ������Ҫ���͵����ݸ���
float Vol;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{
	OLED_ShowString(00, 0, "Volta");
	OLED_ShowString(58, 0, ".");
	OLED_ShowString(80, 0, "V");
	OLED_ShowNumber(45, 0, Voltage / 100, 2, 12);
	OLED_ShowNumber(68, 0, Voltage % 100, 2, 12);
	if (Voltage % 100 < 10)
		OLED_ShowNumber(62, 0, 0, 2, 12);
	OLED_ShowString(00, 10, "V");

	if (Flag_Stop == 0)
		OLED_ShowString(100, 0, "O-N");
	else
		OLED_ShowString(100, 0, "OFF");
	OLED_ShowString(0, 10, "BA");
	if (Pitch < 0)
		OLED_ShowString(15, 10, "-"),
			OLED_ShowNumber(20, 10, -Pitch, 3, 12);
	else
		OLED_ShowString(15, 10, "+"),
			OLED_ShowNumber(20, 10, Pitch, 3, 12);

	OLED_ShowString(40, 10, "UD");
	if (Roll < 0)
		OLED_ShowString(55, 10, "-"),
			OLED_ShowNumber(60, 10, -Roll, 3, 12);
	else
		OLED_ShowString(55, 10, "+"),
			OLED_ShowNumber(60, 10, Roll, 3, 12);

	OLED_ShowString(85, 10, "Z");
	if (gyro[2] < 0)
		OLED_ShowString(95, 10, "-"),
			OLED_ShowNumber(100, 10, -gyro[2], 4, 12);
	else
		OLED_ShowString(95, 10, "+"),
			OLED_ShowNumber(100, 10, gyro[2], 4, 12);

	//=============��3����ʾ���A��״̬=======================//
	if (Target_A < 0)
		OLED_ShowString(00, 20, "-"),
			OLED_ShowNumber(15, 20, -Target_A / (13.76f * 1000) * 180, 6, 12);
	else
		OLED_ShowString(0, 20, "+"),
			OLED_ShowNumber(15, 20, Target_A / (13.76f * 1000) * 180, 6, 12);

	if (Position_A < 0)
		OLED_ShowString(70, 20, "-"),
			OLED_ShowNumber(90, 20, -Position_A / (13.76f * 1000) * 180, 6, 12);
	else
		OLED_ShowString(70, 20, "+"),
			OLED_ShowNumber(90, 20, Position_A / (13.76f * 1000) * 180, 6, 12);
	//=============��4����ʾ���B��״̬=======================//
	if (Target_B < 0)
		OLED_ShowString(00, 30, "-"),
			OLED_ShowNumber(15, 30, -Target_B / (13.76f * 1000) * 180, 6, 12);
	else
		OLED_ShowString(0, 30, "+"),
			OLED_ShowNumber(15, 30, Target_B / (13.76f * 1000) * 180, 6, 12);

	if (Position_B < 0)
		OLED_ShowString(70, 30, "-"),
			OLED_ShowNumber(90, 30, -Position_B / (13.76f * 1000) * 180, 6, 12);
	else
		OLED_ShowString(70, 30, "+"),
			OLED_ShowNumber(90, 30, Position_B / (13.76f * 1000) * 180, 6, 12);
	//=============��5����ʾ���C��״̬=======================//
	if (Target_C < 0)
		OLED_ShowString(00, 40, "-"),
			OLED_ShowNumber(15, 40, -Target_C, 6, 12);
	else
		OLED_ShowString(0, 40, "+"),
			OLED_ShowNumber(15, 40, Target_C, 6, 12);

	if (Position_C < 0)
		OLED_ShowString(70, 40, "-"),
			OLED_ShowNumber(90, 40, -Position_C, 6, 12);
	else
		OLED_ShowString(70, 40, "+"),
			OLED_ShowNumber(90, 40, Position_C, 6, 12);

	OLED_ShowString(00, 50, "X");
	OLED_ShowNumber(7, 50, Target_X, 4, 12);

	OLED_ShowString(37, 50, "Y");
	if (Target_Y < 0)
	{
		OLED_ShowString(50, 50, "-");
		OLED_ShowNumber(56, 50, -Target_Y, 2, 12);
	}
	else
	{
		OLED_ShowString(46, 50, " ");
		OLED_ShowNumber(44, 50, Target_Y, 4, 12);
	}

	OLED_ShowString(74, 50, "Z");
	if (Target_Z < 0)
	{
		OLED_ShowString(86, 50, "-");
		OLED_ShowNumber(92, 50, -Target_Z, 3, 12);
	}
	else
	{
		OLED_ShowString(86, 50, " ");
		OLED_ShowNumber(92, 50, Target_Z, 3, 12);
	}

	if (Menu == 1)
		OLED_ShowString(118, 50, "X");
	else if (Menu == 2)
		OLED_ShowString(118, 50, "Y");
	else if (Menu == 3)
		OLED_ShowString(118, 50, "Z");
	else if (Menu == 4)
		OLED_ShowString(118, 50, "D");
	//OLED_ShowNumber(120, 50, Manipulator_Flag, 1, 12);
	//		//=============ˢ��=======================//
	OLED_Refresh_Gram();
}
