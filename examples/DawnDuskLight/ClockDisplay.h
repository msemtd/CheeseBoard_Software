#pragma once

#include <stdint.h>
#include <GfxFont.h>

class ClockDisplayClass {
public:
    //! Minimum time between Oled refreshes
    const static uint16_t RefreshMs = 200;
    const static uint16_t NightModeActiveMs = 3000;

public:
    //! Constructor
    ClockDisplayClass();

    //! Initialization
    void begin();

    //! Update
    //! Call frequently
    //! \param forceDraq if true (and ClockDisplay is enabled), an immediate redraw will occur)
    void update(bool forceDraw=false);

    //! Set modeLine
    //! \param s the new text
    void setModeLine(const String& s) { _modeLine = s; _updated = true; }

    //! Enable the ClockDisplay
    //! When enabled, updates will draw the clock on the OLED display
    void enable() { _enabled = true; update(true); }

    //! When disabled, updates will not draw to the OLED display
    void disable() { _enabled = false; }

    //! Enable/disable Night Mode
    void setNightMode(bool on);

    //! \return true if was is in night mode, and display was temporarily activated
    bool nightModeWake();

private:
    bool _enabled;
    String _modeLine;
    bool _updated;
    String _lastTimeStr;
    uint32_t _lastUpdate;
    bool _nightMode;
    uint32_t _lastNightOn;

};

// Global object 
extern ClockDisplayClass ClockDisplay;

