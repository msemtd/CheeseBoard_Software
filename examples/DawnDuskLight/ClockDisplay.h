#pragma once

#include <stdint.h>

class ClockDisplayClass {
public:
    //! Minimum time between Oled refreshes
    const static uint16_t RefreshMs = 200;
public:
    ClockDisplayClass();

    void begin();
    void update();

    String modeLine();
    void setModeLine(const String& s) { _modeLine = s; _updated = true; }
    void enable() { _enabled = true; }
    void disable() { _enabled = false; }

private:
    bool _enabled;
    String _modeLine;
    bool _updated;
    String _lastTimeStr;
    uint32_t _lastUpdate;

};

// Global object 
extern ClockDisplayClass ClockDisplay;

