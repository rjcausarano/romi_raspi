#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <wiringPiI2C.h>
#include <math.h>
#include <chrono>

// right wheel: 3 left wheel: 4
int left_device = 0, right_device = 0, address = 3, dir_offset = 2, vel_offset = 3;
int pub_rate = 10, counts_per_rev = 1440;
std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

float get_angular_vel(int device){
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  float time_elapsed = std::chrono::duration<float>(now - last_time).count();
  last_time = now;
  return ((float) wiringPiI2CReadReg16(device, vel_offset) / 4) / time_elapsed;
}

int get_wheel_direction(int device){
  return wiringPiI2CReadReg8(device, dir_offset);
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

  ros::Rate loop_rate(pub_rate);

  right_device = wiringPiI2CSetup(address);
  left_device = wiringPiI2CSetup(address + 1);

  while (ros::ok())
  {
    std_msgs::Float32 lmsg;
    std_msgs::Float32 rmsg;

    float lrad_ps = degrees_to_radians(get_angular_vel(left_device));
    lmsg.data = lrad_ps;

    float rrad_ps = degrees_to_radians(get_angular_vel(right_device));
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
