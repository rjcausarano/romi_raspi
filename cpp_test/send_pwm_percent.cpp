#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
   int fd = 0, pwm_percent = 0, pwm_offset = 0, motor_offset = 0;

   fd = wiringPiI2CSetup(0x4);
   // PWM offset is 1, motor offset is 0
   pwm_offset = 1;
   motor_offset = 0;

   std::cout << "I2C setup successful"<< fd << std::endl;

   // Enable motor, 1 to enable 0 to disable
   wiringPiI2CWriteReg8(fd, motor_offset, 1);

   while(true){
      std::cout << "PWM percent: " << std::endl;
      std::cin >> pwm_percent;
      if(pwm_percent >= 0 && pwm_percent <= 100){
         std::cout << "sending speed command!" << std::endl;
         wiringPiI2CWriteReg8(fd, pwm_offset, pwm_percent);
      } else {
         std::cout << "number not in range 0-100" << std::endl;
      }
   }
}
