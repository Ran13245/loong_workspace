#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/UInt8.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>

bool flag_pic = 0;

bool flag_pub = 0;
uint8_t problem;
cv::Mat pub_image;

cv::Mat processImage(const cv::Mat &original_image);

cv::Mat processImageFromFile(const sensor_msgs::ImageConstPtr &msg)
{
    // cv::Mat image = cv::imread(image_path_, cv::IMREAD_COLOR);
    cv::Mat image = cv_bridge::toCvShare(msg, "bgr8")->image;
    if (image.empty())
    {
        ROS_ERROR("Could not open or find the image at ");
    }

    // Resize the image to 720x460
    cv::resize(image, image, cv::Size(720, 460));

    cv::Mat processed_image = processImage(image);
    return processed_image;
    // Display the processed image
    // cv::imshow("Processed Image", processed_image);
    // cv::waitKey(0); // Wait for a keystroke in the window
}

cv::Mat processImage(const cv::Mat &original_image)
{
    cv::Mat image = original_image.clone(); // Make a copy of the original image

    // Convert to grayscale for easier processing
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    // Apply a blur to smooth the image and increase the averaging pixel matrix size
    cv::Mat blurred_image;
    int kernel_size = 15; // Adjust this value to change the size of the averaging pixel matrix
    cv::blur(gray_image, blurred_image, cv::Size(kernel_size, kernel_size));

    // Calculate the mean of the blurred image
    cv::Scalar mean_scalar = cv::mean(blurred_image);
    double mean_value = mean_scalar[0];

    // First threshold based on mean value * 0.7
    cv::Mat binary_image1;
    double threshold_value1 = mean_value * 0.7; // You can adjust this factor as needed
    cv::threshold(blurred_image, binary_image1, threshold_value1, 255, cv::THRESH_BINARY_INV);

    // Second threshold based on mean value * 1.3 (adjust this factor as needed)
    cv::Mat binary_image2;
    double threshold_value2 = mean_value * 1.3; // Adjust this factor as needed
    cv::threshold(blurred_image, binary_image2, threshold_value2, 255, cv::THRESH_BINARY_INV);

    // Show intermediate images for debugging
    // cv::imshow("Gray Image", gray_image);
    // cv::imshow("Blurred Image", blurred_image);
    // cv::imshow("Binary Image 1", binary_image1);
    // cv::imshow("Binary Image 2", binary_image2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> contours1;
    cv::findContours(binary_image1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(binary_image2, contours1, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    double min_area = image.rows * image.cols / 100.0; // Minimum area threshold, 1/100 of the image area
    double max_area = image.rows * image.cols / 1.5;   // Maximum area threshold, to avoid large boundary contours

    bool detected = false; // Flag to track if any contour is detected

    for (const auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > min_area && area < max_area)
        {
            cv::Rect bounding_box = cv::boundingRect(contour);

            // Check if the bounding box is within the image boundary to avoid marking the whole image
            if (bounding_box.x > 0 && bounding_box.y > 0 &&
                bounding_box.x + bounding_box.width < image.cols &&
                bounding_box.y + bounding_box.height < image.rows)
            {
                cv::rectangle(image, bounding_box, cv::Scalar(0, 255, 0), 2);
                detected = true; // Set detected flag to true if any contour is detected
            }
        }
    }

    for (const auto &contour : contours1)
    {
        double area = cv::contourArea(contour);
        if (area > min_area && area < max_area)
        {
            cv::Rect bounding_box = cv::boundingRect(contour);

            // Check if the bounding box is within the image boundary to avoid marking the whole image
            if (bounding_box.x > 0 && bounding_box.y > 0 &&
                bounding_box.x + bounding_box.width < image.cols &&
                bounding_box.y + bounding_box.height < image.rows)
            {
                cv::rectangle(image, bounding_box, cv::Scalar(0, 255, 0), 2);
                detected = true; // Set detected flag to true if any contour is detected
            }
        }
    }

    // If no contours were detected, return the original image
    if (!detected)
    {
        problem = 0;
        return original_image.clone(); // Return a copy of the original image
    }
    flag_pub=1;
    problem = 1;
    return image;
}


void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    //if (flag_pic == 1)
    //{
        pub_image = processImageFromFile(msg);
        //flag_pic = 0;
    //}
}

//void CameraCallback(const std_msgs::UInt8 &msg)
//{
//    flag_pic = 1;
//}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_processor");

    // if (argc != 2)
    // {
    //     ROS_ERROR("Usage: rosrun your_package_name image_processor <image_path>");
    //     return 1;
    // }

    ros::NodeHandle nh;
    // cv::namedWindow("view");
    //  cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("problem/image", 1);
    image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
    
    ros::Publisher pro_pub = nh.advertise<std_msgs::UInt8>("work/flag", 1);
    //ros::Subscriber sub_cam = nh.subscribe("camera/ctrl", 1, CameraCallback);
    sensor_msgs::ImagePtr msg; 
    std_msgs::UInt8 pro_msg;

    while (ros::ok())
    {
        if(flag_pub==1)
        {
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", pub_image).toImageMsg();  
			pub.publish(msg); 
            pro_msg.data = problem;
            pro_pub.publish(pro_msg);
            flag_pub=0;
        }
        ros::spinOnce();
    }
    // ros::spin();

    return 0;
}
