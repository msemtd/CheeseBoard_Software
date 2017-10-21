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
    void rotaryEvent(int8_t diff, int32_t value) {;}
    void buttonEvent(uint16_t durationMs) {;}

};
