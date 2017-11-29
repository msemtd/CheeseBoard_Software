#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"

/*! Small GfxItem for displaying signal strength as a siny vertical bar graph.
 *
 *  The bar is 2 pixels wide and 12 pixels tall.
 */
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

