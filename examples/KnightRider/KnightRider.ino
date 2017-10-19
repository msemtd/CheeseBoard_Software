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
#include <CbLeds.h>
#include <CbRotaryInput.h>

// Include files from this example
#include "KnightRiderEffect.h"

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d\n", diff, value);
    if (CbRotaryInput.buttonPushed()) {
        // If we push and hold while twisting, adjust the brightness
        if (diff > 0) {
            KnightRiderEffect.brighter();
        } else {
            KnightRiderEffect.dimmer();
        }
    } else {
        // Otherwise, adjust the frequency of the effect
        if (diff > 0) {
            KnightRiderEffect.setFrequency(KnightRiderEffect.getFrequency() * 1.15);
        } else {
            KnightRiderEffect.setFrequency(KnightRiderEffect.getFrequency() / 1.15);
        }
    }
}

void buttonCb(void)
{
    DBLN("buttonCb() - changing color");
    KnightRiderEffect.changeColor();
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN(F("S:setup"));

    // Init CbLeds
    CbLeds.begin();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb, rotaryCb);

    // Start initialize the KnightRiderEffect
    KnightRiderEffect.begin();

    DBLN(F("E:setup"));
}

void loop()
{
    KnightRiderEffect.update();
    CbRotaryInput.update();
}

