#include <CbDebug.h>
#include "GfxTextBox.h"
#include "CbOled.h"

GfxTextBox::GfxTextBox(uint16_t width, String text, bool border) : 
    _width(width),
    _text(text),
    _border(border)
{
}

void GfxTextBox::draw(uint16_t xOffset, uint16_t yOffset) {
    DBF("GfxText draw@%d,%d : text=%s\n", xOffset, yOffset, _text.c_str());
    CbOled.setFont(CBOLED_MESSAGE_FONT);
    uint16_t strLenPixels;
    uint16_t useLen = _text.length();

    do {
        strLenPixels = CbOled.getStrWidth(_text.substring(0, useLen).c_str()); 
        strLenPixels + 4 > _width && useLen > 0;
    } while (strLenPixels + 2 > _width && useLen-- > 0);
    CbOled.drawStr(xOffset+2, 
                yOffset+CBOLED_MESSAGE_FONT_VSEP+CBOLED_MESSAGE_FONT_HEIGHT, 
                _text.substring(0, useLen).c_str());
    if (_border) {
        CbOled.drawFrame(xOffset,
                    yOffset,
                    width(),
                    height());
    }
}

uint16_t GfxTextBox::width()
{
    return _width;
}

uint16_t GfxTextBox::height() {
    return CBOLED_MESSAGE_FONT_HEIGHT + (2*CBOLED_MESSAGE_FONT_VSEP);
}

