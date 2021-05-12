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

add_executable(read_wheel_velocities src/read_wheel_velocities.cpp)
target_link_libraries(
  read_wheel_velocities
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(wheel_velocities_pid src/wheel_velocities_pid.cpp)
target_link_libraries(
  wheel_velocities_pid
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(wheel_velocities src/wheel_velocities.cpp)
target_link_libraries(
  wheel_velocities
  ${catkin_LIBRARIES}
)