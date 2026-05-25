#pragma once
#include "peripheral.h"
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


#define MAX_STEERING (M_PI/2 * 0.9999999)


void loopButtonsWheel(){   
    static int fd=-1;
    struct input_event ev1;
    struct input_event ev2;
    ssize_t bytesRead1;
    ssize_t bytesRead2;
    
    if(fd==-1)
        fd = open(getEvent(DEVICE_STEERING_WHEEL).c_str(), O_RDONLY);

    
            if(true){
                uint ts;
                bytesRead1 = read(fd, &ev1, sizeof(ev1));
                bytesRead2 = read(fd, &ev2, sizeof(ev2));

                if (bytesRead1 == sizeof(ev1)) {
                    //ts=getCurrentTime()*2;

                    //std::cout<<"READING 1 "<<std::endl;
                    //std::cout<<"ev.type "<<ev.type<<std::endl;
                    //std::cout<<"ev.code "<<ev.code<<std::endl;
                    //std::cout<<"ev.value "<<ev.value<<std::endl;
                    //std::cout<<"steeringWheelAngular.value "<<steeringWheelAngular.value<<std::endl;
                    //std::cout<<"angular z "<<interfaceSystem->stacker->robotDynamic.angular.z<<std::endl;

                    nlohmann::json jsonObject;
                    jsonObject["type"] = ev1.type;
                    jsonObject["code"] = ev1.code;
                    jsonObject["value"] = ev1.value;
                    jsonObject["name"] = DEVICE_STEERING_WHEEL;
                    jsonObject["ts"] = ts;
                    std::string jsonString = jsonObject.dump();
                    //std::cout<<jsonString<<std::endl;
                    
                    //pubMQTT(config.remotePublicIP, PORT, "/input_peripheral", jsonString.c_str());

                    if(steeringWheelAngular.update(ev1)) return;
                    if(pedalLinearLeft.update(ev1)) return;
                    if(pedalLinearRight.update(ev1)) return;

                    if(steeringWheelArrowVertical.update(ev1)) return;
                    if(steeringWheelArrowHorizontal.update(ev1)) return;

                    if(steeringWheelButton1.update(ev1)) return;
                    if(steeringWheelButton2.update(ev1)) return;
                    if(steeringWheelButton3.update(ev1)) return;
                    if(steeringWheelPaddelLeft.update(ev1)) return;
                    if(steeringWheelPaddelRight.update(ev1)) return;

                    if(steeringWheelEnter.update(ev1)) return;
                    if(steeringWheelR3.update(ev1)) return;

                }
                

                if (bytesRead2 == sizeof(ev2)) {
                    //ts=getCurrentTime()*2+1;

                    //std::cout<<"READING 2 "<<std::endl;
                    //std::cout<<"ev.type "<<ev.type<<std::endl;
                    //std::cout<<"ev.code "<<ev.code<<std::endl;
                    //std::cout<<"ev.value "<<ev.value<<std::endl;
                    //std::cout<<"steeringWheelAngular.value "<<steeringWheelAngular.value<<std::endl;
                    //std::cout<<"angular z "<<interfaceSystem->stacker->robotDynamic.angular.z<<std::endl;

                    nlohmann::json jsonObject;
                    jsonObject["type"] = ev2.type;
                    jsonObject["code"] = ev2.code;
                    jsonObject["value"] = ev2.value;
                    jsonObject["name"] = DEVICE_STEERING_WHEEL;
                    jsonObject["ts"] = ts;
                    std::string jsonString = jsonObject.dump();
                    //std::cout<<jsonString<<std::endl;

                   // pubMQTT(config.remotePublicIP, PORT, "/input_peripheral", jsonString.c_str());
                    
                    if(steeringWheelAngular.update(ev2)) return;
                    if(pedalLinearLeft.update(ev2)) return;
                    if(pedalLinearRight.update(ev2)) return;

                    if(steeringWheelArrowVertical.update(ev2)) return;
                    if(steeringWheelArrowHorizontal.update(ev2)) return;

                    if(steeringWheelButton1.update(ev2)) return;
                    if(steeringWheelButton2.update(ev2)) return;
                    if(steeringWheelButton3.update(ev2)) return;
                    if(steeringWheelPaddelLeft.update(ev2)) return;
                    if(steeringWheelPaddelRight.update(ev2)) return;

                    if(steeringWheelEnter.update(ev2)) return;
                    if(steeringWheelR3.update(ev2)) return;
                    
                }

            }
}


void buttonsWheel(){
    
    struct input_event ev;
    ssize_t bytesRead;

    
    loopButtonsWheel();

    
    static int lastSteeringWheelButton1=0;
    static int lastSteeringWheelButton2=0;
    static int lastSteeringWheelButton3=0;
    //std::cout<<"buttons wheel"<<std::endl;
    

}

double pedalLeftVal(){return pedalLinearLeft.value;}
double pedalRightVal(){return pedalLinearRight.value;}
double wheelAngularValue(){return steeringWheelAngular.value;}

