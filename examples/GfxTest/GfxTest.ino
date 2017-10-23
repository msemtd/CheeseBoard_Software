#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Encoder.h>

// Include CheeseBoard headers for the components we'll be using in
// this example
#include <MutilaDebug.h>
#include <CbOledDisplay.h>
#include <GfxTextBox2.h>
#include <GfxFont.h>

GfxFont font14(u8g2_font_helvR14_tf, 18);
GfxFont font18(u8g2_font_helvR18_tf, 24);

void setup()
{
    Serial.begin(115200);
    delay(150);
    DBLN(F("\n\nS:setup"));

    CbOledDisplay.begin();
    CbOledDisplay.clear();
    GfxTextBox2 no("Nop");
    GfxTextBox2 maybe("Mayb", font14);
    GfxTextBox2 yes("yES!", font18);
    uint16_t x = 0;
    uint16_t y = 0;
    no.draw(x, y);
    x += no.width();
    y += no.height();
    maybe.draw(x, y);
    x += maybe.width();
    y += maybe.height();
    yes.draw(x, y);
    CbOledDisplay.show();

    DBLN(F("E:setup"));
}

void loop()
{
}


