#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include <iostream>

float wheel_dist_half = 0.141F / 2;
float wheel_radius = 0.036F;
ros::Publisher l_wheel_pub, r_wheel_pub;


void cmdVelCB(const geometry_msgs::Twist::ConstPtr& twist_msg){
  std_msgs::Float32 right_msg;
  std_msgs::Float32 left_msg;

  // First apply linear
  float vel = twist_msg->linear.x;
  float right_linear = vel, left_linear = vel;
  // Then apply angular
  float ang = twist_msg->angular.z;
  if(ang > 0){
    right_linear += ang * wheel_dist_half;
    left_linear -= ang * wheel_dist_half; 
  } else {
    right_linear -= ang * wheel_dist_half;
    left_linear += ang * wheel_dist_half; 
  }

  right_msg.data = right_linear / wheel_radius;
  left_msg.data = left_linear / wheel_radius;
  l_wheel_pub.publish(left_msg);
  r_wheel_pub.publish(right_msg);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "motors_vel_node");
  ros::NodeHandle n;
  l_wheel_pub = n.advertise<std_msgs::Float32>("/rwheel_desired_rate", 1000);
  r_wheel_pub = n.advertise<std_msgs::Float32>("/lwheel_desired_rate", 1000);
  ros::Subscriber cmd_vel_sub = n.subscribe("/cmd_vel", 1000, cmdVelCB);
  ros::spin();
 
  return 0;
}