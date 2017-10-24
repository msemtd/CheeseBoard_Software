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
    _nightMode(false),
    _lastNightOn(0)
{
}

void ClockDisplayClass::begin()
{
}

void ClockDisplayClass::update()
{
    // When not enabled, do nothing
    if (!_enabled) { return; }

    if (Millis() > _lastUpdate + RefreshMs || _updated) {
        _lastUpdate = Millis();

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

        if (_updated || (_nightMode && Millis() > _lastNightOn + NightModeActiveMs && !_blanked)) {
            // clear update flag
            _updated = false;

            DBLN(F("ClockDisplayClass::update redraw"));

            CbOledDisplay.clear();
            // Only draw something if we're not in night mode, or if there has been recent activity, else leave blank
            if (!_nightMode || Millis() <= _lastNightOn + NightModeActiveMs) {
                // Draw the time & date
                GfxTextBox2 timeBox(timeStr, TimeFont, false, 0);
                GfxTextBox2 dateBox(RealTimeClock.dateStr(), GfxDefaultFont, false, 0);
                timeBox.draw(64 - (timeBox.width()/2), 0);
                dateBox.draw(64 - (dateBox.width()/2), timeBox.height());

                if (_nightMode) {
                    GfxTextBox2 statusBox("[night mode]", GfxDefaultFont, false, 0);
                    statusBox.draw(64 - (statusBox.width()/2), timeBox.height()+dateBox.height());
                }

                // Draw the modeLine
                CbOledDisplay.drawText(_modeLine.c_str(), 'C', 'B');
                _blanked = false;
            } else {
                _blanked = true;
            }
            CbOledDisplay.show();
        }
    }
}

void ClockDisplayClass::setNightMode(bool on) 
{ 
    DB(F("setNightMode on="));
    DBLN(on);
    _nightMode = on; 
    _updated = true;
    if (on) {
        _lastNightOn = Millis(); 
    } else {
        _lastNightOn = 0;
    }
}

bool ClockDisplayClass::nightModeWake()
{
    DB(F("nightModeWake "));
    if (!_nightMode) {
        DBLN(F("nm:0 wake=0"));
        return false;
    }
    else if (Millis() > _lastNightOn + NightModeActiveMs) {
        // night mode is active, and the screen is blank - wake it up!
        _lastNightOn = Millis();
        _updated = true;
        DBLN(F("nm=1 wake=1"));
        return true;
    } else {
        // night mode is active, but the screen was already on - just
        // refresh that timer
        _lastNightOn = Millis();
        DBLN(F("nm=1 wake=0"));
        return false;
    }
}



