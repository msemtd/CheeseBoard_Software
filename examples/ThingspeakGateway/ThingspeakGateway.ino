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
#include <EspApConfigurator.h>
#include <PersistentSettingBool.h>
#include <PersistentSettingString.h>
#include <PersistentSettingLong.h>
#include <ModeWifiClient.h>
#include <ModeAP.h>
// Other ESP and network operation headers which are needed for
// EspApConfigurator
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <DNSServer.h>
#include <Mode.h>
#include <ParentMode.h>
#include <MutilaDebug.h>
#include <TimeLib.h>

// Include CheeseBoard headers for the components we'll be using in
// this example.
#include <MutilaDebug.h>
#include <CbLeds.h>
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
bool portValidator(String port);
void updateOled();
void uploadToThingspeak(uint8_t numFields, String fieldValues[]);

// Constants
const uint8_t MaxCharBuffer = 64;
const uint8_t MaxNumFields = 8;

// Global variables
uint32_t LastUploadMs = 0;
uint32_t LastUpdateDisplayMs = 0;
String Fields[MaxNumFields];
String LastReply('-');
String CharBuffer;

void setup() 
{
    Serial.begin(115200);
    DBLN(F("\n\nS:setup"));

    // Configure EspApConfigurator (access point config)
    EspApConfigurator.begin();

    // Turn off the annoying flashing blue light!
    EspApConfigurator.heartbeat()->setEnabled(false);

    // Set AP mode details
    EspApConfigurator.setApDetails(AP_SSID, AP_PASSPHRASE); 

    // Must add settings AFTER EspApConfigurator.begin()
    EspApConfigurator.addSetting(SET_API_SERVER,  new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  48, "api.thingspeak.com"));
    EspApConfigurator.addSetting(SET_API_PORT,    new PersistentSettingLong(EspApConfigurator.nextFreeAddress(),   443, &portValidator));
    EspApConfigurator.addSetting(SET_API_KEY,     new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  16, "[write api key]"));
    EspApConfigurator.addSetting(SET_VERIFY_CERT, new PersistentSettingBool(EspApConfigurator.nextFreeAddress(),    false));
    EspApConfigurator.addSetting(SET_CERT_SERVER, new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  48, "thingspeak.com"));
    EspApConfigurator.addSetting(SET_CERT_SHA,    new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  60, "[SHA hash]"));

    // Check the signature of the settings in EEPROM matches our setting, and reset to defaults if not
    if (!EspApConfigurator.checkSignature()) {
        EspApConfigurator.resetAll();
    }

    // Dump settings to serial
    DBLN(F("Settings:"));
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {
        DB(EspApConfigurator[i].id);
        DB(F(" = "));
        DBLN(EspApConfigurator[i].setting->get());
    }

    CbHC12.begin(HC12_BAUD);

    // Configure hardware
    // Init the OLED display
    CbOledDisplay.begin();
    updateOled();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb);

    DBLN(F("E:setup"));
}

void loop() 
{
    // Give timeslice to EspApConfigurator
    EspApConfigurator.update();
    CbRotaryInput.update();

    // This should be pretty familiar to anyone who has handled serial data input before
    while (CbHC12.available()) {
        int c = CbHC12.read();
        if (c < 0) {
            DB(F("serial error, c="));
            DBLN(c);
        } else if (c >= 32 && c <=127) {
            if (CharBuffer.length() < MaxCharBuffer) {
                // printable character, append to buffer
                //DB(F("HC12 char="));
                //DBLN((char)c);
                CharBuffer += (char)c;
            } else {
                DBLN(F("WARNING: CharBuffer length exceeded"));
            }
        } else if (c == '\n') {
            handleBuffer();
        }
    }

    // Periodically update the OLED display
    if (Millis() > LastUpdateDisplayMs + 500) {
        LastUpdateDisplayMs = Millis();
        updateOled();
    }
}

void buttonCb(uint16_t durationMs)
{
    EspApConfigurator.setApMode(true);
}

void handleBuffer()
{
    DB(F("handleBuffer(), buf="));
    DBLN(CharBuffer);
    // Split CharBuffer into up to 8 fields using comma as separator
    uint8_t i=0;
    uint8_t count=0;
    String field = "";
    while (i < CharBuffer.length() && count < MaxNumFields) {
        if (CharBuffer[i] == ',') {
            DBF("Field[%d] = %s\n", count, field.c_str());
            Fields[count++] = field;
            field = "";
        } else {
            field += CharBuffer[i];
        }
        i++;
    }
    if (field != "") {
        DBF("Field[%d] = %s\n", count, field.c_str());
        Fields[count++] = field;
        field = "";
    }

    CharBuffer = "";
    if (count > 0) {
        if (EspApConfigurator.isConnected()) {
            uploadToThingspeak(count, Fields);
        }
        updateOled();
    }
}

bool portValidator(String port)
{
    return port.toInt() > 0 && port.toInt() < 65536;
}

void updateOled() {
    CbOledDisplay.clearBuffer();
    String text;
    if (EspApConfigurator.inApMode()) {
        text += F("Access Point Active\n\n");
        text += F("SSID: ");
        text += AP_SSID;
        text += F("\nPass: ");
        text += AP_PASSPHRASE;
        CbOledDisplay.drawText(text.c_str(), 'L', 'M');
    } else {
        if (EspApConfigurator.isConnected()) {
            text += F("Connected");
        } else { 
            text += F("Not connected");
        }
        text += F("\nLast reply: ");
        text += LastReply;
        CbOledDisplay.drawText(text.c_str(), 'L', 'M');
    }
    CbOledDisplay.sendBuffer();
}

void uploadToThingspeak(uint8_t numFields, String fieldValues[])
{
    DB(F("uploadToThingspeak numFields="));
    DBLN(numFields);
    WiFiClientSecure client;
    
    DB(F("Opening connection to "));
    DB(EspApConfigurator[SET_API_SERVER]->get());
    DB(F(" on port "));
    DB(EspApConfigurator[SET_API_PORT]->get());
    DBLN(F("... "));
    if (!client.connect(EspApConfigurator[SET_API_SERVER]->get().c_str(), EspApConfigurator[SET_API_PORT]->get().toInt())) {
        DBLN(F("FAILED"));
        return;
    }
    DBLN("OK");

    if (EspApConfigurator[SET_VERIFY_CERT]->get().toInt()) {
        DB(F("Host TLS cert verification (hash: "));
        DB(EspApConfigurator[SET_CERT_SHA]->get().c_str());
        DB(F(")..."));
        if (!client.verify(EspApConfigurator[SET_CERT_SHA]->get().c_str(), EspApConfigurator[SET_API_SERVER]->get().c_str())) {
            DBLN(F("FAILED"));
            return;
        }
        DBLN(F("SUCCESS"));
    }

    String url = F("/update?api_key=");
    url += EspApConfigurator[SET_API_KEY]->get();
    for (uint8_t i=0; i<numFields; i++) {
        url += F("&field");
        url += i+1;
        url += F("=");
        url += fieldValues[i];
    }

    DB(F("url: "));
    DBLN(url);
    DB(F("Sending HTTP request... "));
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                    "Host: " + EspApConfigurator[SET_API_SERVER]->get() + "\r\n" +
                    "User-Agent: CheeseBoard/ThingspeakUploader\r\n" +
                    "Connection: close\r\n\r\n");

    bool headers = true;
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            headers = false;
        } else if (headers && line.startsWith("Status:")) {
            DBLN(line);
        } else if (!headers) {
            DB(F("Reply: "));
            DBLN(line);
            LastReply = line;
        }
    }
    updateOled();
}

