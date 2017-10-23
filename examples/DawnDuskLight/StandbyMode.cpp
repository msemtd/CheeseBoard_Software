#include <MutilaDebug.h>
#include <Millis.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include <EspApConfigurator.h>
#include <PersistentSettingTime.h>
#include "StandbyMode.h"
#include "RealTimeClock.h"
#include "ClockDisplay.h"
#include "OnMode.h"
#include "WakeUpMode.h"
#include "ModeManager.h"
#include "Config.h"

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
    // This will update the mode line without actually changing the wake time
    adjustWakeTime(0);
    _fade = true;
    _lastFade = 0;
    _lastWakeCheck = 0;
}

//! Called when this mode is de-activated
void StandbyModeClass::modeStop()
{
    DB(F("StandbyMode::modeStop"));
    saveWakeTime();
}

//! Called periodically while mode is active
void StandbyModeClass::modeUpdate()
{
    fadeLed();

    if (Millis() > _lastWakeCheck + 3000) {
        _lastWakeCheck = Millis();
        // How many seconds until the wake time
        uint32_t untilWakeEnd = RealTimeClock.secondsUntilNext(EspApConfigurator[SET_WAKE_TIME]->get());
        int32_t untilWakeStart = untilWakeEnd - (EspApConfigurator[SET_WAKE_DURATION]->get().toInt() * 60);
        DB(F("WakeUpMode starts in: "));
        DBLN(untilWakeStart);
        if (untilWakeStart <= 0) {
            ModeManager.switchMode(&WakeUpMode);
        }
    }

    // Periodically check to see if the wake time has changed and save it to EEPROM 
    // if it has changed.  The reason we delay is that we don't want to write EEPROM 
    // hundreds of times when the value is being changed... we we only try to save
    // some seconds after a change...
    if (_lastWakeChange > 0 && Millis() > _lastWakeChange + 10000) {
        _lastWakeChange = 0;
        saveWakeTime();
    }
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
    adjustWakeTime(diff);
}

void StandbyModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("StandbyModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
    adjustWakeTime(diff*30);
}

void StandbyModeClass::adjustWakeTime(int8_t minutes)
{
    DB(F("StandbyModeClass::adjustWakeTime mins="));
    DB(minutes);
    long wakeSeconds = (timeStrToSeconds(EspApConfigurator[SET_WAKE_TIME]->get()) + (minutes*60) + (60*60*24)) % (60*60*24);
    DB(F(" wakeSeconds="));
    DB(wakeSeconds);
    String wakeString = secondsToTimeStr(wakeSeconds, false);
    DB(F(" wakeString="));
    DBLN(wakeString);
    EspApConfigurator[SET_WAKE_TIME]->set(wakeString);
    String s(F("Wake at "));
    s += wakeString;
    ClockDisplay.setModeLine(s);
    _lastWakeChange = Millis();
}

void StandbyModeClass::saveWakeTime()
{
    String currentValue = EspApConfigurator[SET_WAKE_TIME]->get();
    EspApConfigurator[SET_WAKE_TIME]->load();
    String savedValue = EspApConfigurator[SET_WAKE_TIME]->get();
    if (currentValue != savedValue) {
        DB(F("StandbyModeClass::saveWakeTime "));
        DB(currentValue);
        DB(F(" != "));
        DB(savedValue);
        DBLN(F(", saving"));
        EspApConfigurator[SET_WAKE_TIME]->set(currentValue);
        EspApConfigurator[SET_WAKE_TIME]->save();
    }
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



