#pragma once

#include <Encoder.h>
#include <DebouncedButton.h>
#include <stdint.h>

/*! CbRotaryInputClass.
 *
 *  Handy wrapper class for the dual functions of the rotary encoder device:
 *  
 *  1. Push button
 *  2. Rotary encoder for left/right type input
 * 
 *  This class lets a user register callbacks to be triggered when the 
 *  rotary encoder is moved and/or the button is pushed.
 */

class CbRotaryInputClass {
public:
    /*! A function pointer to a fuction which can be used as a callback for button presses.
     *
     *
     *  The parameter to the function is the number of milliseconds the button was pushed for.
     */
    typedef void (*t_buttonCb)(uint16_t);

    /*! A function pointer to a fuction which can be used as a callback for rotary movement events.
     *
     *  The parameters to the function are:
     *
     *  \param int8_t the direction of turn. -1 mean widdershins, +1 means anti-widdershins.
     *  \param int32_t the current position of the rotary dial. It starts at 0 and accumulates.
     */
    typedef void (*t_rotaryCb)(int8_t, int32_t);

    /*! Constructor.
     *
     *  \param buttonPin the pin to which the button leg of the encoder input is attached.
     *  \param aPin the pin to which the A leg of the rotary encoder input is attached.
     *  \param bPin the pin to which the B leg of the rotary encoder input is attached.
     */
    CbRotaryInputClass(uint8_t buttonPin, uint8_t aPin, uint8_t bPin);

    /*! Initializtion.
     *
     *  \param buttonCb pointer to function which will be called when the button is pushed.
     *  \param rotaryCb pointer to function which will be called when the encoder is moved.
     * 
     *  This function is typically called from begin() in sketches.
     */
    void begin(t_buttonCb buttonCb=NULL, t_rotaryCb rotaryCb=NULL);

    /*! Update the state of the CbRotaryInputClass.
     *
     *  Should be called frequently (at least every few ms), typically from loop().
     */
    void update();

    /*! Fetch the current position of the rotary encoder
     *
     *  \return The position of the encoder. After instantiation of the CbRotaryInputClass,
     *          the position is 0.
     */
    int32_t getEncoderPosition();

    /*! Test if the button is currently pushed.
     *
     * \return true if the button is push at the time of calling.
     */
    bool buttonPushed();

private:
    DebouncedButton _button;
    Encoder _encoder;
    int32_t _position;
    t_buttonCb _buttonCb;
    t_rotaryCb _rotatyCb;

};

//! A globally available object instance of CbRotaryInputClass
extern CbRotaryInputClass CbRotaryInput;

