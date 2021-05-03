#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
   int fd = 0, ang_vel = 0, speed_offset = 0, motor_offset = 0;

   fd = wiringPiI2CSetup(0x4);
   // speed offset is 3, motor offset is 0
   speed_offset = 3;
   motor_offset = 0;

   std::cout << "I2C setup successful"<< fd << std::endl;

   // Enable motor, 1 to enable 0 to disable
   wiringPiI2CWriteReg8(fd, motor_offset, 1);

   while(true){
      std::cout << "angular vel: " << std::endl;
      std::cin >> ang_vel;
      if(ang_vel >= 0 && ang_vel <= 1000){
         std::cout << "sending speed command!" << std::endl;
         wiringPiI2CWriteReg16(fd, speed_offset, ang_vel);
      } else {
         std::cout << "number not in range 0-1000" << std::endl;
      }
   }
}
