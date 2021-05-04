#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <iostream>
#include <wiringPiI2C.h>

float desired_ang_vel_ = 0;
int P_ = 100, I_ = 1, D_ = 1, fd_ = 0, pwm_pid_offset_ = 6;

void do_pid(float current, float desired){
  int error = desired - current;
  int p_error = P_ * error;
  int correction = p_error;
  if(correction < -1023){
    correction = -1023;
  } else if(correction > 1023){
    correction = 1023;
  }
  wiringPiI2CWriteReg16(fd_, pwm_pid_offset_, correction);
}

void currentVelCB(const std_msgs::Float32::ConstPtr& msg)
{
  float current_ang_vel = msg->data;
  std::cout << "Angular velocity: " << current_ang_vel << " Rad/s" << std::endl;
  do_pid(current_ang_vel, desired_ang_vel_);
}

void pidVelCB(const std_msgs::Float32::ConstPtr& msg){
  desired_ang_vel_ = msg->data;
}

int main(int argc, char **argv)
{
  fd_ = wiringPiI2CSetup(0x4);
  ros::init(argc, argv, "pid_node");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/left_wheel_vel", 1000, currentVelCB);
  ros::Subscriber pid_sub = n.subscribe("/left_wheel_pid", 1000, pidVelCB);
  ros::spin();

  return 0;
}
