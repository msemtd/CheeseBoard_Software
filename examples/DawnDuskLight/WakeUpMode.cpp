#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include "ClockDisplay.h"
#include "WakeUpMode.h"
#include "StandbyMode.h"
#include "ModeManager.h"
#include "RealTimeClock.h"
#include "ColorUtils.h"
#include "Config.h"

WakeUpModeClass WakeUpMode;

WakeUpModeClass::WakeUpModeClass()
{
}

void WakeUpModeClass::begin()
{
    DB(F("WakeUpMode::begin"));
}

void WakeUpModeClass::modeStart()
{
    DB(F("WakeUpMode::modeStart"));
    _lastFade = 0;
    _finished = false;
    _lastModeLineUpdate = 0;
    _lastModeLine = "";
    _postWake = false;

    ClockDisplay.enable();
    ClockDisplay.setNightMode(false);
    _fadeSeconds = EspApConfigurator[SET_WAKE_DURATION]->get().toInt() * 60;
}

void WakeUpModeClass::modeStop()
{
    DB(F("WakeUpMode::modeStop"));
}

void WakeUpModeClass::modeUpdate()
{
    if (Millis() > _lastFade + FadeMs) {
        _lastFade = Millis();
        long seconds = RealTimeClock.daySeconds();
        float percent;
        if (_postWake) { 
            long waitLeft = RealTimeClock.unixTime() - _fadeStartUnixTime;
            if (waitLeft > EspApConfigurator[SET_WAKE_POST_TIME]->get().toInt() * 60) {
                _finished = true;
            } else {
                DB(F("WakeUpModeClass::modeUpdate postFade left="));
                DBLN(waitLeft);
                return;
            }
        } else {
            percent = getFadePercent();
        }
        uint32_t color = MORNING_COLOR;
        uint32_t faded = fadeColor(color, percent);
        DBF("WakeUpModeClass::modeUpdate updating LEDs lf=%ld sec=%ld full=0x%06x fade=%.1f%% faded=0x%06x\n",
            _lastFade,
            seconds,
            color,
            percent,
            faded);
        for (uint16_t i=0; i<RGBLED_COUNT; i++) { 
            CbLeds.setPixelColor(i, faded);
        }
        CbLeds.show();
        if (percent >= 100-FloatTolerance) {
            DBLN(F("WakeUpModeClass::modeUpdate fade complete"));
            _postWake = true;
            _fadeStartUnixTime = RealTimeClock.unixTime();
        }
    }

    if (Millis() > _lastModeLineUpdate + 2000) {
        _lastModeLineUpdate = Millis();
        updateModeLine();
    }

}

void WakeUpModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("WakeUpModeClass::pushEvent ms="));
    DBLN(durationMs);
    ModeManager.switchMode(&StandbyMode);
}

void WakeUpModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("WakeUpModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void WakeUpModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("WakeUpModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

float WakeUpModeClass::getFadePercent()
{
    return 100. - (100. * RealTimeClock.secondsUntilNext(EspApConfigurator[SET_WAKE_TIME]->get()) / _fadeSeconds);
}

void WakeUpModeClass::updateModeLine()
{
    String newModeLine;
    if (_postWake) {
        newModeLine = F("Turning off in ");
        int mins = ((_fadeStartUnixTime + (EspApConfigurator[SET_WAKE_POST_TIME]->get().toInt()*60) - RealTimeClock.unixTime()) / 60)+1;
        newModeLine += mins;
        newModeLine += mins == 1 ? F(" min") : F(" mins");
    } else {
        newModeLine = F("Waking... ");
        newModeLine += String(getFadePercent(), 0);
        newModeLine += '%';
    }
    if (newModeLine != _lastModeLine) {
        DB(F("WakeUpModeClass::updateModeLine updating: "));
        DBLN(newModeLine);
        ClockDisplay.setModeLine(newModeLine);
        _lastModeLine = newModeLine;
    }
}


