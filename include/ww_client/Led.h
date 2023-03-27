#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led
{

private:
    int pin;

public:
    Led(int pin)
    {
        this->pin = pin;
        pinMode(pin, OUTPUT);
    }
    void toggleOn();
    void toggleOff();
};

#endif