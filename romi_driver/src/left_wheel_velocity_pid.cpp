#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <iostream>

float desired_ang_vel_ = 0;

void commandVelCB(const std_msgs::Float32::ConstPtr& msg)
{
  std::cout << "Angular velocity: " << msg->data << " Rad/s" << std::endl;
}

void pidVelCB(const std_msgs::Float32::ConstPtr& msg){
  desired_ang_vel_ = msg->data;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pid_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/left_wheel_vel", 1000, commandVelCB);
  ros::Subscriber pid_sub = n.subscribe("/left_wheel_pid", 1000, pidVelCB);
  ros::spin();

  return 0;
}
