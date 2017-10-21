#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"

class GfxTextBox : public GfxItem {
public:
    GfxTextBox(uint16_t width, String text, bool border=false);
    void draw(uint16_t xOffset=0, uint16_t yOffset=0);
    uint16_t width();
    uint16_t height();

protected:
    uint16_t _width;
    String _text;
    bool _border;

};

