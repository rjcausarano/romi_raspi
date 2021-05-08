#include <wiringPi.h>

#define RST_PIN 7

int main(){
    wiringPiSetupGpio();

    pinMode(RST_PIN, OUTPUT);

    digitalWrite(RST_PIN, 0);
}