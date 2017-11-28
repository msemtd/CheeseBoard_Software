#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system.
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>
#include <SoftwareSerial.h>

// Include the EspApConfigurator header. This lets the ESP act as 
// an access point for configuration of the device (wifi login, 
// NTP setup, latitude and timezone..
//#include <EspApConfigurator.h>
//#include <PersistentSettingBool.h>
//#include <PersistentSettingString.h>
//#include <PersistentSettingLong.h>
//#include <ModeWifiClient.h>
//#include <ModeAP.h>
// Other ESP and network operation headers which are needed for
// EspApConfigurator
//#include <EEPROM.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266WiFi.h>
//#include <WiFiClientSecure.h>
//#include <DNSServer.h>
//#include <Mode.h>
//#include <ParentMode.h>
//#include <MutilaDebug.h>
//#include <TimeLib.h>

// Include CheeseBoard headers for the components we'll be using in
// this example.
#include <MutilaDebug.h>
#include <Millis.h>
#include <CbRotaryInput.h>
#include <CbOledDisplay.h>
#include <CbHC12.h>
#include <CheeseboardConfig.h>

// Includes from this project directory
#include "Config.h"

// Function prototypes 
void buttonCb(uint16_t durationMs);
void handleBuffer();
uint8_t prevIdx(uint8_t idx);
void rotaryCb(int8_t diff, int32_t value);

// Globals
const uint16_t MaxBufLen = 64;
const uint8_t LineBufferSize = 6;
String CharBuffer;
String LineBuffer[LineBufferSize];
uint8_t LineBufferIdx = 0;

void setup() 
{
    Serial.begin(115200);
    DBLN(F("\n\nS:setup"));

    // Initialize the HC12 module
    CbHC12.begin(9600);

    // Initialize rotary input with callbacks
    CbRotaryInput.begin(buttonCb, rotaryCb);

    // Configure hardware
    // Init the OLED display
    CbOledDisplay.begin();
    CbOledDisplay.setFont(CBOLED_MESSAGE_FONT);
    delay(200);

    CbOledDisplay.clear();
    if (CbHC12.check()) {
        CbOledDisplay.drawText("HC12 OK\nwaiting for comms...", 'C', 'M');
    } else {
        CbOledDisplay.drawText("HC12 ERROR", 'C', 'M');
    }
    CbOledDisplay.show();

    DBLN(F("E:setup"));
}

void loop() 
{
    CbRotaryInput.update();

    while(CbHC12.available()) {
        int c = CbHC12.read();
        if (c < 0) {
            DB(F("serial error, c="));
            DBLN(c);
        } else if (c == '\n') {
            handleBuffer();
        } else if (c >= 32 && c <=127) {
            if (CharBuffer.length() < MaxBufLen) {
                // printable character, append to buffer
                DBF("adding %c\n", (char)c);
                CharBuffer += (char)c;
            } else {
                DBLN(F("CharBuffer length exceeded"));
            }
        } else {
            DBF("Ignoring junk character: 0x%02X\n", c);
        }
    }
}

void rotaryCb(int8_t diff, int32_t value)
{
    String oldBuffer = CharBuffer;
    CharBuffer = F("Twist diff=");
    CharBuffer += String(diff);
    CharBuffer += F(" value=");
    CharBuffer += String(value);
    handleBuffer();
    CharBuffer = oldBuffer;
}

void buttonCb(uint16_t durationMs)
{
    String oldBuffer = CharBuffer;
    CharBuffer = F("Push duration=");
    CharBuffer += String(durATionMs);
    CharBuffer += F("ms");
    handleBuffer();
    CharBuffer = oldBuffer;
}

uint8_t prevIdx(uint8_t idx) {
    return idx == 0 ? LineBufferSize-1 : idx-1;
}

void handleBuffer() {
    // DBLN(F("handleBuffer()"));
    LineBuffer[LineBufferIdx] = CharBuffer;
    CharBuffer = String();
    uint8_t idx = LineBufferIdx;
    CbOledDisplay.clear();
    for (uint8_t i=0; i<LineBufferSize; i++) {
        // DBF(" i=%d LineBuffer[%d]=%s\n", i, idx, LineBuffer[idx].c_str());
        CbOledDisplay.drawStr(0, CbOledDisplay.getDisplayHeight() - ((CBOLED_MESSAGE_FONT_HEIGHT+2)*i), LineBuffer[idx].c_str());
        idx = prevIdx(idx);
    }
    // DBLN("--");
    CbOledDisplay.show();
    LineBufferIdx = (LineBufferIdx + 1) % (LineBufferSize);

}
