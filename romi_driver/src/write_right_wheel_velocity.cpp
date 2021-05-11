#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <iostream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "right_motor_vel_node");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<std_msgs::Float32>("/right_wheel_pid", 1000);
  float desired_ang_vel_ = 0;

  while(true){
    std::cout << "Desired right angular velocity: " << std::endl;
    std::cin >> desired_ang_vel_;
    std_msgs::Float32 msg;
    msg.data = desired_ang_vel_;
    pub.publish(msg);
    ros::spinOnce();
  }

  return 0;
}