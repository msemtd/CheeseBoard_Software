#pragma once

#include <stdint.h>
#include <GfxFont.h>

class ClockDisplayClass {
public:
    //! Minimum time between Oled refreshes
    const static uint16_t RefreshMs = 1000;          //!< How often to check for time updates
    const static uint16_t NightModeActiveMs = 5000;  //!< How long after activity before blanking OLED in night mode

public:
    //! Constructor
    ClockDisplayClass();

    //! Initialization
    void begin();

    //! Update
    //! Call frequently, will decide if OLED needs to be re-drawn, and do it if required
    void update();

    //! Set modeLine
    //! \param s the new text
    void setModeLine(const String& s) { _modeLine = s; _updated = true; }

    //! Enable the ClockDisplay
    //! When enabled, updates will draw the clock on the OLED display
    void enable() { _enabled = true; _updated = true; }

    //! When disabled, updates will not draw to the OLED display
    void disable() { _enabled = false; }

    //! Find out if night mode is active
    bool nightMode() { return _nightMode; }

    //! Enable/disable Night Mode
    void setNightMode(bool on);

    //! Wakes up from night more, activating the display for NightModeActiveMs
    //! \return true if in night mode, and display was temporarily activated
    //!         false if not in night mode, or the display was already active
    bool nightModeWake();

private:
    bool _enabled;          //!< flag to enable/disable all activity
    String _modeLine;       //!< text on bottom line of display
    bool _updated;          //!< flag to trigger immediate re-draw (e.g. if modeLine is updated)
    String _lastTimeStr;    //!< previous time value (used to trigger redraws when time updates)
    uint32_t _lastUpdate;   //!< for thottling re-draw decisions
    bool _nightMode;        //!< flag for night mode
    uint32_t _lastNightOn;  //!< timer for night mode screen blanking
    bool _blanked;          //!< true if last display blanked the screen

};

// Global object 
extern ClockDisplayClass ClockDisplay;

