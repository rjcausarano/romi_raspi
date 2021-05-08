#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
   int fd = 0, address = 3, pwm_percent = 0, pwm_offset = 0, motor_offset = 0, wheel_dir_offset = 0;;

   fd = wiringPiI2CSetup(address);
   // PWM offset is 1, motor offset is 0
   motor_offset = 0;
   pwm_offset = 1;
   wheel_dir_offset = 2;

   std::cout << "I2C setup successful"<< fd << std::endl;
   // 1:forward | 0: backward
   wiringPiI2CWriteReg8(fd, wheel_dir_offset, 1);

   while(true){
      std::cout << "PWM percent: " << std::endl;
      std::cin >> pwm_percent;
      if(pwm_percent >= 0 && pwm_percent <= 100){
         std::cout << "sending speed command!" << std::endl;
         if(pwm_percent > 0){
            // Enable motor, 1 to enable 0 to disable
            wiringPiI2CWriteReg8(fd, motor_offset, 1);
         } else{
            wiringPiI2CWriteReg8(fd, motor_offset, 0);
         }
         wiringPiI2CWriteReg8(fd, pwm_offset, pwm_percent);
      } else {
         std::cout << "number not in range 0-100" << std::endl;
      }
   }
}
