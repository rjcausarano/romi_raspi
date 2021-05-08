#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
   // speed offset is 6, motor offset is 0
   int fd = 0, address = 3, ang_vel = 0, speed_offset = 6, motor_offset = 0, wheel_dir_offset = 2;

   fd = wiringPiI2CSetup(address);

   std::cout << "I2C setup successful"<< fd << std::endl;
   // 1:forward | 0: backward
   wiringPiI2CWriteReg8(fd, wheel_dir_offset, 1);

   // Enable motor, 1 to enable, 0 to disable
   wiringPiI2CWriteReg8(fd, motor_offset, 1);

   while(true){
      std::cout << "angular vel: " << std::endl;
      std::cin >> ang_vel;
      if(ang_vel >= -1000 && ang_vel <= 1000){
         std::cout << "sending speed command!" << std::endl;
         wiringPiI2CWriteReg16(fd, speed_offset, ang_vel);
      } else {
         std::cout << "number not in range 0-1000" << std::endl;
      }
   }
}
