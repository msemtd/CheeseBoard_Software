#pragma once

#include <Encoder.h>
#include <CbDebouncedButton.h>
#include <stdint.h>

//! \class CbRotaryInputClass 
//! Handy wrapper class for the dual functions of the rotary encoder device:
//! 
//! 1. Push button
//! 2. Rotary encoder for left/right type input
//!
//! This class lets a user register callbacks to be triggered when the 
//! rotary encoder is moved and/or the button is pushed.
class CbRotaryInputClass {
public:
    typedef void (*t_buttonCb)();
    typedef void (*t_rotaryCb)(int8_t, int32_t);

    //! Constructor
    //! \param buttonPin the pin to which the button leg of the encoder input is attached
    //! \param aPin the pin to which the A leg of the rotary encoder input is attached
    //! \param bPin the pin to which the B leg of the rotary encoder input is attached
    CbRotaryInputClass(uint8_t buttonPin, uint8_t aPin, uint8_t bPin);

    //! Initializtion
    //! \param buttonCb pointer to function which will be called when the button is pushed
    //! \param rotaryCb pointer to function which will be called when the encoder is moved
    //!
    //! This function is typically called from begin() in sketches
    void begin(t_buttonCb buttonCb=NULL, t_rotaryCb rotaryCb=NULL);

    //! Update the state of the CbRotaryInputClass
    //! Should be called frequently (at least every few ms).
    void update();

    //! Fetch the current position of the rotary encoder
    int32_t getEncoderPosition();

    //! Test if the button is currently pushed
    bool buttonPushed();

private:
    CbDebouncedButton _button;
    Encoder _encoder;
    int32_t _position;
    t_buttonCb _buttonCb;
    t_rotaryCb _rotatyCb;

};

//! A globally available object instance of CbRotaryInputClass
extern CbRotaryInputClass CbRotaryInput;

