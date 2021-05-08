#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <iostream>
#include <wiringPiI2C.h>
#include <mutex>
#include <cstdlib>
#include <wiringPi.h>

#define RST_PIN 7

float desired_ang_vel_ = 0, wheel_radius_ = 0.036F, error_sum = 0, last_error = 0;
int P_ = 25, I_ = 30, D_ = 1, fd_ = 0, pwm_pid_offset_ = 6, motor_offset_ = 0, wheel_dir_offset_ = 2;
std::mutex mutex_;

void do_pid(float current, float desired){
  float error = desired - current;
  error_sum += error;
  int correction;
  if(abs(error) < 0.01){
    correction = 0;
  } else{
    float p_error = P_ * error;
    float i_error = error_sum / I_;
    float d_error = D_ * (error - last_error);
    correction = (int) (p_error + i_error + d_error);
  }
  last_error = error;
  if(correction < -1023){
    correction = -1023;
  } else if(correction > 1023){
    correction = 1023;
  }
  std::cout << "Desired: " << desired << " Applying correction: " << correction << std::endl;
  wiringPiI2CWriteReg16(fd_, pwm_pid_offset_, correction);
}

void currentVelCB(const std_msgs::Float32::ConstPtr& msg)
{
  float current_ang_vel = msg->data;
  std::cout << current_ang_vel << std::endl;
  if(current_ang_vel < 0){
    std::cout << "Negative angular vel, not applying PID" << std::endl;
  } else{
    std::cout << "Angular velocity: " << current_ang_vel << " Rad/s" << std::endl;
    std::cout << "Linear velocity: " << current_ang_vel * wheel_radius_ << " m/s" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    do_pid(current_ang_vel, desired_ang_vel_);
  }
}

void pidVelCB(const std_msgs::Float32::ConstPtr& msg){
  std::lock_guard<std::mutex> lock(mutex_);
  desired_ang_vel_ = msg->data;
}

int main(int argc, char **argv)
{
  // Set reset to high
  wiringPiSetup();
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);

  fd_ = wiringPiI2CSetup(0x4);

  std::cout << "i2c fd: " << fd_ << std::endl;
  // turn on motor and set to move forward
  // Enable motor, 1 to enable, 0 to disable
  wiringPiI2CWriteReg8(fd_, motor_offset_, 1);
  std::cout << "Motor enabled" << std::endl;
  // 1:forward | 0: backward
  wiringPiI2CWriteReg8(fd_, wheel_dir_offset_, 1);
  std::cout << "Going forward" << std::endl;
  ros::init(argc, argv, "pid_node");
  ros::NodeHandle n;
  ros::Subscriber vel_sub = n.subscribe("/left_wheel_vel", 1000, currentVelCB);
  ros::Subscriber pid_sub = n.subscribe("/left_wheel_pid", 1000, pidVelCB);
  ros::spin();

  return 0;
}
