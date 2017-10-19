#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <ESP8266WiFi.h>

// Include CheeseBoard headers for the components we'll be using in
// this example
#include <CbDebug.h>
#include <CbMillis.h>
#include <CbOled.h>
#include <GfxSSIDListBox.h>
#include <CbRotaryInput.h>

#include "Scanner.h"


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

int rssiToQuality(int RSSI) {
    int quality = 0;

    if (RSSI <= -100) {
        quality = 0;
    } else if (RSSI >= -50) {
        quality = 100;
    } else {
        quality = 2 * (RSSI + 100);
    }   
    return quality;
}

void scanCb(int16_t numNetworks)
{
    DBF("scanCb number of networks found=%d:\n", numNetworks);
    for (int16_t i=0; i<numNetworks; i++) {
        DBF("%d: %s, Ch:%d Qu:%d %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), rssiToQuality(WiFi.RSSI(i)), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "secure");
        listbox.update(WiFi.SSID(i), WiFi.channel(i), rssiToQuality(WiFi.RSSI(i)));
    }
    Scanner.startScan();
}

void updateDisplay()
{
    if (CbMillis() > lastDisplayRefresh + 3000 || displayUpdate) {
        lastDisplayRefresh = CbMillis();
        displayUpdate = false;

        CbOled.clearBuffer();
        if (listbox.count() == 0) { 
            CbOled.drawText("no networks found\nscanning...", 'C', 'M');
        } else {
            if (listbox.selected() == -1) {
                listbox.select(0);
            }
            listbox.draw();
        }
        CbOled.sendBuffer();
    }
}
void setup()
{
    Serial.begin(115200);
    DBLN(F("S:setup"));

    CbOled.begin();
    CbRotaryInput.begin(buttonCb, rotaryCb);
    Scanner.begin(scanCb);

    Scanner.startScan();
    DBLN(F("E:setup"));
}

void loop()
{
    CbRotaryInput.update();
    Scanner.update();
    updateDisplay();
}


