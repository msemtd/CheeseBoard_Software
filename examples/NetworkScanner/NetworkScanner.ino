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

// A global listbox object
GfxSSIDListBox listbox(128);

uint32_t lastDisplayRefresh = 0;
bool displayUpdate = true;

void buttonCb(void)
{
    DBLN("buttonCb");
    displayUpdate = true;
}

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d\n", diff, value);
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

}

