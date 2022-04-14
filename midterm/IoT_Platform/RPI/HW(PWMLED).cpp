#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;
#define PWM_LEDR 18 
#define PWM_LEDG 13
bool running = true;
int main()
{
    wiringPiSetupGpio();
    pinMode(PWM_LEDR, PWM_OUTPUT);
    pinMode(PWM_LEDG, PWM_OUTPUT);
    pwmSetRange(1000);

    pwmWrite(PWM_LEDR, 0);
    pwmWrite(PWM_LEDG, 0);

    while (running)
    {

        for (double i = 1; i < 1000; i += 0.5)
        {
            pwmWrite(PWM_LEDR, i);
            usleep(1000);
        }
        for (double i = 999; i >= 0; i -= 0.5)
        {
            pwmWrite(PWM_LEDR, i);
            usleep(1000);
        }
        for (double i = 1; i < 1000; i += 0.5)
        {
            pwmWrite(PWM_LEDG, i);
            usleep(1000);
        }
        for (double i = 999; i >= 0; i -= 0.5)
        {
            pwmWrite(PWM_LEDG, i);
            usleep(1000);
        }
    }
    return 0;
}