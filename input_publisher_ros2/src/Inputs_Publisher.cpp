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
    rclcpp::Rate rate(400); //1ms

    RCLCPP_INFO(node->get_logger(),"[Inputs] Inputs Publisher Node Initialized...");
    double wheel_angular_value=0.0;
    double pedal_left_value=0.0;
    double pedal_right_value=0.0;

    // buttonsWheel(); //read buttons and update values
    std::thread threadWheelButtons([&]() {threadFunctionWheelButtons();});
	threadWheelButtons.detach();

    while(rclcpp::ok())
    { 
       

        pedal_right_value = pedalRightVal(); //Right Side Pedal
        pedal_left_value = pedalLeftVal(); //Left Side Pedal
        wheel_angular_value = wheelAngularValue();  //Steering Wheel value

        node->PublishData(pedal_right_value, pedal_left_value, wheel_angular_value); //Publish Values

        
        //clear terminal
        //system("clear");

        //Print values to terminal
        RCLCPP_INFO(node->get_logger(),"/////////////////////////////");
        RCLCPP_INFO(node->get_logger(),"Left Pedal -> Backwards : %f", pedal_left_value);
        RCLCPP_INFO(node->get_logger(),"Right Pedal -> Throttle : %f", pedal_right_value);
        RCLCPP_INFO(node->get_logger(),"Steering : %f", wheel_angular_value);
        
        
        rclcpp::spin_some(node);
	    rate.sleep();
    }
    
    RCLCPP_INFO(node->get_logger(),"[InputsPublisher] Exitting...");
    rclcpp::shutdown();
}