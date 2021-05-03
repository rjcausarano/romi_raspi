g++ i2cread.cpp -lwiringPi -lpthread -lrt -lcrypt -o build/i2cread
g++ send_pwm_percent.cpp -lwiringPi -lpthread -lrt -lcrypt -o build/send_pwm_percent
g++ send_ang_vel.cpp -lwiringPi -lpthread -lrt -lcrypt -o build/send_ang_vel