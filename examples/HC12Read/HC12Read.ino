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

// Function prototypes.  For the Arduino IDE you don't need these, but they 
// make it possible to build with the Makefile approach without having to put
// functions in the file before they are referenced by other functions.
void setup();
void loop();
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
    CbHC12.begin(H12Baud);

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

void buttonCb(uint16_t durationMs)
{
    String oldBuffer = CharBuffer;
    CharBuffer = F("Push duration=");
    CharBuffer += String(durationMs);
    CharBuffer += F("ms");
    handleBuffer();
    CharBuffer = oldBuffer;
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

uint8_t prevIdx(uint8_t idx) {
    return idx == 0 ? LineBufferSize-1 : idx-1;
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

