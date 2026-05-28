#include <Inputs_Publisher.hpp>


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

//obstacle_detection_class::obstacle_detection_class(){ //construtor
inputs_publisher_class::inputs_publisher_class()
: Node("InputsPublisher")
{
    
   
    //PUBLISHER
    ThrottleInputPub = this->create_publisher<std_msgs::msg::Float32>("wheel_throttle_topic",1);
    BrakeInputPub = this->create_publisher<std_msgs::msg::Float32>("wheel_brake_topic",1);
    SteeringInputPub = this->create_publisher<std_msgs::msg::Float32>("wheel_steering_topic",1);
    

}

inputs_publisher_class::~inputs_publisher_class(){
    //destructor

}



void inputs_publisher_class::PublishData(double throttle_val, double brake_val,double steering_val){
 
    throttle_msg.data=throttle_val;
    brake_msg.data=brake_val;
    steering_msg.data=steering_val;
    ThrottleInputPub->publish(throttle_msg);
    BrakeInputPub->publish(brake_msg);
    SteeringInputPub->publish(steering_msg);


}

int main(int argc, char **argv)
{  
    
    rclcpp::init(argc,argv);    
    auto node = std::make_shared<inputs_publisher_class>();
    rclcpp::Rate rate(200); //5ms

    RCLCPP_INFO(node->get_logger(),"[Inputs] Inputs Publisher Node Initialized...");
    double wheel_angular_value=0.0;
    double pedal_left_value=0.0;
    double pedal_right_value=0.0;

    while(rclcpp::ok())
    { 
        buttonsWheel();
        pedal_left_value = pedalLeftVal();
        pedal_right_value = pedalRightVal();
        wheel_angular_value = wheelAngularValue();

        node->PublishData(pedal_left_value, pedal_right_value, wheel_angular_value);

        RCLCPP_INFO(node->get_logger(),"/////////////////////////////");
        RCLCPP_INFO(node->get_logger(),"LPedal: %f", pedal_left_value);
        RCLCPP_INFO(node->get_logger(),"RPedal: %f", pedal_right_value);
        RCLCPP_INFO(node->get_logger(),"Steering: %f", wheel_angular_value);
          
        rclcpp::spin_some(node);
	    rate.sleep();
    }
    
    RCLCPP_INFO(node->get_logger(),"[InputsPublisher] Exitting...");
    rclcpp::shutdown();
}