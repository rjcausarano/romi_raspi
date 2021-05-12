#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <iostream>
#include <wiringPiI2C.h>
#include <mutex>
#include <cstdlib>
#include <wiringPi.h>

#define RST_PIN 7

float ldesired_ang_vel_ = 0, rdesired_ang_vel_ = 0, wheel_radius_ = 0.036F, lerror_sum_ = 0, rerror_sum_ = 0, llast_error_ = 0, rlast_error_ = 0;
// right wheel: 3 left wheel: 4
int address_ = 3, left_device_ = 0, right_device_ = 0, P_ = 25, I_ = 30, D_ = 1, pwm_pid_offset_ = 6, motor_offset_ = 0, wheel_dir_offset_ = 2;
std::mutex mutex_;
bool lforward_ = true, rforward_ = true;

void do_pid_left(float current, float desired){
  float error = desired - current;
  lerror_sum_ += error;

  int correction;
  if(abs(error) < 0.01){
    correction = 0;
  } else{
    float p_error = P_ * error;
    float i_error = lerror_sum_ / I_;
    float d_error = D_ * (error - llast_error_);
    correction = (int) (p_error + i_error + d_error);
  }
  llast_error_ = error;
  if(correction < -1023){
    correction = -1023;
  } else if(correction > 1023){
    correction = 1023;
  }
  std::cout << "Desired: " << desired << " Applying correction: " << correction << std::endl;
  wiringPiI2CWriteReg16(left_device_, pwm_pid_offset_, correction);
}

void do_pid_right(float current, float desired){
  float error = desired - current;
  rerror_sum_ += error;

  int correction;
  if(abs(error) < 0.01){
    correction = 0;
  } else{
    float p_error = P_ * error;
    float i_error = rerror_sum_ / I_;
    float d_error = D_ * (error - rlast_error_);
    correction = (int) (p_error + i_error + d_error);
  }
  rlast_error_ = error;
  if(correction < -1023){
    correction = -1023;
  } else if(correction > 1023){
    correction = 1023;
  }
  std::cout << "Desired: " << desired << " Applying correction: " << correction << std::endl;
  wiringPiI2CWriteReg16(right_device_, pwm_pid_offset_, correction);
}

void currentLeftVelCB(const std_msgs::Float32::ConstPtr& msg)
{
  float current_ang_vel = msg->data;
  std::cout << current_ang_vel << std::endl;
  if(current_ang_vel < 0){
    std::cout << "Negative angular vel, not applying PID" << std::endl;
  } else{
    std::cout << "Angular velocity: " << current_ang_vel << " Rad/s" << std::endl;
    std::cout << "Linear velocity: " << current_ang_vel * wheel_radius_ << " m/s" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    if(ldesired_ang_vel_ > 0 && lforward_ == false){
      // let's go forward
      wiringPiI2CWriteReg8(left_device_, wheel_dir_offset_, 1);
      lforward_ = true;
    } else if(ldesired_ang_vel_ < 0 && lforward_ == true){
      // let's go backwards
      wiringPiI2CWriteReg8(left_device_, wheel_dir_offset_, 0);
      lforward_ = false;
    }
    do_pid_left(current_ang_vel, abs(ldesired_ang_vel_));
  }
}

void currentRightVelCB(const std_msgs::Float32::ConstPtr& msg)
{
  float current_ang_vel = msg->data;
  std::cout << current_ang_vel << std::endl;
  if(current_ang_vel < 0){
    std::cout << "Negative angular vel, not applying PID" << std::endl;
  } else{
    std::cout << "Angular velocity: " << current_ang_vel << " Rad/s" << std::endl;
    std::cout << "Linear velocity: " << current_ang_vel * wheel_radius_ << " m/s" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    if(rdesired_ang_vel_ > 0 && rforward_ == false){
      // let's go forward
      wiringPiI2CWriteReg8(right_device_, wheel_dir_offset_, 1);
      rforward_ = true;
    } else if(rdesired_ang_vel_ < 0 && rforward_ == true){
      // let's go backwards
      wiringPiI2CWriteReg8(right_device_, wheel_dir_offset_, 0);
      rforward_ = false;
    }
    do_pid_right(current_ang_vel, abs(rdesired_ang_vel_));
  }
}

float get_angular_vel(int ticks_ps){
  return (float) (ticks_ps / 4);
}

void pidLeftVelCB(const std_msgs::Float32::ConstPtr& msg){
  std::lock_guard<std::mutex> lock(mutex_);
  ldesired_ang_vel_ = msg->data;
}

void pidRightVelCB(const std_msgs::Float32::ConstPtr& msg){
  std::lock_guard<std::mutex> lock(mutex_);
  rdesired_ang_vel_ = msg->data;
}

int main(int argc, char **argv)
{
  // Set reset to high
  wiringPiSetup();
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);

  right_device_ = wiringPiI2CSetup(address_);
  left_device_ = wiringPiI2CSetup(address_ + 1);

  // turn on motor and set to move forward
  // Enable motor, 1 to enable, 0 to disable
  wiringPiI2CWriteReg8(left_device_, motor_offset_, 1);
  wiringPiI2CWriteReg8(right_device_, motor_offset_, 1);
  std::cout << "Motor enabled" << std::endl;
  // 1:forward | 0: backward
  wiringPiI2CWriteReg8(left_device_, wheel_dir_offset_, 1);
  wiringPiI2CWriteReg8(right_device_, wheel_dir_offset_, 1);
  std::cout << "Going forward" << std::endl;

  ros::init(argc, argv, "wheels_pid_node");
  ros::NodeHandle n;

  ros::Subscriber lvel_sub = n.subscribe("/left_wheel_vel", 1000, currentLeftVelCB);
  ros::Subscriber rvel_sub = n.subscribe("/right_wheel_vel", 1000, currentRightVelCB);
  ros::Subscriber lpid_sub = n.subscribe("/lwheel_desired_rate", 1000, pidLeftVelCB);
  ros::Subscriber rpid_sub = n.subscribe("/rwheel_desired_rate", 1000, pidRightVelCB);
  ros::spin();

  return 0;
}
