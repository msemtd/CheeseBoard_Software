#pragma once

#include <Arduino.h>
#include <Mode.h>
#include <stdint.h>

/*! \class StandbyModeClass
 * 
 * This mode is active when the device off - it fades out the LEDs if they are on
 * and displays the time on the OLED display.
 *
 */

class StandbyModeClass : public Mode {
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

    void drawClock();

    void fadeLed();

protected:
    uint8_t fadeChannel(uint8_t c);

private:
    String _lastTime;
    bool _fade;
    uint32_t _lastFade;

};

extern StandbyModeClass StandbyMode;


