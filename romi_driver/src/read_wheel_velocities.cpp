#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <wiringPiI2C.h>
#include <math.h>
#include <chrono>

// right wheel: 3 left wheel: 4
int left_device_ = 0, right_device_ = 0, address_ = 3, dir_offset_ = 2, vel_offset_ = 3;
int pub_rate_ = 10, counts_per_rev_ = 1440;
std::chrono::steady_clock::time_point llast_time_ = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point rlast_time_ = std::chrono::steady_clock::now();

float get_left_angular_vel(){
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  float time_elapsed = std::chrono::duration<float>(now - llast_time_).count();
  llast_time_ = now;
  return ((float) wiringPiI2CReadReg16(left_device_, vel_offset_) / 4) / time_elapsed;
}

float get_right_angular_vel(){
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  float time_elapsed = std::chrono::duration<float>(now - rlast_time_).count();
  rlast_time_ = now;
  return ((float) wiringPiI2CReadReg16(right_device_, vel_offset_) / 4) / time_elapsed;
}

int get_wheel_direction(int device){
  return wiringPiI2CReadReg8(device, dir_offset_);
}

float degrees_to_radians(float deg){
  return deg * M_PI / 180;
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "encoders_node");

  ros::NodeHandle n;

  ros::Publisher left_vel_pub = n.advertise<std_msgs::Float32>("/left_wheel_vel", 1000);
  ros::Publisher right_vel_pub = n.advertise<std_msgs::Float32>("/right_wheel_vel", 1000);

  ros::Rate loop_rate(pub_rate_);

  right_device_ = wiringPiI2CSetup(address_);
  left_device_ = wiringPiI2CSetup(address_ + 1);

  while (ros::ok())
  {
    std_msgs::Float32 lmsg;
    std_msgs::Float32 rmsg;

    float lrad_ps = degrees_to_radians(get_left_angular_vel());
    lmsg.data = lrad_ps;

    float rrad_ps = degrees_to_radians(get_right_angular_vel());
    rmsg.data = rrad_ps;

    std::cout << "left rad/s: " << lrad_ps << std::endl;
    std::cout << "right rad/s: " << rrad_ps << std::endl;

    left_vel_pub.publish(lmsg);
    right_vel_pub.publish(rmsg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
