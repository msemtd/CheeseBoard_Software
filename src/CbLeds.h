#pragma once

#include <Adafruit_NeoPixel.h>

/*! \brief Wrapper around Adafruit_NeoPixel with Millis() correction
 *
 *  When the Adafruit_NeoPixel.show() member is called interrupts are 
 *  temporarily disabled to allow the precise timing required when
 *  sending data out to the LEDs. This has a few side-effects,
 *  including a drift in the timer which is used by millis().
 *
 *  This is usually a small effect, but when LEDs are updated very
 *  frequently the offset can add up and become noticable. In one 
 *  application, the author noticed the value of millis being 50% 
 *  slower than real time!
 *
 *  This class is a simple sub-class of the Adafruit_NeoPixel class
 *  which estimates the offset to the millis() timer, and updates the
 *  offset for the Millis() call.
 */
class CbNeoPixel : public Adafruit_NeoPixel {
public:
    /*! Constructor.
     *
     *  Parameter names kept the same as the Adafruit_NeoPixel docs for
     *  consistency.
     *
     *  \param n The number of LEDs in the strip.
     *  \param p The pin for the LED data line.
     *  \param t RGB LED type.
     */
    CbNeoPixel(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB+NEO_KHZ800);

    /*! Send updated pixel state to LEDs.
     *  
     * This calls Adafruit_NeoPixel.show() and then makes an adjustment to
     * the Millis() offset, so that Millis() returns about the right time.
     */
    void show(void);

};

/*! Global instance of CbNeoPixel.
 *
 *  Parameters to the constructor are taken from RGBLED_COUNT, 
 *  RGBLED_DATA_PIN, and RGBLED_TYPE globals, which are defined in the 
 *  configuration file for a specific board, and should be included via
 *  CheeseboardConfig.h
 */
extern CbNeoPixel CbLeds;

