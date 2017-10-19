#pragma once

#include <stdint.h>

class KnightRiderEffectClass {
public:
    KnightRiderEffectClass();

    //! Call this once after oinstantiation, before first update() (i.e. in setup())
    //! @param defaultFrequency initial frequency after begin() (Hz)
    //! @param minFrequency the minimum frequency we can set (Hz)
    //! @param maxFrequency the maximum frequency we can set (Hz)
    //! @param fadeMs how long between fading of CbLeds (note: it takes many fades to go from on -> off)
    //! @param fadeRatio how much each fade event fades the CbLeds (lower = quicker)
    //!
    //! The frequency parameters are the number of times per second the entire sequence 
    //! takes - from left to right and back again.
    void begin(float defaultFrequency=1.5, float minFrequency=0.1, float maxFrequency=3.0, uint16_t fadeMs=10, float fadeRadio=0.9);

    //! Call this often to update the state of the effect (i.e. from loop())
    void update();

    //! Get the frequency of the effect - i.e. how many wooshes per second.  Typically between 0.5 and 2.0
    float getFrequency() { return _frequency; }

    //! Set frequency of effect - i.e. how many wooshes per second.  Typically between 0.5 and 2.0
    //! @param f the new frequency
    //!
    //! Note: If you set a frequency outside the permitted range (as defined in the call to begin()),
    //! the value will be clipped - e.g. of you set a value higher than the maximum frequency, the
    //! new value will actually be the maximum permitted value
    void setFrequency(float f);

    //! Changed color (red -> green -> blue -> red)
    void changeColor();

protected:
    // fades all CbLeds one notch
    void fader();

    // turns on the next LED in the sequence
    void nextOn();

    // fades a color channel in a non-linear way which looks nice to the eye
    uint8_t fadeColor(uint8_t c);

private:
    float _frequency;
    float _minFrequency;
    float _maxFrequency;
    uint16_t _fadeMs;
    float _fadeRatio;
    uint32_t _lastOn;
    uint32_t _lastFade;
    uint16_t _nextLED;
    bool _goingRight;
    uint8_t _color;

};

extern KnightRiderEffectClass KnightRiderEffect;

