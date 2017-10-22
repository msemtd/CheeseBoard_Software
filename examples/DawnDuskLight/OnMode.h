#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "EventMode.h"

/*! \class OnModeClass
 * 
 * Turns on the light, allows for adjusting of brightness, click to switch
 * into GoToSleepMode (with the current brighness).
 *
 */

class OnModeClass : public EventMode {
public:
    OnModeClass();

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

protected:
    void setBrightness(float percent);

private:
    uint32_t _color;
    float _brightnessPercent;
    bool _updateBrightness;

};

extern OnModeClass OnMode;


