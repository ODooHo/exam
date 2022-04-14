#include <iostream>
#include <wiringPi.h>
#include <mcp3004.h>
#include <unistd.h>
using namespace std;

#define BASE 1024
#define SPI_LIGHT 0
#define LED_R 27
#define LED_G 20
#define LED_B 21

int main()
{
    wiringPiSetupGpio();
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    mcp3004Setup(BASE, SPI_LIGHT);

    int x;

    while (1)
    {
        x = analogRead(BASE);
        if (x < 200)
        {
            digitalWrite(LED_R, HIGH);
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_B, LOW);
        }
        else if (x < 650)
        {
            digitalWrite(LED_G, HIGH);
            digitalWrite(LED_R, LOW);
            digitalWrite(LED_B, LOW);
        }
        else
        {
            digitalWrite(LED_B, HIGH);
            digitalWrite(LED_R, LOW);
            digitalWrite(LED_G, LOW);
        }
        cout << x << endl;
        delay(200);
    }
    return 0;
}