#include <CbMillis.h>
#include <CbDebug.h>

#include "KnightRiderEffect.h"
#include "CbLeds.h"

KnightRiderEffectClass KnightRiderEffect;

KnightRiderEffectClass::KnightRiderEffectClass()
{
}

void KnightRiderEffectClass::begin(float defaultFrequency, float minFrequency, float maxFrequency, uint16_t fadeMs, float fadeRatio)
{
    _frequency = defaultFrequency;
    _minFrequency = minFrequency;
    _maxFrequency = maxFrequency;
    _fadeMs = fadeMs;
    _fadeRatio = fadeRatio;
    _lastOn = 0;
    _lastFade = 0;
    _nextLED = 0;
    _goingRight = true;
    _color = 0;
    _brightness = 128;
    CbLeds.setBrightness(_brightness);
}

void KnightRiderEffectClass::update()
{
    uint32_t now = CbMillis();

    // If it's time, dim any CbLeds which are on at all
    if (now > _lastFade + _fadeMs) {
        _lastFade = now;
        fader();
    }

    // Work out if we need to illuminate a new LED
    if (now > _lastOn + (1000/(_frequency*((2*CbLeds.numPixels())-1)))) {
        _lastOn = now;
        nextOn();
    }
}

void KnightRiderEffectClass::setFrequency(float f)
{
    if (f > _maxFrequency) {
        _frequency = _maxFrequency;
    } else if (f < _minFrequency) {
        _frequency = _minFrequency;
    } else {
        _frequency = f;
    }
}

void KnightRiderEffectClass::changeColor()
{
    _color = (_color+1) % 7;
    DBF("color now: %d\n", _color);
}

void KnightRiderEffectClass::dimmer()
{
    float f = _brightness;
    f *= _fadeRatio;
    _brightness = (uint8_t)f;
    CbLeds.setBrightness(_brightness);
}

void KnightRiderEffectClass::brighter()
{
    float f = _brightness;
    f /= _fadeRatio;
    if (f > 250) {
        _brightness = 255;
    } else if (f < 1.) {
        _brightness = 22;
    } else {
        _brightness = (uint8_t)f;
    }
    CbLeds.setBrightness(_brightness);
}

void KnightRiderEffectClass::fader()
{
    for (uint16_t i=0; i<CbLeds.numPixels(); i++) { 
        uint32_t pixelColor = CbLeds.getPixelColor(i);
        uint8_t r = fadeColor(pixelColor >> 16);
        uint8_t g = fadeColor((pixelColor >> 8) % 256);
        uint8_t b = fadeColor(pixelColor % 256);
        CbLeds.setPixelColor(i, r, g, b);
    }
    CbLeds.show();
}

void KnightRiderEffectClass::nextOn()
{
    switch (_color) {
    case 0:
        CbLeds.setPixelColor(_nextLED, 255, 0, 0);
        break;
    case 1:
        CbLeds.setPixelColor(_nextLED, 0, 255, 0);
        break;
    case 2:
        CbLeds.setPixelColor(_nextLED, 0, 0, 255);
        break;
    case 3:
        CbLeds.setPixelColor(_nextLED, 255, 255, 0);
        break;
    case 4:
        CbLeds.setPixelColor(_nextLED, 255, 0, 255);
        break;
    case 5:
        CbLeds.setPixelColor(_nextLED, 0, 255, 255);
        break;
    case 6:
        CbLeds.setPixelColor(_nextLED, 255, 255, 255);
        break;
    }

    if (_goingRight) {
        if (_nextLED == CbLeds.numPixels()-1) {
            _goingRight = false;
            _nextLED--;
        } else {
            _nextLED++;
        }
    } else {
        if (_nextLED == 0) {
            _goingRight = true;
            _nextLED++;
        } else {
            _nextLED--;
        }
    }
}

uint8_t KnightRiderEffectClass::fadeColor(uint8_t c)
{
    float f = c;
    f *= _fadeRatio;
    return (uint8_t)f;
}

