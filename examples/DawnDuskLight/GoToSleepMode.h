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
    void rotaryEvent(int8_t diff, int32_t value);
    void buttonEvent(uint16_t durationMs);

    //! Let parent mode know we have competed our duties
    bool isFinished() { return _finished; }

    //! The rotary encoder controls a temporary value in the mode here
    void setFadeTime(uint8_t minutes);

    void drawClock();
    void fadeLed();

protected:
    float getFadePercent();

private:
    uint32_t _fadeStartUnixTime;
    String _lastTime;
    bool _fade;
    uint32_t _lastFade;
    uint8_t _fadeMinutes;
    bool _finished;

};

extern GoToSleepModeClass GoToSleepMode;


