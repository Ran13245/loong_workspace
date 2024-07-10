extern int led;
extern int motor_speed;
extern int speed_change_flag;
extern int dir_1;
extern int dir_2;
extern int final_motor1,final_motor2;
extern int motor1_stop;
extern int motor2_stop;
void Motor_enable(void);

void Motor_disable(void);

void Motor_speed_Set(int motor_speed);

void Motor_direction_1(int dir);

void Motor_direction_2(int dir);

void control(int motor, int speed, int direction);