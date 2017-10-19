#include <Arduino.h>
#include "CbMillis.h"

float _CbMillisOffsetMs = 0.;

unsigned long CbMillis() {
    return millis() + (unsigned long)_CbMillisOffsetMs;
}

void addCbMillisOffset(float ms)
{
    _CbMillisOffsetMs += ms;
}

