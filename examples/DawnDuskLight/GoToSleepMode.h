#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "EventMode.h"

/*! \class GoToSleepModeClass
 * 
 * This mode is active when the user is going to sleep
 *
 * The sleep timer is set from the persistent setting SET_SLEEP_DURATION, although the
 * rotary dial can be twisted to alter this time.  If the rotary dial is twisted, the
 * timer starts again.
 *
 * The time is drawn on the OLED display until the timer has expired
 *
 * If the user presses the rotary button, the mode will end immediately.
 */

class GoToSleepModeClass : public EventMode {
public:
    // How often we update the LEDs.  Levels = 256, min time is 1 minute, so 
    // should be at most 60s/256 = 234ms
    // const uint16_t FadeMs = 200;
    const uint16_t FadeMs = 1000;

    // How close to an integer is an integer?
    const uint8_t FloatTolerance = 0.001;

public:
    GoToSleepModeClass();

    //! Initialization
    void begin();

    //! Called when this mode is activated
    void modeStart();

    //! Called when this mode is de-activated
    void modeStop();

    //! Called periodically while mode is active
    void modeUpdate();

    //! Handle incoming events from the RotaryInput
    void pushEvent(uint16_t durationMs);
    void twistEvent(int8_t diff, int32_t value);
    void pushTwistEvent(int8_t diff, int32_t value);

    //! Let parent mode know we have competed our duties
    bool isFinished() { return _finished; }

    //! Change the fade duration setting
    void adjustFadeDuration(int8_t minutes);

    //! Save fade duration to EEPROM
    void saveFadeDuration();

    //! (re-)apply the new value of fade duration to the fader - starts counter again
    void applyFadeDuration();

    void fadeLed();

protected:
    float getFadePercent();
    void updateModeLine();

private:
    uint32_t _fadeStartUnixTime;
    bool _fade;
    uint32_t _lastFade;
    uint8_t _fadeMinutes;
    bool _finished;
    uint32_t _lastModeLineUpdate;
    String _lastModeLine;
    uint32_t _lastFadeChange;

};

extern GoToSleepModeClass GoToSleepMode;


