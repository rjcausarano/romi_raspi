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

add_executable(read_velocity src/read_velocity.cpp)
target_link_libraries(
  read_velocity 
  ${catkin_LIBRARIES}
  wiringPi
  rt
)

add_executable(write_velocity src/write_velocity.cpp)
target_link_libraries(
  write_velocity 
  ${catkin_LIBRARIES}
)