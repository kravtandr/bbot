
#include "dc_motor_wiring_pi.h"
//#include <GpioExpanderPi.h>
#include <std_msgs/Float64.h>

#define MOTOR_1_PIN_D 4 // Wiring pi 7 = BCM 4
#define MOTOR_1_PIN_E 5 // Wiring pi 1 = BCM 18
#define MOTOR_2_PIN_D 16 // Wiring pi 26 = BCM  12
#define MOTOR_2_PIN_E 6 // Wiring pi 27 = BCM 16
//constexpr  uint8_t EXPANDER_1_PIN_D  = 7;
//constexpr  uint8_t EXPANDER_1_PIN_E  = 6;
//constexpr  uint8_t EXPANDER_2_PIN_D  = 5;
//constexpr  uint8_t EXPANDER_2_PIN_E  = 4;


DCMotorWiringPi left_dc_motor(MOTOR_1_PIN_D, MOTOR_1_PIN_E);
DCMotorWiringPi right_dc_motor(MOTOR_2_PIN_D, MOTOR_2_PIN_E);

void leftMotorCallback(const std_msgs::Float64& msg) {
	int16_t pwm = msg.data * 100;
	ROS_INFO("left %i", pwm);
	if (pwm > 0) {
		left_dc_motor.ccw(abs(pwm));
	} else if (pwm < 0) {
		left_dc_motor.cw(abs(pwm));
	} else if (pwm == 0) {
		left_dc_motor.stop();
	}
}

void rightMotorCallback(const std_msgs::Float64& msg) {
	int16_t pwm = msg.data * 100;
	ROS_INFO("right %i", pwm);
	if (pwm > 0) {
		right_dc_motor.ccw(abs(pwm));
	} else if (pwm < 0) {
		right_dc_motor.cw(abs(pwm));
	} else if (pwm == 0) {
		right_dc_motor.stop();
	}
}

int main(int argc, char** argv) {
	//GpioExpanderPi expander;
	//if (!expander.begin()){
	///	ROS_ERROR("I2C failed in dc_motors.cpp");
	///	return -1;
	//}
	//DCMotorWiringPi left_dc_motor(expander, EXPANDER_1_PIN_D, EXPANDER_1_PIN_E);
	//DCMotorWiringPi right_dc_motor(expander,EXPANDER_2_PIN_D, EXPANDER_2_PIN_E);
	ros::init(argc, argv, "dc_motors");
	ros::NodeHandle node;
	ros::Subscriber left_motor_target_vel_sub = node.subscribe("/abot/left_wheel/pwm", 1, &leftMotorCallback);
	ros::Subscriber right_motor_target_vel_sub = node.subscribe("/abot/right_wheel/pwm", 1, &rightMotorCallback);
	ros::spin();
	return 0;
}
