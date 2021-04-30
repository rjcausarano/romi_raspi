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

add_executable(velocity src/velocity.cpp)
target_link_libraries(
  velocity 
  ${catkin_LIBRARIES}
  wiringPi
  rt
)
add_dependencies(velocity romi_driver_generate_messages_cpp)

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
add_dependencies(listener romi_driver_generate_messages_cpp)
