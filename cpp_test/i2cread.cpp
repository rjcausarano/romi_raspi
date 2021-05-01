#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

int main()
{
   int fd, result, offset;

   fd = wiringPiI2CSetup(0x4);
   // encoder low byte is offset 3
   offset = 3;

   std::cout << "I2C setup successful: "<< fd << std::endl;

   // std::cout << "byte read: " << wiringPiI2CReadReg16(fd, offset) << std::endl;
   std::cout << "byte read: " << wiringPiI2CReadReg16(fd, offset) << std::endl;
}
