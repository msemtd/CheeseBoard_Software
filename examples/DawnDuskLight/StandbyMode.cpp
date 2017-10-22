#include <MutilaDebug.h>
#include <Millis.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include "StandbyMode.h"
#include "ModeRealTime.h"
#include "ClockDisplay.h"
#include "OnMode.h"
#include "ModeManager.h"

StandbyModeClass StandbyMode;

StandbyModeClass::StandbyModeClass()
{
}

void StandbyModeClass::begin()
{
    DB(F("StandbyMode::begin"));
}

//! Called when this mode is activated
void StandbyModeClass::modeStart()
{
    DB(F("StandbyMode::modeStart"));
    ClockDisplay.enable();
    _fade = true;
    _lastFade = 0;
}

//! Called when this mode is de-activated
void StandbyModeClass::modeStop()
{
    DB(F("StandbyMode::modeStop"));
}

//! Called periodically while mode is active
void StandbyModeClass::modeUpdate()
{
    fadeLed();
}

void StandbyModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("StandbyModeClass::pushEvent ms="));
    DBLN(durationMs);
    ModeManager.switchMode(&OnMode);
}

void StandbyModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("StandbyModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void StandbyModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("StandbyModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void StandbyModeClass::fadeLed()
{
    bool allBlack = true;
    if (_fade && Millis() > _lastFade + FadeMs) {
        _lastFade = Millis();
        DBLN(F("StandbyMode::fade"));
        for (uint16_t i=0; i<RGBLED_COUNT; i++) {
            uint32_t color = CbLeds.getPixelColor(i);
            uint8_t r = fadeChannel(color >> 16);
            uint8_t g = fadeChannel((color >> 8) % 256);
            uint8_t b = fadeChannel(color % 256);
            CbLeds.setPixelColor(i, r, g, b);
            if (r > 0 || g > 0 || b > 0) {
                allBlack = false;
            }
        }
        CbLeds.show();
        if (allBlack) {
            DBLN(F("allBlack now, stopping fade"));
            _fade = false;
        }
    }
}

uint8_t StandbyModeClass::fadeChannel(uint8_t c)
{
    float f(c);
    f *= FadeRatio;
    return (uint8_t)f;
}



