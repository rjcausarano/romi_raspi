#include "ros/ros.h"
#include "std_msgs/String.h"

#include <wiringPiI2C.h>

int fd = 0, address = 4, offset = 3;

int get_rpm(){
  return wiringPiI2CReadReg16(fd, offset);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "encoders_node");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("/counts", 1000);

  ros::Rate loop_rate(10);

  fd = wiringPiI2CSetup(address);

  int count = 0;
  while (ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << get_rpm() << " counts";
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
