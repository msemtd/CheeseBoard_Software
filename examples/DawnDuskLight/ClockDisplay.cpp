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
    _lastUpdate(0)
{
}

void ClockDisplayClass::begin()
{
}

void ClockDisplayClass::update()
{
    if (Millis() > _lastUpdate + RefreshMs) {
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

        if (_updated) {
            // clear update flag
            _updated = false;

            CbOledDisplay.clearBuffer();
            GfxTextBox2 timeBox(timeStr, TimeFont, false, 0);
            GfxTextBox2 dateBox(RealTimeClock.dateStr(), GfxDefaultFont, false, 0);
            timeBox.draw(64 - (timeBox.width()/2), 0);
            dateBox.draw(64 - (dateBox.width()/2), timeBox.height() + 3);
            CbOledDisplay.drawText(_modeLine.c_str(), 'C', 'B');
            CbOledDisplay.sendBuffer();
        }
    }
}

