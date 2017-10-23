#include <CbOledDisplay.h>
#include <RealTimeClock.h>
#include <EspApConfigurator.h>
#include <Millis.h>
#include <GfxTextBox2.h>
#include <U8g2lib.h>

#include "ClockDisplay.h"
#include "Config.h"


ClockDisplayClass ClockDisplay;

GfxFont TimeFont = GfxFont(u8g2_font_helvR18_tf, 24);

ClockDisplayClass::ClockDisplayClass() :
    _enabled(true),
    _modeLine(""),
    _updated(true),
    _lastTimeStr(""),
    _lastUpdate(0),
    _lastNightOn(0) // default to night mode off
{
}

void ClockDisplayClass::begin()
{
}

void ClockDisplayClass::update(bool forceDraw)
{
    if (Millis() > _lastUpdate + RefreshMs || forceDraw) {
        _lastUpdate = Millis();

        if (!_enabled) { return; }

        String timeStr;

        if (RealTimeClock.haveRealTime()) {
            timeStr = RealTimeClock.timeStr(EspApConfigurator[SET_SHOW_SECONDS]->get().toInt());
        } else { 
            timeStr = F("--:--");
            if (EspApConfigurator[SET_SHOW_SECONDS]->get().toInt()) { timeStr += F(":--"); }
        }
        if (timeStr != _lastTimeStr) {
            _updated = true;
            _lastTimeStr = timeStr;
        }

        if (_updated || forceDraw) {
            // clear update flag
            _updated = false;

            CbOledDisplay.clearBuffer();
            if (!_nightMode || Millis() < _lastNightOn + NightModeActiveMs) {
                GfxTextBox2 timeBox(timeStr, TimeFont, false, 0);
                GfxTextBox2 dateBox(RealTimeClock.dateStr(), GfxDefaultFont, false, 0);
                timeBox.draw(64 - (timeBox.width()/2), 0);
                dateBox.draw(64 - (dateBox.width()/2), timeBox.height() + 3);
                CbOledDisplay.drawText(_modeLine.c_str(), 'C', 'B');
            } 
            CbOledDisplay.sendBuffer();
        }
    }
}

void ClockDisplayClass::setNightMode(bool on) 
{ 
    DB(F("setNightMode on="));
    DBLN(on);
    _nightMode = on; 
    _lastNightOn = Millis(); 
}

bool ClockDisplayClass::nightModeWake()
{
    DB(F("nightModeWake"));
    if (_nightMode && Millis() > _lastNightOn + NightModeActiveMs) {
        _lastNightOn = Millis();
        update(true);
        return true;
    }
    return false;
}



