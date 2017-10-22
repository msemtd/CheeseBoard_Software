#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include "ClockDisplay.h"
#include "GoToSleepMode.h"
#include "StandbyMode.h"
#include "ModeManager.h"
#include "ModeRealTime.h"
#include "ColorUtils.h"
#include "Config.h"

GoToSleepModeClass GoToSleepMode;

GoToSleepModeClass::GoToSleepModeClass()
{
}

void GoToSleepModeClass::begin()
{
    DB(F("GoToSleepMode::begin"));
}

void GoToSleepModeClass::modeStart()
{
    DB(F("GoToSleepMode::modeStart"));
    _fade = true;
    _lastFade = 0;
    _finished = false;

    ClockDisplay.enable();
    setFadeTime(EspApConfigurator[SET_SLEEP_DURATION]->get().toInt());

}

void GoToSleepModeClass::modeStop()
{
    DB(F("GoToSleepMode::modeStop"));
}

void GoToSleepModeClass::modeUpdate()
{
    if (_fade) {
        if (Millis() > _lastFade + FadeMs) {
            _lastFade = Millis();
            long seconds = ModeRealTime.daySeconds();
            uint32_t color = dayColor(seconds);
            float percent = getFadePercent();
            uint32_t faded = fadeColor(color, percent);
            DBF("GoToSleepModeClass::modeUpdate updating LEDs lf=%ld sec=%ld full=0x%06x fade=%.1f%% faded=0x%06x\n",
                _lastFade,
                seconds,
                color,
                percent,
                faded);
            for (uint16_t i=0; i<RGBLED_COUNT; i++) { 
                CbLeds.setPixelColor(i, faded);
            }
            CbLeds.show();
            if (percent <= FloatTolerance) {
                DBLN(F("GoToSleepModeClass::modeUpdate fade complete"));
                _fade = false;
                _finished = true;
            }
        }
    }
}

void GoToSleepModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("GoToSleepModeClass::pushEvent ms="));
    DBLN(durationMs);
    ModeManager.switchMode(&StandbyMode);
}

void GoToSleepModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("GoToSleepModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void GoToSleepModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("GoToSleepModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void GoToSleepModeClass::setFadeTime(uint8_t minutes)
{
    DB(F("GoToSleepModeClass::setFadeTime mins="));
    DBLN(minutes);
    _fadeMinutes = minutes;
    _fadeStartUnixTime = ModeRealTime.unixTime();
}

float GoToSleepModeClass::getFadePercent()
{
    uint32_t secDone = ModeRealTime.unixTime() - _fadeStartUnixTime;
    float f = (secDone * 100.) / (_fadeMinutes * 60);
    if (f < 0) { f = 0; }
    else if (f > 100) {
        f = 100;
    }
    return 100-f;
}



