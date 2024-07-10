#include <ros/ros.h>
#include <std_msgs/UInt8.h>
#include <geometry_msgs/Vector3.h>
#include <fcntl.h>
#include <termios.h>
#include <image_transport/image_transport.h>
#include <string>
#include <iostream>

int serial_fd;
uint8_t data[8];
uint16_t x, y, z;

int open_serial_port(const char *port); // 打开串口
void configure_serial_port(int fd);     // 初始化串口相关设置

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "ctrl_pub");
    ros::NodeHandle nh;
    ros::Publisher arm_pub1 = nh.advertise<geometry_msgs::Vector3>("arm/position", 10);
    ros::Publisher arm_pub2 = nh.advertise<std_msgs::UInt8>("arm/ctrl",10);
    ros::Publisher cam_pub = nh.advertise<std_msgs::UInt8>("camera/ctrl", 1);
    ros::Publisher work_pub = nh.advertise<std_msgs::UInt8>("work", 1);

    // image_transport::ImageTransport it(nh);
    // image_transport::Publisher cam_pub = it.advertise("camera/image",1);

    const char *port = "/dev/ttyS2";
    serial_fd = open_serial_port(port);
    if (serial_fd == -1)
    {
        fprintf(stderr, "Failed to open\n");
        return 1;
    }
    configure_serial_port(serial_fd);

    uint8_t s[100];
    geometry_msgs::Vector3 arm_pos;
    std_msgs::UInt8 arm_ctrl;
    std_msgs::UInt8 cam_ctrl;
    std_msgs::UInt8 work_ctrl;
    cam_ctrl.data = 1;

    while (ros::ok())
    {
        if (read(serial_fd, s, 1) > 0)
        {
            ROS_INFO("%d,%d", s[0],s[1]);
            switch (s[0])
            {
            case 1:
                arm_ctrl.data = 'w';
                arm_pub2.publish(arm_ctrl);
                break;
            case 2:
                arm_ctrl.data = 's';
                arm_pub2.publish(arm_ctrl);
                break;
            case 3:
                arm_ctrl.data = 'a';
                arm_pub2.publish(arm_ctrl);
                break;
            case 4:
                arm_ctrl.data = 'd';
                arm_pub2.publish(arm_ctrl);
                break;
            case 6:
                arm_ctrl.data = 'q';
                arm_pub2.publish(arm_ctrl);
                break;
            case 7:
                arm_ctrl.data = 'e';
                arm_pub2.publish(arm_ctrl);
                break;
            case 8:
                arm_ctrl.data = 'r';
                arm_pub2.publish(arm_ctrl);
                break;
            case 5:
                cam_pub.publish(cam_ctrl);
                break;
            case 9:     //work start
                work_ctrl.data = 1;
                work_pub.publish(work_ctrl);
                break;
            case 10:    //work finish
                work_ctrl.data = 0;
                work_pub.publish(work_ctrl);
                break;
            default:
                break;
            }
        }
    }
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
