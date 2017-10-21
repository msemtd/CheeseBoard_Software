#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"

// 2 x 12 signal strength indicator
// draws box between pixels 2 and 11
class GfxSignalStrength : public GfxItem {
public:
    GfxSignalStrength();
    void setSignal(uint8_t percent);
    void draw(uint16_t xOffset=0, uint16_t yOffset=0);
    uint16_t width();
    uint16_t height();

protected:
    String _text;
    uint8_t _signal;

};

