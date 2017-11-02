#include <Millis.h>
#include "CbLeds.h"
#include "CheeseboardConfig.h"

CbNeoPixel CbLeds(RGBLED_COUNT, RGBLED_DATA_PIN, RGBLED_TYPE);

CbNeoPixel::CbNeoPixel(uint16_t n, uint8_t p, neoPixelType t) :
    Adafruit_NeoPixel(n, p, t)
{
}

void CbNeoPixel::show()
{
    Adafruit_NeoPixel::show();
    addMillisOffset(numPixels() * 0.0215);
}




