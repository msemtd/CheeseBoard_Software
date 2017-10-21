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
#include <PersistentSettingTime.h>
#include <PersistentSettingFloat.h>
#include <PersistentSettingBool.h>
#include <PersistentSettingUInt8.h>
#include <PersistentSettingString.h>
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

// And finally includes from this project
#include "Config.h"
#include "StandbyMode.h"
#include "GoToSleepMode.h"
#include "ModeManager.h"
#include "ModeRealTime.h"
#include "EventMode.h"

bool pushTwist = false;

void rotaryCb(int8_t diff, int32_t value)
{
    if (CbRotaryInput.buttonPushed()) {
        pushTwist = true;
        ModeManager.pushTwistEvent(diff, value);
    } else {
        ModeManager.twistEvent(diff, value);
    }

    //int mins = value + EspApConfigurator[SET_SLEEP_DURATION]->get().toInt();
    //if (mins >= 1 && mins <= 30) {
    //    GoToSleepMode.setFadeTime(mins);
    //}
}

void buttonCb(uint16_t durationMs)
{
    if (pushTwist) {
        pushTwist = false;
    } else {
        ModeManager.pushEvent(durationMs);
    }
    //if (durationMs > 1000) {
    //    EspApConfigurator.setApMode();
    //} else if (ModeRealTime.unixTime() == 0) {
    //    DBLN(F("Haven't got real time yet..."));
    //} else {
    //    if (mode != &GoToSleepMode) {
    //        switchMode(&GoToSleepMode);
    //    } else {
    //        switchMode(&StandbyMode);
    //    }
    //}
}

bool latitudeValidator(String s) 
{
    float f = s.toFloat();
    return f >= -180.0 && f <= 180.0;
}

bool timezoneValidator(String s) 
{
    // Timezones can be between -12 and 12, and have .0 and .5 frational parts
    const float tol = 0.001; // tolerance for float precision 
    float tz = s.toFloat();

    // get fractional part of tz...
    float f = tz - (int)tz;
    if (f < 0) f *= -1;

    return tz >= -12 && tz <= 12 && ((f > -tol && f < tol) || (f > 0.5-tol && f < 0.5+tol));
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    DBLN(F("S:setup"));

    EspApConfigurator.begin();
    EspApConfigurator.setApDetails(AP_SSID, AP_PASSPHRASE); 

    // Enable web server in ModeWifiClient - this just makes testing
    // easier as I don't have to keep connecting to the AP to see
    // how the interface is being rendered...
    ModeWifiClient.enableHttpServer(true);

    // Set up NTP time adjuster
    ModeRealTime.begin();

    // Must add settings AFTER EspApConfigurator.begin()
    EspApConfigurator.addSetting(SET_WAKE_TIME,      new PersistentSettingTime(EspApConfigurator.nextFreeAddress(), "7:30"));
    EspApConfigurator.addSetting(SET_WAKE_DURATION,  new PersistentSettingUInt8(EspApConfigurator.nextFreeAddress(), 30));
    EspApConfigurator.addSetting(SET_SLEEP_DURATION, new PersistentSettingUInt8(EspApConfigurator.nextFreeAddress(), 15));
    EspApConfigurator.addSetting(SET_LATITUDE,       new PersistentSettingFloat(EspApConfigurator.nextFreeAddress(), 52.95, 5, latitudeValidator));
    EspApConfigurator.addSetting(SET_TIMEZONE,       new PersistentSettingFloat(EspApConfigurator.nextFreeAddress(), 0, 1, timezoneValidator));
    EspApConfigurator.addSetting(SET_DST,            new PersistentSettingBool(EspApConfigurator.nextFreeAddress(), false));
    EspApConfigurator.addSetting(SET_NTP_SERVER,     new PersistentSettingString(EspApConfigurator.nextFreeAddress(), 64, "time.google.com"));

    // Dump settings 
    DBLN(F("Settings:"));                   
    for (uint8_t i=0; i<EspApConfigurator.count(); i++) {                                
        DB(EspApConfigurator[i].id);        
        DB(F(" = "));                       
        DBLN(EspApConfigurator[i].setting->get());                                       
    }

    // Init CbLeds and blank them
    CbLeds.begin();
    CbLeds.clear();
    CbLeds.show();

    // Init the RotaryInput object
    CbRotaryInput.begin(buttonCb, rotaryCb);

    // Init the OLED display
    CbOledDisplay.begin();
    CbOledDisplay.clearBuffer();
    CbOledDisplay.sendBuffer();

    // Init modes from this project
    StandbyMode.begin();
    GoToSleepMode.begin();
    ModeManager.begin(&StandbyMode);

    DBLN(F("E:setup"));
}

void loop()
{
    EspApConfigurator.update();
    ModeRealTime.update();
    CbRotaryInput.update();
    ModeManager.update();   // updates the currently-active mode

    // All modes that are not StandbyMode can finish - and we will switch
    // back to standby.
    if (ModeManager.isFinished()) {
        ModeManager.switchMode(&StandbyMode);
    }

}

