cmake_minimum_required(VERSION 3.0.2)
project(bbot_teleop)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  joy
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(bbot_teleop src/bbot_teleop.cpp)
target_link_libraries(bbot_teleop ${catkin_LIBRARIES})
