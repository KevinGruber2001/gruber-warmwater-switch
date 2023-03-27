#include <ww_client/Led.h>

void Led::toggleOn()
{
    digitalWrite(pin, HIGH);
};

void Led::toggleOff()
{
    digitalWrite(pin, LOW);
}