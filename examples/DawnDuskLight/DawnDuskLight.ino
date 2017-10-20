#include <Arduino.h>

// Include all dependent libraries so the Arduino IDE knows to add 
// the library paths into the build. Make sure all these libraries
// are installed on your system.
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Encoder.h>

// Include the EspApConfigurator header. This lets the ESP act as 
// an access point for configuration of the device (wifi login, 
// NTP setup, latitude and timezone..
#include <EspApConfigurator.h>
#include <PersistentSettingChar.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingLong.h>
#include <PersistentSettingString.h>
#include <ModeWifiClient.h>
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

// And finally includes from this project
#include "Config.h"
#include "ModeRealTime.h"

void rotaryCb(int8_t diff, int32_t value)
{
    DBF("rotaryCb diff=%d value=%d\n", diff, value);
}

void buttonCb(uint16_t durationMs)
{
    DBF("buttonCb() - durationMs=%d ", durationMs);
    EspApConfigurator.setApMode();
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN(F("S:setup"));

    EspApConfigurator.begin();

    // Enable web server in ModeWifiClient - this just makes testing
    // easier as I don't have to keep connecting to the AP to see
    // how the interface is being rendered...
    ModeWifiClient.enableHttpServer(true);

    // Set up NTP time adjuster
    ModeRealTime.begin();

    // Must add settings AFTER EspApConfigurator.begin()
    EspApConfigurator.addSetting(SET_NTP_SERVER, new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 64, "time.google.com"));

    // Dump settings 
    DBLN(F("Settings:"));                   
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {                                
        DB(EspApConfigurator[i].id);        
        DB(F(" = "));                       
        DBLN(EspApConfigurator[i].setting->get());                                       
    }


    // Init CbLeds
    CbLeds.begin();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb, rotaryCb);

    DBLN(F("E:setup"));
}

void loop()
{
    EspApConfigurator.update();
    ModeRealTime.update();
    CbRotaryInput.update();
}

