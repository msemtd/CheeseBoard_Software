#pragma once

#include <U8g2lib.h>
#include "CheeseboardConfig.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*! Enhanced OLED class.
 *
 *  This class is derived from a U8G2lib display class, adding a few extra features like wrapped text display.
 */
class CbOledDisplayClass : public CBOLED_CLASS {
public:
    /*! Constructor.
     *
     *  Parameters are the same as U8G2lib class parameters - see the U8G2lib documentation for details (yeah 
     *  right!).
     *
     *  \param rotation
     *  \param clock
     *  \param data
     *  \param reset
     */
    CbOledDisplayClass(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset=U8X8_PIN_NONE);

    /*! Initialization. 
     */
    void begin();

    /*! Get the width of a specific character.
     *  
     *  For some reason this didn't seem possible in the base class. Please tell me how if I'm missing 
     *  something.
     */
    u8g2_uint_t getCharWidth(char c);

    /*! Draw a string, right justified.
     *  
     *  \param x the x position of the right-most pixel of the text.
     *  \param y the bottom most pixel of the text.
     *  \param s the string to draw.
     */
    u8g2_uint_t drawStrR(u8g2_uint_t x, u8g2_uint_t y, const char *s);

    /*! Display text, wrapped, with horizontal and vertical justification
     *
     *  \param text the text to display.
     *  \param hAlign horizontal alignment: 'L' (left) 'C' (center) 'R' (right.
     *  \param vAlign vertical aignment: 'T' (top) 'M' (middle) 'B' (bottom).
     */
    void drawText(const char* text, char hAlign='L', char vAlign='M');

    /*! Alias for clearBuffer() to make interface more consistent with CbLeds.
     */

    void clear() { this->clearBuffer(); }
    /*! Alias for sendBuffer() to make interface more consistent with CbLeds.
     */
    void show() { this->sendBuffer(); }

};

extern CbOledDisplayClass CbOledDisplay;

