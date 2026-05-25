#include <Wheel_Control.hpp>
#include <time.h>
#include <ctime>

wheel_control_class::wheel_control_class()
: Node("WheelControl")
{

 //SUBSCRIBERS
    //Create Subscribers
        //Receive Forwards Backwards input
        ThrottleSub = this->create_subscription<std_msgs::msg::Float32>("wheel_throttle_topic", 1, std::bind(&wheel_control_class::Throttle_Callback, this, _1)); 
        BrakeSub = this->create_subscription<std_msgs::msg::Float32>("wheel_brake_topic", 1, std::bind(&wheel_control_class::Brake_Callback, this, _1));
        //Receive Left and Right inputs
        SteeringSub = this->create_subscription<std_msgs::msg::Float32>("wheel_steering_topic", 1, std::bind(&wheel_control_class::Steering_Callback, this, _1));
        //Receive speed calculated by ACC
        ACCSub = this->create_subscription<geometry_msgs::msg::Twist>("acc_vel", 1, std::bind(&wheel_control_class::ACC_Callback, this, _1));
        //Receive Emergency Brake from Joystick
        StopSub = this->create_subscription<std_msgs::msg::Bool>("joystick_stop_topic", 1, std::bind(&wheel_control_class::Stop_Callback, this, _1));
        
        EnableACCSub = this->create_subscription<std_msgs::msg::Bool>("enable_acc_topic", 1, std::bind(&wheel_control_class::Enable_ACC_Callback, this, _1));
        DeactivateACCSub = this->create_subscription<std_msgs::msg::Bool>("deactivate_acc_topic", 1, std::bind(&wheel_control_class::Deactivate_ACC_Callback, this, _1));

//PUBLISHER
        //Vel PUblisher
        VelPublisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel",1);
        
        EnablePublisher = this->create_publisher<std_msgs::msg::Bool>("enableACC",1);

    //Initialize velocity message
        vel = geometry_msgs::msg::Twist();

//SERVICE
    //Create Client
        _client = this->create_client<std_srvs::srv::SetBool>("enable_acc_service");

//INITS
    //Flags
        start_moving = false;
        robot_state = robot_stop;
        enable_acc=false;
        received_acc_vel=false;
        received_enable_command=false;
        value_stop=false;
        received_acc_vel=false;
        stop=false;
        send_request_flag=false;

    //Initialize auxiliary variables
        steering_angle = 0;
        linear_velocity = 0;
        angular_velocity = 0;
        value_throttle=0;
        value_steering=0;
        last_linear_velocity=0;
    
  
}
wheel_control_class::~wheel_control_class()
{

}

void wheel_control_class::Throttle_Control(){

    if (!enable_acc) //If Manual Control is ON
    {
        float value=value_throttle;
        robot_state = FORWARD;
        if (value == MAX_THROTTLE_VAL){
             //valor maximo corresponde à posição default;
            robot_state = robot_stop;
        } 
      
        float aux_val=MAX_THROTTLE_VAL - value;
        float percent = (aux_val) / MAX_THROTTLE_VAL; //calculate input percentage; 32767 is the max value
        linear_velocity = percent * MAX_LINEAR_SPEED;  //calculate speed from input percentage;

        if (abs(linear_velocity)<0.008) linear_velocity=0; //filter noise
        
        
    }else //ACC mode is ON
    {
        linear_velocity=vel_from_acc; //Speed corresponds to ACC calculated speed
        RCLCPP_INFO(this->get_logger(),"LinearVelocity (ACC): %f", linear_velocity);
        
    }
    
}

void wheel_control_class::Brake_Control(){
    float value;
    if (!enable_acc) //If Manual Control is ON
    {
        value=value_brake;
        robot_state = BACKWARD;

        if (value == MAX_THROTTLE_VAL){
            value=0; //valor maximo corresponde a posição default;
            robot_state = robot_stop;
            return;
        } 
     
        float aux_val=MAX_THROTTLE_VAL - value;
        float percent = (aux_val) / MAX_THROTTLE_VAL; //calculate input percentage; 32767 is the max value
        linear_velocity = percent * MAX_LINEAR_SPEED * -1;  //calculate speed from input percentage;


        if (abs(linear_velocity)<0.008) linear_velocity=0; //filter noise
        
        
        
    }else //ACC mode is ON
    {
        if (value <(MAX_BRAKE_VAL-1)) {  //If brake is activated
            enable_acc=false;
            received_enable_command=true;
            send_request_flag=true;
        }
    }
    
}


