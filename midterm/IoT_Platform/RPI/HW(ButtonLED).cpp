#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define LED_GPIOR 18
#define LED_GPIOG 13
#define LED_GPIOY 22
#define BUTTON_GPIOR 23
#define BUTTON_GPIOG 24
#define BUTTON_GPIOY 26
#define DEBOUNCE_TIME 200
bool running1 = false;
bool running2 = false;
bool running3 = false;

void buttonPress1(void)
{
    static unsigned long lastISRTime = 0;
    static unsigned long count = 0;
    unsigned long currentISRTime = millis();
    if (currentISRTime - lastISRTime > DEBOUNCE_TIME)
    {
        running1 = !running1;
    }
    lastISRTime = currentISRTime;
}

void buttonPress2(void)
{
    static unsigned long lastISRTime = 0;
    static unsigned long count = 0;
    unsigned long currentISRTime = millis();
    if (currentISRTime - lastISRTime > DEBOUNCE_TIME)
    {
        running2 = !running2;
    }
    lastISRTime = currentISRTime;
}

void buttonPress3(void)
{
    static unsigned long lastISRTime = 0;
    static unsigned long count = 0;
    unsigned long currentISRTime = millis();
    if (currentISRTime - lastISRTime > DEBOUNCE_TIME)
    {
        running3 = !running3;
    }
    lastISRTime = currentISRTime;
}

int main()
{
    wiringPiSetupGpio();
    pinMode(LED_GPIOR, OUTPUT);
    pinMode(BUTTON_GPIOR, INPUT);
    pinMode(LED_GPIOG, OUTPUT);
    pinMode(BUTTON_GPIOG, INPUT);
    pinMode(LED_GPIOY, OUTPUT);
    pinMode(BUTTON_GPIOY, INPUT);

    wiringPiISR(BUTTON_GPIOR, INT_EDGE_RISING, &buttonPress1);
    wiringPiISR(BUTTON_GPIOG, INT_EDGE_RISING, &buttonPress2);
    wiringPiISR(BUTTON_GPIOY, INT_EDGE_RISING, &buttonPress3);

    while (1)
    {
        if (running1 == true)
        {
            digitalWrite(LED_GPIOR, HIGH);
        }
        else
            digitalWrite(LED_GPIOR, LOW);

        if (running2 == true)
        {
            digitalWrite(LED_GPIOG, HIGH);
        }
        else
            digitalWrite(LED_GPIOG, LOW);

        if (running3 == true)
        {
            digitalWrite(LED_GPIOY, HIGH);
        }
        else
            digitalWrite(LED_GPIOY, LOW);
    }

    return 0;
}