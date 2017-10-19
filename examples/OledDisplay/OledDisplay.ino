// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// Include CheeseBoard headers for the components we'll be using in
// this example
#include <CbDebug.h>
#include <CbOled.h>
#include <GfxSSIDListBox.h>

// A global listbox object
GfxSSIDListBox listbox(128);

void setup()
{
    Serial.begin(115200);
    DBLN(F("S:setup"));

    CbOled.begin();

    listbox.add(GfxNetInfo(String("wirescubedextended"), 1, 75));
    listbox.add(GfxNetInfo(String("leslians"), 3, 53));
    listbox.add(GfxNetInfo(String("OtherNet1"), 7, 13));
    listbox.add(GfxNetInfo(String("OtherNet2"), 1, 15));
    listbox.add(GfxNetInfo(String("OtherNet3"), 8, 25));
    listbox.add(GfxNetInfo(String("OtherNet4"), 9, 36));
    listbox.select(1);

    DBLN(F("E:setup"));
}

void loop()
{
    CbOled.clearBuffer();
    listbox.draw();
    CbOled.sendBuffer();
    delay(500);
}

