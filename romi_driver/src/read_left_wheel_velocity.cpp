#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <wiringPiI2C.h>
#include <math.h>
#include <chrono>

int fd = 0, address = 3, dir_offset = 2, vel_offset = 3;
int pub_rate = 10, counts_per_rev = 1440;
std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

float get_angular_vel(){
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  float time_elapsed = std::chrono::duration<float>(now - last_time).count();
  last_time = now;
  return ((float) wiringPiI2CReadReg16(fd, vel_offset) / 4) / time_elapsed;
}

int get_wheel_direction(){
  return wiringPiI2CReadReg8(fd, dir_offset);
}

float degrees_to_radians(float deg){
  return deg * M_PI / 180;
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "left_encoder_node");

  ros::NodeHandle n;

  ros::Publisher left_vel_pub = n.advertise<std_msgs::Float32>("/left_wheel_vel", 1000);

  ros::Rate loop_rate(pub_rate);

  fd = wiringPiI2CSetup(address);

  while (ros::ok())
  {
    std_msgs::Float32 msg;

    float deg_ps = get_angular_vel();
    // int direction = get_wheel_direction();
    float rad_ps = degrees_to_radians(deg_ps);
    msg.data = rad_ps;

    std::cout << "Deg/s: " << deg_ps << " Rad/s: " << rad_ps << std::endl;

    left_vel_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
