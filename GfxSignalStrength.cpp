#include <CbDebug.h>
#include "GfxSignalStrength.h"
#include "CbOled.h"

GfxSignalStrength::GfxSignalStrength() : 
    _signal(0)
{
}

void GfxSignalStrength::draw(uint16_t xOffset, uint16_t yOffset) 
{
    _DBF("GfxSignalStrength draw@%d,%d : signal=%d\n", xOffset, yOffset, _signal);
    uint8_t fullBar = height() - 4;
    uint8_t h = fullBar * _signal / 100;
    
    CbOled.drawVLine(xOffset+3, yOffset+fullBar+2-h, h);
    CbOled.drawVLine(xOffset+4, yOffset+fullBar+2-h, h);
}

uint16_t GfxSignalStrength::width()
{
    return 8;
}

uint16_t GfxSignalStrength::height()
{
    return CBOLED_MESSAGE_FONT_HEIGHT + (2*CBOLED_MESSAGE_FONT_VSEP);
}

void GfxSignalStrength::setSignal(uint8_t percent) 
{
    if (percent > 100) {
        _signal = 100;
    } else {
        _signal = percent;
    }
}
