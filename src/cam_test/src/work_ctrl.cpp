#include <ros/ros.h>
#include <std_msgs/UInt8.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <termios.h>
#include <fcntl.h>

uint8_t working = 0;
uint8_t problem = 0;
class A
{
public:
    int serial_fd;
    uint8_t data;
    std_msgs::UInt8 arm_ctrl;
    ros::NodeHandle nh;
    ros::Publisher cam_pub;
    ros::Publisher arm_pub2;    
    ros::Subscriber sub;
    ros::Subscriber flag_sub ;
    
    A()
    {
        cam_pub = nh.advertise<std_msgs::UInt8>("camera/ctrl", 1);
        arm_pub2 = nh.advertise<std_msgs::UInt8>("arm/ctrl", 1);
        sub = nh.subscribe("work", 1, &A::workCallback,this);
        flag_sub = nh.subscribe("work/flag", 1, &A::flagCallback,this);
    }   

    void workCallback(const std_msgs::UInt8 msg)
    {
        working = msg.data;
    }


    void flagCallback(const std_msgs::UInt8 msg)
    {
        ros::Rate st(0.066);
        problem = msg.data;
        if (problem == 1)
        {
            data = 1;
            write(serial_fd, &data, 1); // 串口发停止
            arm_ctrl.data = 'r';
            arm_pub2.publish(arm_ctrl); // circle
            st.sleep();                 // 延时
            data = 0;
            write(serial_fd, &data, 1); // 串口发继续
        }
    }
};
//void problemCallback(const sensor_msgs::ImageConstPtr &msg)
//{
//    problem = 1;
//}

int open_serial_port(const char *port); // 打开串口
void configure_serial_port(int fd);     // 初始化串口相关设置

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cam_sub");
    A a;

  //  image_transport::ImageTransport it(nh);
//    image_transport::Subscriber problem_sub = it.subscribe("problem/image", 1, problemCallback);

    const char *port = "/dev/ttyS2";
    a.serial_fd = open_serial_port(port);
    if (a.serial_fd == -1)
    {
        fprintf(stderr, "Failed to open\n");
        return 1;
    }
    // 初始化串口相关设置
    configure_serial_port(a.serial_fd);
    std_msgs::UInt8 cam_ctrl;

    ros::Rate rate(0.2); // 拍照频率

    while (ros::ok())
    {
        if (working && problem != 2)
        {
            // 定时拍照
            a.cam_pub.publish(cam_ctrl);
            problem = 2;
            //rate.sleep();
            
        }
        ros::spinOnce();
    }
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
