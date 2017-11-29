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
#include <SoftwareSerial.h>

// Some includes from the Mutila library which is used by the 
// CheeseBoard library and others which I use 
#include <MutilaDebug.h>
#include <Millis.h>

// Include CheeseBoard headers for the components we'll be using in
// this example
#include <CbLeds.h>
#include <CbOledDisplay.h>
#include <CbRotaryInput.h>
#include <CbHC12.h>

#include "EspID.h"
#include "Config.h"

// Function prototypes.  For the Arduino IDE you don't need these, but they 
// make it possible to build with the Makefile approach without having to put
// functions in the file before they are referenced by other functions.
void setup();
void loop();
void buttonCb(uint16_t durationMs);
void display();
void rotaryCb(int8_t diff, int32_t value);

// Global variables
bool HC12Status = false;
bool PreviousButton = false;
int32_t RotaryValue = 0;
uint32_t LastSendHC12 = 0;

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN(F("\n\nCheeseBoard Test Firmware\nLEDs should be on (dim -> brighter)\nOLED should show button status, rotary pos and HC12 status\n"));

    EspID.begin();

    // Init CbLeds
    CbLeds.begin();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb, rotaryCb);

    // TODO: try to communicate with HC12 and decide if it is present or not
    CbOledDisplay.begin();

    CbLeds.clear();

    // Set white with varying intensity accross the LEDs
    CbLeds.setPixelColor(0, 0x020202);
    CbLeds.setPixelColor(1, 0x040404);
    CbLeds.setPixelColor(2, 0x080808);
    CbLeds.setPixelColor(3, 0x101010);
    CbLeds.setPixelColor(4, 0x202020);
    CbLeds.setPixelColor(5, 0x404040);
    CbLeds.show();

    CbHC12.begin(H12Baud);
    delay(50); // Wait for SoftSerial to stabalize
    HC12Status = CbHC12.check();
    DB(F("HC12 test "));
    DBLN(HC12Status ? F("pass") : F("fail"));

    // Update the OLED display
    display();

    DBLN(F("E:setup"));
}

void loop()
{
    CbRotaryInput.update();
    bool button = CbRotaryInput.buttonPushed();

    if (Millis() > LastSendHC12 + SendHC12PeriodMs) {
        LastSendHC12 = Millis();
        String message(F("CheeseBoard ID="));
        message += EspID.get();
        message += F(" button=");
        message += button ? F("pressed") : F("not pressed");
        message += F(" rotary pos=");
        message += RotaryValue;
        CbHC12.println(message);
    }

    if (button != PreviousButton) {
        PreviousButton = button;
        display();
    }
}

void buttonCb(uint16_t durationMs)
{
    DBF("buttonCb() - durationMs=%d\n", durationMs);
}

void display() {
    String text = F("CheeseBoard Test\nESP8266 ID: ");
    text += EspID.get();
    text += F("\nHC12 test: ");
    text += HC12Status ? F("pass") : F("fail");
    text += F("\nRotary Position: ");
    text += RotaryValue;
    text += F("\nButton: ");
    text += CbRotaryInput.buttonPushed() ? F("pressed") : F("not pressed");
    CbOledDisplay.clearBuffer();
    CbOledDisplay.drawText(text.c_str(), 'C', 'M');
    CbOledDisplay.sendBuffer();
}

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d\n", diff, value);
    RotaryValue = value;
    display();
}

