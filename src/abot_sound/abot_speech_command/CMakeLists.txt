cmake_minimum_required(VERSION 3.0.2)
project(abot_speech_command)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  actionlib
  move_base_msgs
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(vc_set_navigation_goal src/vc_set_navigation_goal.cpp)
target_link_libraries(vc_set_navigation_goal ${catkin_LIBRARIES})
