#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <std_msgs/UInt8.h>
#include <opencv2/highgui/highgui.hpp>  
#include <cv_bridge/cv_bridge.h>  
#include <sstream> // for converting the command line parameter to integer  

bool work;
void CameraCallback(const std_msgs::UInt8 &msg)
{
    work = 1;
}

int main(int argc, char** argv)  
{  
	// Check if video source has been passed as a parameter  
	if(argv[1] == NULL)   
	{  
		ROS_INFO("argv[1]=NULL\n");  
		return 1;  
	}  

	ros::init(argc, argv, "cam_pub");  
	ros::NodeHandle nh;  
	image_transport::ImageTransport it(nh);  
	image_transport::Publisher pub = it.advertise("camera/image", 1);  
    ros::Subscriber sub_cam = nh.subscribe("camera/ctrl", 1, CameraCallback);

	// Convert the passed as command line parameter index for the video device to an integer  
	std::istringstream video_sourceCmd(argv[1]);  
	int video_source;  
	// Check if it is indeed a number  
	if(!(video_sourceCmd >> video_source))   
	{  
		ROS_INFO("video_sourceCmd is %d\n",video_source);  
		return 1;  
	}  

	cv::VideoCapture cap(video_source);  
	// Check if video device can be opened with the given index  
	if(!cap.isOpened())   
	{  
		ROS_INFO("can not opencv video device\n");  
		return 1;  
	}  
	cv::Mat frame;  
	sensor_msgs::ImagePtr msg; 
    work = 0; 
    
	//ros::Rate loop_rate(1);  
	while (nh.ok()) 
	{  
        if(work == 1)
        {
            for(int i = 0 ;i< 4;i++)
                cap.grab();
		    cap >> frame;  
		    // Check if grabbed frame is actually full with some content  
		    if(!frame.empty()) 
		    {  
                cv::resize(frame, frame, cv::Size(640, 480));  
			    msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
			    pub.publish(msg);  
			    //cv::Wait(1);  
		    }  
            work = 0;
		    //loop_rate.sleep();  
        }	
	    ros::spinOnce();              
    }  
}  
