#include <wiringPi.h>
#include <unistd.h>

#define RST_PIN 7

int main(){
    wiringPiSetup();
    pinMode(RST_PIN, OUTPUT);
    digitalWrite(RST_PIN, LOW);
    sleep(1);
    digitalWrite(RST_PIN, HIGH);
}