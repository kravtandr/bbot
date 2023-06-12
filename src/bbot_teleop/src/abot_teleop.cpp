#include "bbot_teleop.h"

int main(int argc, char** argv) {
	ros::init(argc, argv, "bbot_teleop");
	ros::NodeHandle private_node("~");
	bbotTeleop bbotTeleop(private_node);
	ros::spin();
}