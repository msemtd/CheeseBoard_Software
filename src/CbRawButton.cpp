#include <Arduino.h>
#include "CbRawButton.h"

CbRawButton::CbRawButton(uint8_t pin, bool pullup) : 
    _pin(pin),
    _pullup(pullup)
{
}

void CbRawButton::begin()
{
    if (_pullup) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }
}

bool CbRawButton::on()
{
    if (_pullup) {
        return !digitalRead(_pin);
    } else {
        return digitalRead(_pin);
    }
}


