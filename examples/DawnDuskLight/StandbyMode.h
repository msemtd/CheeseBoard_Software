#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "EventMode.h"

/*! \class StandbyModeClass
 * 
 * This mode is active when the device off - it fades out the LEDs if they are on
 * and displays the time on the OLED display.
 *
 */

class StandbyModeClass : public EventMode {
public:
    const uint8_t FadeMs = 30;
    const float FadeRatio = 0.9;

public:
    StandbyModeClass();

    //! Initialization
    void begin();

    //! Called when this mode is activated
    void modeStart();

    //! Called when this mode is de-activated
    void modeStop();

    //! Called periodically while mode is active
    void modeUpdate();

    //! Handle input events
    void pushEvent(uint16_t durationMs);
    void twistEvent(int8_t diff, int32_t value);
    void pushTwistEvent(int8_t diff, int32_t value);

    //! Adjust the wake time
    //! \param minutes the number of minutes to adjust 
    void adjustWakeTime(int8_t minutes);

    //! Save the wake time (if it is different from the value in EEPROM)
    void saveWakeTime();

    void fadeLed();

protected:
    uint8_t fadeChannel(uint8_t c);

private:
    bool _fade;
    uint32_t _lastFade;
    uint32_t _lastWakeChange;
    uint32_t _lastWakeCheck;

};

extern StandbyModeClass StandbyMode;


