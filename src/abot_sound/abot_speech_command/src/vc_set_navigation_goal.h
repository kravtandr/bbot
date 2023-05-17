#ifndef VC_SET_NAVIGATION_GOAL_H_
#define VC_SET_NAVIGATION_GOAL_H_

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <actionlib/client/simple_action_client.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class VCSetNavigationGoal {
public:
	VCSetNavigationGoal();
private:
	ros::NodeHandle _node;
	ros::Subscriber _stt_sub;
	ros::Publisher _tts_pub;

	void grammarCallback(const std_msgs::String::ConstPtr& msg);
};
std::vector<std::string> read_record(std::string idstr);

#endif // SET_NAVIGATION_GOAL_H_
