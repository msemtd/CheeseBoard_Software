#pragma once

/*! Base class for other Gfx objects.
 */
class GfxItem {
public:
    /*! Draw the object on the display (don't send buffer).
     */
    virtual void draw(uint16_t xOffset=0, uint16_t yOffset=0) = 0;

    /*! Get object width.
     *
     *  \return Object width in pixels.
     */
    virtual uint16_t width() = 0;

    /*! Get object height.
     *
     *  \return Object height in pixels.
     */
    virtual uint16_t height() = 0;

};

