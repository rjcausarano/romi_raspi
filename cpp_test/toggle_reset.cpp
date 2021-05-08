#include <wiringPi.h>
#include <unistd.h>

#define RST_PIN 7

bool on_off = true;
int main(){
    wiringPiSetup();

    pinMode(RST_PIN, OUTPUT);

    while(true){
        on_off = !on_off;
        
        if(on_off) digitalWrite(RST_PIN, HIGH);
        else digitalWrite(RST_PIN, LOW);

        sleep(2);
    }
}