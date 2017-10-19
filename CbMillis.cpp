#include <Arduino.h>
#include "CbMillis.h"

float _CbMillisOffsetMs = 0.;

unsigned long CbMillis() {
    return millis() + (unsigned long)_CbMillisOffsetMs;
}

void addMillisOffset(float ms)
{
    _CbMillisOffsetMs += ms;
}

