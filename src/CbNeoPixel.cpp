#include "CbNeoPixel.h"
#include "Millis.h"

CbNeoPixel::CbNeoPixel(uint16_t n, uint8_t p, neoPixelType t) :
    Adafruit_NeoPixel(n, p, t)
{
}

void CbNeoPixel::show()
{
    Adafruit_NeoPixel::show();
    addMillisOffset(numPixels() * 0.0215);
}



