cmake_minimum_required(VERSION 3.1.0)
project(romi_driver)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  genmsg
)

## Declare ROS messages and services
add_message_files(FILES Num.msg)
add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package(

)

## Build
include_directories(
  include 
  ${catkin_INCLUDE_DIRS}
)

add_executable(read_velocity src/read_velocity.cpp)
target_link_libraries(
  read_velocity 
  ${catkin_LIBRARIES}
  wiringPi
  rt
)
add_dependencies(read_velocity romi_driver_generate_messages_cpp)

add_executable(write_velocity src/write_velocity.cpp)
target_link_libraries(write_velocity ${catkin_LIBRARIES})
add_dependencies(write_velocity romi_driver_generate_messages_cpp)
