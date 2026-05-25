#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <random>  
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
using namespace std;  
using namespace std::chrono_literals;

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/duration.hpp>
#include <rclcpp/type_adapter.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float32.hpp>

#include <vector>
#include <buttonsWheel.h>


#include <math.h>
#include <cmath>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


class inputs_publisher_class : public rclcpp::Node {
    public:
    inputs_publisher_class();
    ~inputs_publisher_class();
    
    void PublishData(double throttle_val, double brake_val,double steering_val);

    private:

   
    //PUBLISHER
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr ThrottleInputPub;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr BrakeInputPub;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr SteeringInputPub;

    std_msgs::msg::Float32 throttle_msg;
    std_msgs::msg::Float32 brake_msg;
    std_msgs::msg::Float32 steering_msg;
 

};