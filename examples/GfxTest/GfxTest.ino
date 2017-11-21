#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Encoder.h>
#include <SoftwareSerial.h>

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
    GfxTextBox2 t1("Crypto");
    GfxTextBox2 t2("Crypto", font14, true, 3);   // border=true, padding=3
    GfxTextBox2 t3("Crypto", font18, true, 5);   // border=true, padding=5
    uint16_t x = 0;
    uint16_t y = 0;
    t1.draw(x, y);
    x += t1.width();
    y += t1.height();
    t2.draw(x, y);
    x += t2.width();
    y += t2.height();
    t3.draw(x, y);
    CbOledDisplay.show();

    DBLN(F("E:setup"));
}

void loop()
{
}


