#pragma once

#include <Mode.h>

/*! Base class for all modes in the light to inherit
 *
 * This provides an interface to for the push button and rotary 
 * encoder events to get through to the active mode.
 *
 */
class EventMode : public Mode {
public:
    virtual void pushEvent(uint16_t durationMs) {;}
    virtual void twistEvent(int8_t diff, int32_t value) {;}
    virtual void pushTwistEvent(int8_t diff, int32_t value) {;}

};
