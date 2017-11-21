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

bool HC12Status = false;
bool previousButton = false;
int32_t rotaryValue = 0;

void display() {
    String text = F("CheeseBoard Test\nButton: ");
    text += CbRotaryInput.buttonPushed() ? F("pressed") : F("not pressed");
    text += '\n';
    text += F("Rotary Position: ");
    text += rotaryValue;
    text += '\n';
    text += F("HC12 test: ");
    text += HC12Status ? F("pass") : F("fail");
    CbOledDisplay.clearBuffer();
    CbOledDisplay.drawText(text.c_str(), 'C', 'M');
    CbOledDisplay.sendBuffer();
}

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d\n", diff, value);
    rotaryValue = value;
    display();
}

void buttonCb(uint16_t durationMs)
{
    DBF("buttonCb() - durationMs=%d\n", durationMs);
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN(F("\n\nCheeseBoard Test Firmware\nLEDs should be on (dim -> brighter)\nOLED should show button status, rotary pos and HC12 status\n"));

    // Init CbLeds
    CbLeds.begin();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb, rotaryCb);

    // TODO: try to communicate with HC12 and decide if it is present or not
    CbOledDisplay.begin();

    CbLeds.clear();
    CbLeds.setPixelColor(0, 0x020202);
    CbLeds.setPixelColor(1, 0x040404);
    CbLeds.setPixelColor(2, 0x080808);
    CbLeds.setPixelColor(3, 0x101010);
    CbLeds.setPixelColor(4, 0x202020);
    CbLeds.setPixelColor(5, 0x404040);
    CbLeds.show();

    CbHC12.begin();
    delay(50);
    HC12Status = CbHC12.check();
    DB(F("HC12 test "));
    DBLN(HC12Status ? F("pass") : F("fail"));

    display();
    DBLN(F("E:setup"));
}

void loop()
{
    CbRotaryInput.update();
    bool button = CbRotaryInput.buttonPushed();
    if (button != previousButton) {
        previousButton = button;
        display();
    }
}

