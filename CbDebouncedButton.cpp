#include <Arduino.h>
#include "CbDebouncedButton.h"
#include "CbMillis.h"

CbDebouncedButton::CbDebouncedButton(uint8_t pin, bool pullup) :
    CbRawButton(pin, pullup)
{
}

void CbDebouncedButton::begin(uint8_t threshold, uint8_t delay)
{
    CbRawButton::begin();
    _threshold = threshold;
    _delay = delay;
    _lastUpdate = 0;
    setState(false);
    _lastOnDuration = 0;
}

void CbDebouncedButton::update()
{
    if (CbMillis() > _lastUpdate + _delay) {
        if (CbRawButton::on() != _state) {
            _counter++;
            if (_counter > _threshold) {
                setState(!_state);
            }
        } else if (_counter > 0) {
            _counter = 0;
        }
        _lastUpdate = CbMillis();
    }
}

// over-rides CbRawButton::on
bool CbDebouncedButton::on()
{
    return _state;
}

bool CbDebouncedButton::pushed()
{
    bool r = _pushed;
    _pushed = false;
    return r;
}

unsigned long CbDebouncedButton::tapped()
{
    unsigned long r = _lastOnDuration;
    _lastOnDuration = 0;
    return r;
}

bool CbDebouncedButton::held(uint16_t ms)
{
    return (on() && CbMillis() > _lastStateChange + ms);
}

bool CbDebouncedButton::repeat(uint16_t initialMs, uint16_t repeatMs)
{
    bool r = on() && CbMillis() > _nextRepeatTime;
    if (r) {
        if (_repeatCount++ == 0) {
            _nextRepeatTime += initialMs;
        } else {
            _nextRepeatTime += repeatMs;
        }
    }
    return r;
}

void CbDebouncedButton::setState(bool newState)
{
    unsigned long now = CbMillis();
    if (newState) { 
        _pushed = true;
        _repeatCount = 0;
        _nextRepeatTime = now;
    } else {
        _lastOnDuration = now - _lastStateChange;
    }
    if (_state!=newState) {
        _lastStateChange = now;
        _state = newState;
        _counter = 0;
    }
}

