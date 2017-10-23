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
#include <GfxTextBox.h>

void setup()
{
    Serial.begin(115200);
    DBLN(F("S:setup"));

    CbOledDisplay.begin();
    CbOledDisplay.clear();
    GfxTextBox tb(64, "Bananas", true);
    tb.draw(32, 32);
    CbOledDisplay.show();

    DBLN(F("E:setup"));
}

void loop()
{
}


