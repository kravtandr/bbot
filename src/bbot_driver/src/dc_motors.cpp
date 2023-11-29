
#include "dc_motor_wiring_pi.h"
#include <std_msgs/Float64.h>

#define MOTOR_1_PIN_D 4 // Wiring pi 7 = BCM 4
#define MOTOR_1_PIN_E 5 // Wiring pi 21 = BCM 5
#define MOTOR_2_PIN_D 12 // Wiring pi 26 = BCM  12
#define MOTOR_2_PIN_E 6 // Wiring pi 22 = BCM 6


DCMotorWiringPi right_dc_motor(MOTOR_1_PIN_D, MOTOR_1_PIN_E);
DCMotorWiringPi left_dc_motor(MOTOR_2_PIN_D, MOTOR_2_PIN_E);

void leftMotorCallback(const std_msgs::Float64& msg) {
	int16_t pwm = msg.data * 100;
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
	if (pwm > 0) {
		right_dc_motor.ccw(abs(pwm));
	} else if (pwm < 0) {
		right_dc_motor.cw(abs(pwm));
	} else if (pwm == 0) {
		right_dc_motor.stop();
	}
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "dc_motors");
	ros::NodeHandle node;
	ros::Subscriber left_motor_target_vel_sub = node.subscribe("/bbot/left_wheel/pwm", 1, &leftMotorCallback);
	ros::Subscriber right_motor_target_vel_sub = node.subscribe("/bbot/right_wheel/pwm", 1, &rightMotorCallback);
	ros::spin();
	return 0;
}
