#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "EventMode.h"

/*! \class SetupModeClass
 * 
 * This mode provides a menu-driver setup system
 *
 */

class SetupModeClass : public EventMode {
public:
    SetupModeClass();

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

};

extern SetupModeClass SetupMode;


