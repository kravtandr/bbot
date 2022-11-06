
#ifndef DC_MOTOR_WIRING_PI_H_
#define DC_MOTOR_WIRING_PI_H_

#include <ros/ros.h>
#include <wiringPi.h>
#include <softPwm.h>
//#include <GpioExpanderPi.h>
constexpr uint16_t RPI_MAX_PWM_VALUE = 1023;

class DCMotorWiringPi {
public:
	DCMotorWiringPi(int8_t direction_pin, int8_t enable_pin);
	void cw(uint16_t val);
	void ccw(uint16_t val);
	void stop();

private:
	//GpioExpanderPi* _expander;
	int8_t _direction_pin;
	int8_t _enable_pin;
	uint16_t protectOutput(uint16_t val);
};

DCMotorWiringPi::DCMotorWiringPi(int8_t direction_pin, int8_t enable_pin) {
	_direction_pin = direction_pin;
	_enable_pin = enable_pin;
	//GpioExpanderPi expander;
	//_expander = &expander;
	//GpioExpanderPi expander;
	if (wiringPiSetupGpio() < 0) {
		throw std::runtime_error("DCMotor wiringPi error: GPIO setup error");
	}
	//if(!_expander->begin()){
	//	ROS_ERROR("I2C setup error");
	//	throw std::runtime_error("i2c setup error ");
	//}
	ROS_INFO("DCMotor wiringPi: i2c setup");
	pinMode(_direction_pin, OUTPUT);
	//pinMode(_enable_pin,PWM_OUTPUT);
	softPwmCreate(_enable_pin, 0, 1023);
	stop();
	ROS_INFO("DCMotor wiringPi: Motor setup");
}

void DCMotorWiringPi::stop() {
	softPwmWrite(_enable_pin, 0);
	//_expander->analogWrite(_enable_pin, 0);
	digitalWrite(_direction_pin, 0);
}

void DCMotorWiringPi::cw(uint16_t val) {
	softPwmWrite(_enable_pin, protectOutput(val));
	//_expander->analogWrite(_enable_pin, protectOutput(val));
	digitalWrite(_direction_pin, 1);
}

void DCMotorWiringPi::ccw(uint16_t val) {
	softPwmWrite(_enable_pin, protectOutput(val));
	//analogWrite(_enable_pin, protectOutput(val));
	digitalWrite(_direction_pin, 0);
}

uint16_t DCMotorWiringPi::protectOutput(uint16_t val) {
	return val > RPI_MAX_PWM_VALUE ? RPI_MAX_PWM_VALUE : val;
}

#endif // DC_MOTOR_WIRING_PI_H_
