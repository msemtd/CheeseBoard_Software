#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>

// Include CheeseBoard headers for the components we'll be using in
// this example
#include <CbDebug.h>
#include <CbMillis.h>
#include <CbOled.h>
#include <GfxSSIDListBox.h>
#include <CbRotaryInput.h>


GfxSSIDListBox listbox(128);        // A global listbox object
uint32_t lastDisplayRefresh = 0;    // Timer to refresh display now and then
bool displayUpdate = true;          // Flag to request immediate display update

void buttonCb(uint16_t durationMs)
{
    DBLN("buttonCb");
    displayUpdate = true;
}

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d", diff, value);
    int16_t i = listbox.selected();
    int16_t n = diff > 0 ? listbox.findNext(i) : listbox.findPreceding(i);
    DBF(" current=%d new=%d\n", i, n);
    if (n != -1) {
        listbox.select(n);
        listbox.scrollTo(n);
    }
    displayUpdate = true;
}

void setup()
{
    Serial.begin(115200);
    DBLN(F("S:setup"));

    CbOled.begin();
    CbRotaryInput.begin(buttonCb, rotaryCb);

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
    CbRotaryInput.update();

    if (CbMillis() > lastDisplayRefresh + 5000 || displayUpdate) {
        CbOled.clearBuffer();
        listbox.draw();
        CbOled.sendBuffer();
        displayUpdate = false;
    }
    DBF("%-12d D8 digitalRead=%d\n", CbMillis(), digitalRead(D8));
}

