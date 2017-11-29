#pragma once

#include <stdint.h>
#include "CbOledDisplay.h"

/*! Font object which knows how tall it is in pixels.
 */
class GfxFont {
public:
    /*! Constructor.
     *
     *  \param font a pointer to a U8g2lib font defined elsewhere
     *  \param fontHeight total height of font in pixels including whatever usually is "below the line"
     */
    GfxFont(const uint8_t* font, uint8_t fontHeight);

    /*! Get height 
     *
     *  \return Font height in pixels.
     */
    uint8_t height();

    /*! Make this font the current font.
     */
    void use();

private:
    const uint8_t* _font;
    uint8_t _height;

};

extern GfxFont GfxDefaultFont;
