#!/bin/bash
 
# Source ROS and Catkin workspaces
source /opt/ros/noetic/setup.bash
if [ -f /ros/devel/setup.bash ]
then
  source /ros/devel/setup.bash
fi
if [ -f /ros/devel/setup.bash ]
then
  source /ros/devel/setup.bash
fi
echo "Sourced Catkin workspace!"
 
# Set environment variables
#export TURTLEBOT3_MODEL=waffle_pi
#export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:$(rospack find tb3_worlds)/models
 
# Execute the command passed into this entrypoint
exec "$@"