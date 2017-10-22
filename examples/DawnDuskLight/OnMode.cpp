#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include "OnMode.h"
#include "StandbyMode.h"
#include "GoToSleepMode.h"
#include "ModeManager.h"
#include "ModeRealTime.h"
#include "ColorUtils.h"
#include "Config.h"

OnModeClass OnMode;

OnModeClass::OnModeClass()
{
}

void OnModeClass::begin()
{
    DB(F("OnMode::begin"));
}

void OnModeClass::modeStart()
{
    DB(F("OnMode::modeStart"));
    _lastTime = "";
    _color = dayColor(ModeRealTime.daySeconds());
    setBrightness(EspApConfigurator[SET_MAX_BRIGHTNESS]->get().toFloat());
}

void OnModeClass::modeStop()
{
    DB(F("OnMode::modeStop"));

    // Save the setting for brightness so it can be picked up in GoToSleepMode
    EspApConfigurator[SET_MAX_BRIGHTNESS]->set(String(_brightnessPercent));
}

void OnModeClass::modeUpdate()
{
    drawClock();
    updateLeds();
}

void OnModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("OnModeClass::pushEvent ms="));
    ModeManager.switchMode(&GoToSleepMode);
}

void OnModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("OnModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
    
    if (diff < 0) {
        setBrightness(_brightnessPercent * 0.9);
    } else {
        setBrightness(_brightnessPercent / 0.9);
    }
}

void OnModeClass::drawClock()
{
    String t = ModeRealTime.timeStr();   
    if (t != _lastTime) {
        DBLN(F("OnMode::drawClock updating clock display"));
        _lastTime = t;
        CbOledDisplay.clearBuffer();
        CbOledDisplay.drawText(t.c_str(), 'C', 'M');
        CbOledDisplay.sendBuffer();
    }
}

void OnModeClass::updateLeds()
{
    if (!_updateBrightness) {
        return;
    }

    DB(F("OnModeClass::updateLeds new % ="));
    DBLN(_brightnessPercent, 1);
    uint32_t col = fadeColor(_color, _brightnessPercent);
    if (col == 0x000000) {
        DBLN(F("Brighness set to 0 - reverting to StandbyMode"));
        ModeManager.switchMode(&StandbyMode);
    }

    for(uint16_t i=0; i<RGBLED_COUNT; i++) {
        CbLeds.setPixelColor(i, col);
    }
    CbLeds.show();
    _updateBrightness = false;
}

void OnModeClass::setBrightness(float percent)
{
    _brightnessPercent = percent;
    if (_brightnessPercent > 100) {
        _brightnessPercent = 100;
    }
    if (_brightnessPercent < 0) {
        _brightnessPercent = 0;
    }
    _updateBrightness = true;
}



