#include<ros/ros.h>
#include<geometry_msgs/Vector3.h>
#include<iostream>

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"arm_pub1");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Vector3>("arm/position",10);

    uint16_t x,y,z;
    while(ros::ok())
    {
        geometry_msgs::Vector3 msg;
        std::cin >> x >> y >> z;
        msg.x = x;
        msg.y = y;
        msg.z = z;
        pub.publish(msg);
        ROS_INFO("x:%d,y:%d,z:%d",x,y,z);
    }
    return 0;
}
