#pragma once

class GfxItem {
public:
    virtual void draw(uint16_t xOffset=0, uint16_t yOffset=0) = 0;
    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;

};

