#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>

using namespace std;

int main()
{
   int fd, result, offset;

   fd = wiringPiI2CSetup(0x4);
   offset = 0;

   cout << "I2C setup successful: "<< fd << endl;

   result = wiringPiI2CReadReg8(fd, offset);

   // toggle Motor
   int status = 0;
   while(true){
      wiringPiI2CWriteReg8(fd, offset, status);
      cout << "Motor status: " << status << std::endl;

      status = !status;
      sleep(5);
   }
}
