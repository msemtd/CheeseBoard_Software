#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "EventMode.h"

/*! \class WakeUpModeClass
 * 
 * Will be started SET_WAKE_DURATION before the target wake up time
 *
 * The task is to fade from off -> full brightness over that period, then quit
 * 
 */

class WakeUpModeClass : public EventMode {
public:
    // How often we update the LEDs.  Levels = 256, min time is 1 minute, so 
    // should be at most 60s/256 = 234ms
    // const uint16_t FadeMs = 200;
    const uint16_t FadeMs = 1000;

    // How close to an integer is an integer?
    const uint8_t FloatTolerance = 0.001;

public:
    WakeUpModeClass();

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

    void fadeLed();

protected:
    float getFadePercent();
    void updateModeLine();

private:
    uint32_t _fadeStartUnixTime;
    uint32_t _lastFade;
    uint16_t _fadeSeconds;
    bool _finished;
    uint32_t _lastModeLineUpdate;
    String _lastModeLine;
    bool _postWake;

};

extern WakeUpModeClass WakeUpMode;


