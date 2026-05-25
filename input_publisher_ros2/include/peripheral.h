#include <cstdint>
#include <string>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <iostream>

	//device events
	#define DEVICE_JOYSTICK "Mad Catz Saitek Pro Flight X-56 Rhino Stick"
	#define DEVICE_STEERING_WHEEL2 "Fanatec FANATEC Wheel"
	#define DEVICE_PEDALS "Fanatec FANATEC Wheel"
	#define DEVICE_HEADPHONES "HDA Intel PCH Headphone Mic"
	#define DEVICE_MOUSE "LIDL Deskset LIDL Deskset Mouse"
	#define DEVICE_STEERING_WHEEL "Logitech G29 Driving Force Racing Wheel"
	#define DEVICE_GAMEPAD "Logitech Logitech Cordless RumblePad 2"

//stores info from a single degree of freedom of a peripheral
class inputPeripheral{
	public:
	uint16_t type;		//type of input
	uint16_t code;		//ID of the input inside the peripheral
	uint32_t value;		//value inputed
	std::string name;	//name attributed to the input for easy instantiation

	//definition of the input with initial value
	inputPeripheral(uint16_t type, uint16_t code, uint32_t value, std::string name="NULL"){
		this->type=type;
		this->code=code;
		this->value=value;
		this->name=name;
	}

	//update of the input value
	bool update(struct input_event ev){

        //std::cout<<"---------------------- "<<std::endl;
        //std::cout<<"ev.type "<<ev.type<<std::endl;
        //std::cout<<"ev.code "<<ev.code<<std::endl;

        //std::cout<<"type "<<type<<std::endl;
        //std::cout<<"code "<<code<<std::endl;
        //std::cout<<"---------------------- "<<std::endl;
		if(type==ev.type&&code==ev.code){
			value=ev.value;
			return true;
		}
		return false;
	}

	void update(uint32_t value){
		this->value=value;
	}

	//debug only
	void print(){
		//std::cout << "Name=" << name << "   Value=" << value << std::endl;
	}

};


std::string getEvent(std::string deviceName){
    std::string eventName;
    for(int i=0;i<30;i++){
        eventName="/dev/input/event"+std::to_string(i);
        int fd;
        fd = open(eventName.c_str(), O_RDONLY);
        if (fd != -1) {
            char name[256] = "Unknown";
    		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

            std::string nameStr(name);
            //std::cout << "nameStr " << nameStr << std::endl;
            close(fd);
            if(nameStr==deviceName){
                //std::cout << "found " << std::endl;
                return eventName;
            }
        }
    }
    return "";
}





//input steering wheel
class inputPeripheral steeringWheelAngular(3, 0, 32767, "steeringWheelAngular");
class inputPeripheral pedalLinearLeft(3, 1, 255, "pedalLinearLeft");
class inputPeripheral pedalLinearRight(3, 2, 255, "pedalLinearRight");

//input steering wheel
class inputPeripheral steeringWheelArrowVertical(3, 17, 0, "steeringWheelArrowVertical");
class inputPeripheral steeringWheelArrowHorizontal(3, 16, 0, "steeringWheelArrowHorizontal");

class inputPeripheral steeringWheelButton1(1, 296, 0, "steeringWheelButton1");
class inputPeripheral steeringWheelButton2(1, 297, 0, "steeringWheelButton2");
class inputPeripheral steeringWheelButton3(1, 712, 0, "steeringWheelButton3");
class inputPeripheral steeringWheelPaddelLeft(1, 293, 0, "steeringWheelPaddelLeft");
class inputPeripheral steeringWheelPaddelRight(1, 292, 0, "steeringWheelPaddelRight");


class inputPeripheral steeringWheelCross(1, 288, 0, "steeringWheelCross");//cross
class inputPeripheral steeringWheelSquare(1, 289, 0, "steeringWheelSquare");//square
class inputPeripheral steeringWheelCircle(1, 290, 0, "steeringWheelCircle");//circle
class inputPeripheral steeringWheelTriangle(1, 291, 0, "steeringWheelTriangle");//triangle


class inputPeripheral steeringWheelPlus(1, 707, 0, "steeringWheelPlus");//+
class inputPeripheral steeringWheelMinus(1, 708, 0, "steeringWheelMinus");//-

class inputPeripheral steeringWheelEnter(1, 711, 0, "steeringWheelEnter");//enter
class inputPeripheral steeringWheelR3(1, 298, 0, "steeringWheelR3");//R3


//input joystick
class inputPeripheral joystickUpDown(3, 1, 32767, "joystickUpDown");
class inputPeripheral joystickHorizontal(3, 0, 32767, "joystickHorizontal");
class inputPeripheral joystickInclination(3, 5, 2047, "joystickInclination");

//input gamepad
class inputPeripheral gamepadLinear(3, 1, 128, "gamepadLinear");
class inputPeripheral gamepadAngular(3, 2, 128, "gamepadAngular");
