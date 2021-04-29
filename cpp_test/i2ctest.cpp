#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

using namespace std;

int main()
{
   int fd, result, offset;

   fd = wiringPiI2CSetup(0x4);
   offset = 5;

   cout << "I2C setup successful: "<< fd << endl;

   result = wiringPiI2CReadReg8(fd, offset);

   cout << "LED status: " << result << std::endl;

   // toggle LED
   int status = 0;
   while(true){
      wiringPiI2CWriteReg8(fd, offset, status);
      status = !status;
      sleep(5);
   }
}
