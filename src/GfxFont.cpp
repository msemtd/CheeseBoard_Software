#include "GfxFont.h"

GfxFont GfxDefaultFont(u8g2_font_helvR08_tf, 10);

GfxFont::GfxFont(const uint8_t* font, uint8_t fontHeight) : 
    _font(font), 
    _height(fontHeight)
{
}

uint8_t GfxFont::height()
{
    return _height;
}

void GfxFont::use() {
    CbOledDisplay.setFont(_font);
}

