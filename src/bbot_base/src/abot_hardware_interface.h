
#ifndef bbot_HARDWARE_INTERFACE_HPP_
#define bbot_HARDWARE_INTERFACE_HPP_

#include <boost/assign/list_of.hpp>
#include <sstream>
#include <std_msgs/Float64.h>
#include <std_srvs/Empty.h>

#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <ros/console.h>
#include <ros/ros.h>

class bbotHardwareInterface : public hardware_interface::RobotHW {
public:
	bbotHardwareInterface(ros::NodeHandle node, ros::NodeHandle private_node, double target_max_wheel_angular_speed);

	void updateJointsFromHardware(const ros::Duration& period);
	void writeCommandsToHardware();

private:
	ros::NodeHandle _node;
	ros::NodeHandle _private_node;

	hardware_interface::JointStateInterface _joint_state_interface;
	hardware_interface::VelocityJointInterface _velocity_joint_interface;

	ros::Subscriber _left_wheel_angle_sub;
	ros::Subscriber _right_wheel_angle_sub;
	ros::Publisher _left_wheel_vel_pub;
	ros::Publisher _right_wheel_vel_pub;

	struct Joint {
		double position;
		double position_offset;
		double velocity;
		double effort;
		double velocity_command;

		Joint()
			: position(0)
			, velocity(0)
			, effort(0)
			, velocity_command(0) { }
	} _joints[2];

	double _left_wheel_angle;
	double _right_wheel_angle;
	double _max_wheel_angular_speed;

	void registerControlInterfaces();
	void leftWheelAngleCallback(const std_msgs::Float64& msg);
	void rightWheelAngleCallback(const std_msgs::Float64& msg);
	void limitDifferentialSpeed(double& diff_speed_left_side, double& diff_speed_right_side);
};

bbotHardwareInterface::bbotHardwareInterface(ros::NodeHandle node, ros::NodeHandle private_node, double target_max_wheel_angular_speed)
	: _node(node)
	, _private_node(private_node)
	, _max_wheel_angular_speed(target_max_wheel_angular_speed) {
	registerControlInterfaces();

	_left_wheel_vel_pub = _node.advertise<std_msgs::Float64>("/bbot/left_wheel/target_velocity", 1);
	_right_wheel_vel_pub = _node.advertise<std_msgs::Float64>("/bbot/right_wheel/target_velocity", 1);
	_left_wheel_angle_sub = _node.subscribe("bbot/left_wheel/angle", 1, &bbotHardwareInterface::leftWheelAngleCallback, this);
	_right_wheel_angle_sub = _node.subscribe("bbot/right_wheel/angle", 1, &bbotHardwareInterface::rightWheelAngleCallback, this);
}

void bbotHardwareInterface::writeCommandsToHardware() {
	double diff_angle_speed_left = _joints[0].velocity_command;
	double diff_angle_speed_right = _joints[1].velocity_command;

	limitDifferentialSpeed(diff_angle_speed_left, diff_angle_speed_right);

	std_msgs::Float64 left_wheel_vel_msg;
	std_msgs::Float64 right_wheel_vel_msg;

	left_wheel_vel_msg.data = diff_angle_speed_left;
	right_wheel_vel_msg.data = diff_angle_speed_right;

	_left_wheel_vel_pub.publish(left_wheel_vel_msg);
	_right_wheel_vel_pub.publish(right_wheel_vel_msg);
}

void bbotHardwareInterface::updateJointsFromHardware(const ros::Duration& period) {
	double delta_left_wheel = _left_wheel_angle - _joints[0].position - _joints[0].position_offset;
	double delta_right_wheel = _right_wheel_angle - _joints[1].position - _joints[1].position_offset;

	if (std::abs(delta_left_wheel) < 1) {
		_joints[0].position += delta_left_wheel;
		_joints[0].velocity = delta_left_wheel / period.toSec();
	} else {
		_joints[0].position_offset += delta_left_wheel;
	}

	if (std::abs(delta_right_wheel) < 1) {
		_joints[1].position += delta_right_wheel;
		_joints[1].velocity = delta_right_wheel / period.toSec();
	} else {
		_joints[1].position_offset += delta_right_wheel;
	}
}

void bbotHardwareInterface::registerControlInterfaces() {
	ros::V_string joint_names = boost::assign::list_of("left_wheel_to_base")("right_wheel_to_base");

	for (unsigned int i = 0; i < joint_names.size(); i++) {
		hardware_interface::JointStateHandle joint_state_handle(joint_names[i], &_joints[i].position, &_joints[i].velocity, &_joints[i].effort);
		_joint_state_interface.registerHandle(joint_state_handle);

		hardware_interface::JointHandle joint_handle(joint_state_handle, &_joints[i].velocity_command);
		_velocity_joint_interface.registerHandle(joint_handle);
	}
	registerInterface(&_joint_state_interface);
	registerInterface(&_velocity_joint_interface);
}

void bbotHardwareInterface::leftWheelAngleCallback(const std_msgs::Float64& msg) {
	_left_wheel_angle = msg.data;
}

void bbotHardwareInterface::rightWheelAngleCallback(const std_msgs::Float64& msg) {
	_right_wheel_angle = msg.data;
}

void bbotHardwareInterface::limitDifferentialSpeed(double& diff_speed_left_side, double& diff_speed_right_side) {
	double large_speed = std::max(std::abs(diff_speed_left_side), std::abs(diff_speed_right_side));
	if (large_speed > _max_wheel_angular_speed) {
		diff_speed_left_side *= _max_wheel_angular_speed / large_speed;
		diff_speed_right_side *= _max_wheel_angular_speed / large_speed;
	}
}

#endif // bbot_HARDWARE_INTERFACE_HPP_