void wheel_control_class::Steering_Control(){
    float value=value_steering;
    int i=1;

    if (value == MID_STEERING_VAL) angular_velocity=0; //valor 128 corresponde ao 0 no axis do joystick;
    if (value > MID_STEERING_VAL){
        i=-1;
        value-= MID_STEERING_VAL; //calcular a percentagem de giro para direita, subtraindo o valor recebido do valor central (32767)
    }
    else if(value< MID_STEERING_VAL){
        value = MID_STEERING_VAL - value; //calcular a percentagem de giro para esquerda, subtraindo ao valor central (32767) o valor recebido
    }
    float percent = (value * 1) / MID_STEERING_VAL; //calculate steering percentage; 32767 is the middle value
    float steering_angle =  percent * MAX_STEERING_ANGLE * i;

    angular_velocity = tan(steering_angle) * linear_velocity / WHEEL_BASE; //calculate angular velocity from steering angle and linear velocity

    //if (robot_state==BACKWARD) angular_velocity *= -1;

    if (abs(angular_velocity)<0.008) angular_velocity=0;
    RCLCPP_INFO(this->get_logger(),"SteeringAngle: %f", steering_angle);
    RCLCPP_INFO(this->get_logger(),"LinearVelocity: %f", linear_velocity);
    RCLCPP_INFO(this->get_logger(),"AngularVelocity: %f", angular_velocity);
}


void wheel_control_class::Stop_Control(){
    if(value_stop==true) //if emergency stop is received
    {
        stop=true;
        
        if(enable_acc){
            received_enable_command=true;
            enable_acc=false;
            send_request_flag=true;
        }
    }else
    {
        stop=false;
    }
}

void wheel_control_class::PublishData()
{
   
    vel.linear.x=linear_velocity;
    vel.angular.z=angular_velocity;
   
    if(stop)
    {
        vel.linear.x=0;
        vel.angular.z=0;
        linear_velocity=0;
    }
    if(received_enable_command)
    {
        RCLCPP_INFO(this->get_logger(),"ACC STATUS: %d", enable_acc);
        received_enable_command=false;
        enable_msg.data=enable_acc;
      
        //SendRequest(enable_acc);
        //EnablePublisher->publish(enable_msg);
    }
    
    VelPublisher->publish(vel); //publish cmd_vel
    
    last_linear_velocity=linear_velocity;
    
    
}


void wheel_control_class::Throttle_Callback(const std_msgs::msg::Float32::ConstPtr& throttle_received)
{
    value_throttle=throttle_received->data;
    //RCLCPP_INFO(this->get_logger(),"throttle_value : %f ",throttle_received->data);
}
void wheel_control_class::Brake_Callback(const std_msgs::msg::Float32::ConstPtr& brake_received)
{
    value_brake=brake_received->data;
    //RCLCPP_INFO(this->get_logger(),"brake_value : %f ",brake_received->data);
}
void wheel_control_class::Steering_Callback(const std_msgs::msg::Float32::ConstPtr& steering_received)
{
    value_steering=steering_received->data;
    //RCLCPP_INFO(this->get_logger(),"steering_value : %f ",steering_received->data);
}
void wheel_control_class::Stop_Callback(const std_msgs::msg::Bool::ConstPtr& stop_received)
{
    value_stop=stop_received->data;
}
void wheel_control_class::ACC_Callback(const geometry_msgs::msg::Twist::ConstPtr& vel_received)
{
    vel_from_acc=vel_received->linear.x;
    received_acc_vel=true;
}

void wheel_control_class::Enable_ACC_Callback(const std_msgs::msg::Bool::ConstPtr& enable_received)
{
    bool received = enable_received->data;
    if(linear_velocity > 0.08 ){ //Cruise Control não ativa com marcha-atrás nem parado
        enable_acc = received;
        received_enable_command=true;
        send_request_flag=true;
    }
    
}
void wheel_control_class::Deactivate_ACC_Callback(const std_msgs::msg::Bool::ConstPtr& msg_received)
{

    enable_acc=msg_received->data;
    RCLCPP_INFO(this->get_logger(),"ACC DEACTIVATED DUE TO LOW SPEED");
    //received_enable_command=true;
    //send_request_flag=true; //no need to send request for acc because the command itselfs comes from that same node
}

int wheel_control_class::SendRequest(bool command)
{
    //reset flag
    send_request_flag=false;

    //prepare request
    auto request = std::make_shared<std_srvs::srv::SetBool::Request>();
    request->data=command;

    while (!_client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
            enable_acc=0;
            return 0;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Failed to activate ACC! (service not available)");
            enable_acc=0;
            return 0;
    }
    
    auto result = _client->async_send_request(request);

    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(),result) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Response: %ld", result.get()->success);
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service enable_acc_service");
    }

 return 1;
}


int main(int argc, char **argv)
{  
    rclcpp::init(argc,argv);    //ROS2
    auto node = std::make_shared<wheel_control_class>(); //ROS2
    rclcpp::Rate rate(20);
    RCLCPP_INFO(node->get_logger(),"[Manual Control Active]");

    while(rclcpp::ok())
    { 
        
        RCLCPP_INFO(node->get_logger(),"--------COMMANDS--------");
        node->Stop_Control();
        node->Throttle_Control();
        node->Brake_Control();
        node->Steering_Control();
        node->PublishData();
        
        if(node->send_request_flag){
            node->SendRequest(node->enable_acc);
        }
        
        rclcpp::spin_some(node);
	    rate.sleep();
        
    }
    
    rclcpp::shutdown();

}
