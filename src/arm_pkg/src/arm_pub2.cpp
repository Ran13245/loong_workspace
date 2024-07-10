#include <ros/ros.h>
#include <std_msgs/UInt8.h>
#include <iostream>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "arm_pub2");
    ros::NodeHandle nh;
    ros::Publisher pub2 = nh.advertise<std_msgs::UInt8>("arm/ctrl", 10);
    
    std_msgs::UInt8 c;

    while (ros::ok())
    {
        std::cin >> c.data;
        pub2.publish(c);
    }

    return 0;
}
