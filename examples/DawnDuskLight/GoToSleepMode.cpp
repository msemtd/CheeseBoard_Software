#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include "ClockDisplay.h"
#include "GoToSleepMode.h"
#include "StandbyMode.h"
#include "ModeManager.h"
#include "RealTimeClock.h"
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
    _lastFade = 0;
    _finished = false;
    _lastModeLineUpdate = 0;
    _lastModeLine = "";
    _lastFadeChange = 0;
    _initialColor = fadeColor(dayColor(RealTimeClock.daySeconds()), EspApConfigurator[SET_MAX_BRIGHTNESS]->get().toFloat());

    ClockDisplay.enable();
    ClockDisplay.setNightMode(false);
    _fadeMinutes = EspApConfigurator[SET_SLEEP_DURATION]->get().toInt();
    applyFadeDuration();
}

void GoToSleepModeClass::modeStop()
{
    DB(F("GoToSleepMode::modeStop"));
    saveFadeDuration();
}

void GoToSleepModeClass::modeUpdate()
{
    if (Millis() > _lastFade + FadeMs) {
        _lastFade = Millis();
        long seconds = RealTimeClock.daySeconds();
        float percent = getFadePercent();
        uint32_t faded = fadeColor(_initialColor, percent);
        DBF("GoToSleepModeClass::modeUpdate updating LEDs lf=%ld sec=%ld full=0x%06x fade=%.1f%% faded=0x%06x\n",
            _lastFade,
            seconds,
            _initialColor,
            percent,
            faded);
        for (uint16_t i=0; i<RGBLED_COUNT; i++) { 
            CbLeds.setPixelColor(i, faded);
        }
        CbLeds.show();
        if (percent <= FloatTolerance) {
            DBLN(F("GoToSleepModeClass::modeUpdate fade complete"));
            _finished = true;
        }
    }

    if (Millis() > _lastModeLineUpdate + 2000) {
        _lastModeLineUpdate = Millis();
        updateModeLine();
    }

    if (_lastFadeChange > 0 && Millis() > (_lastFadeChange + 10000)) {
        _lastFadeChange = 0;
        saveFadeDuration();
    }
}

void GoToSleepModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("GoToSleepModeClass::pushEvent ms="));
    DBLN(durationMs);
    ClockDisplay.setNightMode(true);
    ModeManager.switchMode(&StandbyMode);
}

void GoToSleepModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("GoToSleepModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
    adjustFadeDuration(diff);
}

void GoToSleepModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("GoToSleepModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
    adjustFadeDuration(diff*5);
}

void GoToSleepModeClass::adjustFadeDuration(int8_t minutes)
{
    DB(F("GoToSleepModeClass::adjustFadeDuration current="));
    DB(_fadeMinutes);
    DB(F(" adjust mins="));
    DBLN(minutes);
    int newFade = _fadeMinutes;
    newFade += minutes;
    if (newFade > 60) {
        newFade = 60;
    } else if (newFade < 1) {
        newFade = 1;
    }
    _fadeMinutes = newFade;
    _lastFadeChange = Millis();
    applyFadeDuration();
}

void GoToSleepModeClass::saveFadeDuration()
{
    String currentValue = String(_fadeMinutes);
    EspApConfigurator[SET_SLEEP_DURATION]->load();
    String savedValue = EspApConfigurator[SET_SLEEP_DURATION]->get();
    if (currentValue != savedValue) {
        DB(F("GoToSleepModeClass::saveFadeDuration "));
        DB(currentValue);
        DB(F(" != "));
        DB(savedValue);
        DBLN(F(", saving"));
        EspApConfigurator[SET_SLEEP_DURATION]->set(currentValue);
        EspApConfigurator[SET_SLEEP_DURATION]->save();
    } else {
        _fadeMinutes = currentValue.toInt();
    }
}

void GoToSleepModeClass::applyFadeDuration()
{
    _fadeStartUnixTime = RealTimeClock.unixTime();
    updateModeLine();
}

float GoToSleepModeClass::getFadePercent()
{
    uint32_t secDone = RealTimeClock.unixTime() - _fadeStartUnixTime;
    float f = (secDone * 100.) / (_fadeMinutes * 60);
    if (f < 0) { f = 0; }
    else if (f > 100) {
        f = 100;
    }
    return 100-f;
}

void GoToSleepModeClass::updateModeLine()
{
    String newModeLine(F("Lights out in "));
    newModeLine += _fadeMinutes - ((RealTimeClock.unixTime() - _fadeStartUnixTime) / 60);
    newModeLine += F(" minutes");
    if (newModeLine != _lastModeLine) {
        DB(F("GoToSleepModeClass::updateModeLine updating: "));
        DBLN(newModeLine);
        ClockDisplay.setModeLine(newModeLine);
        _lastModeLine = newModeLine;
    }
}


