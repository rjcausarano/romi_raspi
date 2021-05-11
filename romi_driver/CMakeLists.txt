cmake_minimum_required(VERSION 3.1.0)
project(romi_driver)

## Packages that this one depends on
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

## Packages that depend on this one
catkin_package(

)

## Build
include_directories(
  include 
  ${catkin_INCLUDE_DIRS}
)

add_executable(read_left_wheel_velocity src/read_left_wheel_velocity.cpp)
target_link_libraries(
  read_left_wheel_velocity 
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(write_left_wheel_velocity src/write_left_wheel_velocity.cpp)
target_link_libraries(
  write_left_wheel_velocity 
  ${catkin_LIBRARIES}
)

add_executable(left_wheel_velocity_pid src/left_wheel_velocity_pid.cpp)
target_link_libraries(
  left_wheel_velocity_pid
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(read_right_wheel_velocity src/read_right_wheel_velocity.cpp)
target_link_libraries(
  read_right_wheel_velocity 
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(write_right_wheel_velocity src/write_right_wheel_velocity.cpp)
target_link_libraries(
  write_right_wheel_velocity 
  ${catkin_LIBRARIES}
)

add_executable(right_wheel_velocity_pid src/right_wheel_velocity_pid.cpp)
target_link_libraries(
  right_wheel_velocity_pid
  ${catkin_LIBRARIES}
  wiringPi
  rt
)