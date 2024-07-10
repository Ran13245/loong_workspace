#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/UInt8.h>
#include <termios.h>
#include <fcntl.h>
#include <string>
#include <iostream>

#define R 50

using namespace std;

int serial_fd;
uint8_t data[8];
int16_t x, y, z;

int open_serial_port(const char *port); // 打开串口
void configure_serial_port(int fd);     // 初始化串口相关设置

void arm_write()
{
    if (x < 130)
		x = 130;
	else if (x > 300)
		x = 300;
	if (y < -90)
		y = -90;
	else if (y > 160)
		y = 160;
	if (z < -160)
		z = -160;
	else if (z > 180)
		z = 180;

	if (y < 16)
	{
		if (x < 200)
			x = 200;
		else if (x > 270)
			x = 270;
	}
    data[2] = x >> 8;
    data[3] = x;
    data[4] = y >> 8;
    data[5] = y;
    data[6] = z >> 8;
    data[7] = z;
    write(serial_fd, data, 8);
    ROS_INFO("x = %d y = %d z = %d",x,y,z);

}

void circle()
{
    //ROS_INFO("beginning!");
    int16_t p_x = x;
    int16_t p_y = y;
    int16_t p_z = z;
    ros::Rate rate(15);
    ros::Rate st(1);

    y = -45;
    arm_write();
    st.sleep();
    
    x = p_x - 50;
    arm_write();
    st.sleep();
    
    x = p_x;
    arm_write();
    st.sleep();
    
    x = p_x + 50;
    arm_write();
    st.sleep();
    
    x = p_x;
    arm_write();
    st.sleep();

    z = p_z - 10;
    arm_write();
    st.sleep();
    
    z = p_z;
    arm_write();
    st.sleep();
    
    z = p_z + 10;
    arm_write();
    st.sleep();
        
    z = p_z;
    arm_write();
    st.sleep();
    
    x = p_x;
    y = p_y;
    z = p_z;
    st.sleep();
}

//void circle()
//{
//    //ROS_INFO("beginning!");
//   int16_t p_x = x;
//    int16_t p_y = y;
//    int16_t p_z = z;
//    ros::Rate rate(15);
//   ros::Rate st(0.3);
//   ros::Rate st2(0.5);
//    
//    z = -90;
//    arm_write();
//    st.sleep();

//    y -= 40;
//    arm_write();
//    st2.sleep();
//    st2.sleep();

//    for (int i = -90; i < 90; i+=5)
//    {
//        z = i;
 //       arm_write();
  //      rate.sleep();
    //}
   // st.sleep();
    //st2.sleep(); 
    //st2.sleep(); 

    //y = p_y;
    //arm_write();
    //st2.sleep();

//    z = p_z;    
//}

void subCallback1(const geometry_msgs::Vector3 msg)
{
    ROS_INFO("x:%d,y:%d,z:%d", (int)msg.x, (int)msg.y, (int)msg.z);
    x = msg.x;
    y = msg.y;
    z = msg.z;
    arm_write();
}
void subCallback2(const std_msgs::UInt8 msg)
{
    switch (msg.data)
    {
    case 'w':
        y += 10;
        break;
    case 's':
        y -= 10;
        break;
    case 'a':
        z += 10;
        break;
    case 'd':
        z -= 10;
        break;
    case 'q':
        x += 10;
        break;
    case 'e':
        x -= 10;
        break;
    case 'r':
        circle();
        break;
    case 'z':
        x = 220;
        y = 80;
        z = 0;
        break;
    default:
        break;
    }
    arm_write();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "arm_ctrl");
    ros::NodeHandle n;
    ros::Subscriber sub1 = n.subscribe<geometry_msgs::Vector3>("arm/position", 10, subCallback1);
    ros::Subscriber sub2 = n.subscribe<std_msgs::UInt8>("arm/ctrl", 10, subCallback2);

    const char *port = "/dev/ttyS1";
    serial_fd = open_serial_port(port);
    if (serial_fd == -1)
    {
        fprintf(stderr, "Failed to open\n");
        return 1;
    }
    // 初始化串口相关设置
    configure_serial_port(serial_fd);

    x = 220;
    y = 80;
    z = 0;

    data[0] = 0xff;
    data[1] = 0xfe;
    ros::spin();
    close(serial_fd);
    return 0;
}

int open_serial_port(const char *port)
{
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        ROS_ERROR("open_serial_port:error\n");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
    }

    return fd;
}

void configure_serial_port(int fd)
{
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0)
    {
        ROS_ERROR("configure_serial_port:Error\n");
        return;
    }
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;

    tty.c_cflag &= ~CRTSCTS;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 5;
    tty.c_iflag &= ~(INLCR | ICRNL);
    tty.c_iflag &= ~(IXON);

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        ROS_ERROR("configure_serial_port : Error\n");
    }
}
