#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>

void commandVelCB(const geometry_msgs::Twist::ConstPtr& msg)
{
  std::cout << "linear x: " << msg->linear.x << std::endl;
  std::cout << "linear y: " << msg->linear.y << std::endl;
  std::cout << "linear z: " << msg->linear.z << std::endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motors_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/cmd_vel", 1000, commandVelCB);

  ros::spin();

  return 0;
}
