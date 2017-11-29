#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"
#include "GfxFont.h"

class GfxTextBox2 : public GfxItem {
public:
    /*! Constructor.
     *
     * \param initialText the initial text in the box
     * \param font the font to be used. If NULL, the default font will be used
     * \param border whether or not to draw the border around the text box
     * \param padding how many pixels padding between the border and the font
     * \param justify 'L', 'C', or 'R' (left, center, right)
     * \param width the width in pixels.  If 0, determine width from font/text
     * \param height the height in pixels.  If 0, determine width from font/text
     */
    GfxTextBox2(String initialText, GfxFont& font=GfxDefaultFont, bool border=true, uint8_t padding=1, char justify='C', uint16_t width=0, uint16_t height=0);

    /*! Draw it.
     *
     * Draws the bext box, starting at the top left corner specified by (xOffset,yOffset)
     */
    void draw(uint16_t xOffset=0, uint16_t yOffset=0);

    /*! Get the width of the text box.
     */
    uint16_t width();

    /*! Get the height of the text box.
     */
    uint16_t height();

    /*! Get the text in the text box.
     */
    const String& text();

    /*! Update the text in the text box.
     */
    void setText(const String& newText);

protected:
    String _text;
    GfxFont& _font;
    bool _border;
    uint8_t _padding;
    bool _justify;
    uint16_t _width;
    uint16_t _height;

};

