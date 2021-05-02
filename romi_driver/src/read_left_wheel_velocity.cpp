#include "ros/ros.h"
#include "std_msgs/String.h"
#include <wiringPiI2C.h>
#include <math.h>

int fd = 0, address = 4, offset = 3;

int get_angular_vel(){
  return wiringPiI2CReadReg16(fd, offset);
}

float degrees_to_radians(int deg){
  std::cout << "Value of Pi: " << M_PI << std::endl;
  return deg * M_PI / 180;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "left_encoder_node");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("/left_wheel_vel", 1000);

  ros::Rate loop_rate(1);

  fd = wiringPiI2CSetup(address);

  while (ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    int deg_ps = get_angular_vel();
    float rad_ps = degrees_to_radians(deg_ps);
    ss << "Deg/s: " << deg_ps << " Rad/s: " << rad_ps;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
