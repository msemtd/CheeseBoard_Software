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
void getQuote();
void rotaryCb(int8_t diff, int32_t value);
void buttonCb(uint16_t durationMs);
void parseQuote();

// Constants
const uint8_t MaxCharBuffer = 8;
const uint8_t MaxNumFields = 1;

// Global variables
uint32_t LastUploadMs = 0;
uint32_t LastUpdateDisplayMs = 0;
String Fields[MaxNumFields];
String LastReply('-');
String CharBuffer;
String payload; // Holds the reply from the HTTP request
String quote; // Holds the final quote
String author;  // Holds the author of the quote
String textDisplay; // Holds the data to show on the OLED screen
bool PreviousButton = false;
int32_t RotaryValue = 0;
bool quoteLong = false; // This is a flag. If set then the quote is shown in two parts

#define DEBUG 1

void setup()
{
  Serial.begin(115200);
  DBLN(F("\n\nS:setup"));

  // Configure EspApConfigurator (access point config)
  EspApConfigurator.begin();
  // Use IP 192.168.4.1 when in configuration mode
  
  // Set AP mode details
  EspApConfigurator.setApDetails(AP_SSID, AP_PASSPHRASE);

  // Turn off the annoying flashing blue light!
  EspApConfigurator.heartbeat()->setEnabled(false);

  // Must add settings AFTER EspApConfigurator.begin()
  EspApConfigurator.addSetting(SET_QUOTE_URL,     new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  50, "http://api.forismatic.com/api/1.0/"));
  EspApConfigurator.addSetting(SET_SEED,    new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  6, ""));
  EspApConfigurator.addSetting(SET_LANG,    new PersistentSettingString(EspApConfigurator.nextFreeAddress(),  6, "en"));

  // Check the signature of the settings in EEPROM matches our setting, and reset to defaults if not
  if (!EspApConfigurator.checkSignature()) {
    EspApConfigurator.resetAll();
  }



  // Dump settings to serial
  DBLN(F("Settings:"));
  for (uint8_t i = 0; i < EspApConfigurator.count(); i++) {
    DB(EspApConfigurator[i].id);
    DB(F(" = "));
    DBLN(EspApConfigurator[i].setting->get());
  }
  // Init CbLeds
  CbLeds.begin();

  // Configure hardware
  // Init the OLED display
  CbOledDisplay.begin();
  updateOled();

  // Init the RotaryInput object
  CbRotaryInput.begin(buttonCb, rotaryCb);

  // Clear the LEDs for initial turn on
  CbLeds.clear();
  CbLeds.show();
}

void loop()
{
  // Give timeslice to EspApConfigurator
  EspApConfigurator.update();

  // Give a timeslice to the CBRotaryInput
  CbRotaryInput.update();

  // Quote is obtained when button is given a short press

  // Periodically update the OLED display
  if (Millis() > LastUpdateDisplayMs + 500) {
    LastUpdateDisplayMs = Millis();
    updateOled();
  }
}

void buttonCb(uint16_t durationMs)
{
  // This is what to do if the button is pressed
  // If the button is pressed for a long time then enter access point mode
  if (durationMs > 5000) {
    EspApConfigurator.setApMode(true);
  } else {
    // Short presses get a new quote
    getQuote();
    while(quoteLong!=false)
    {
      delay(250);
      getQuote();
    }
    updateOled();
  }
}

void rotaryCb(int8_t diff, int32_t value)
{
  //DBF("rotaryCb diff=%d value=%d\n", diff, value);
  if (RotaryValue != value)
  {
    if (textDisplay == author)
    {
      textDisplay = quote;
    }
    else
    {
      textDisplay = author;
    }
  }

  RotaryValue = value;
  // If the value has changed then update the OLED as required
  // Want to go through Quote then Author


}

bool portValidator(String port)
{
  return port.toInt() > 0 && port.toInt() < 65536;
}

void updateOled() {

  CbOledDisplay.clearBuffer();
  String text;
  if (EspApConfigurator.inApMode()) {
    // This is dpslayed when it is in Acess Point Mode
    // It shows the SSID and passwork needed to connect to the unit
    text += F("Access Point Active\n\n");
    text += F("SSID: ");
    text += AP_SSID;
    text += F("\nPass: ");
    text += AP_PASSPHRASE;
    CbOledDisplay.drawText(text.c_str(), 'L', 'M');
  } else {
    // This is displayed if quotes are all good
    if (EspApConfigurator.isConnected()) {
      //text += F("Connected");
    } else {
      text += F("Not connected");
    }
    text += textDisplay;
    CbOledDisplay.drawText(text.c_str(), 'L', 'M');
  }
  CbOledDisplay.sendBuffer();
}

void getQuote()
{
  // Check if we are connected to the wifi:

  if (EspApConfigurator.isConnected()) {
    // Only get the quote if connected
    HTTPClient http;

    // Here we create the URL to check for getting the quote information
    // Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en
    String url = EspApConfigurator[SET_QUOTE_URL]->get();
    // Create the full URL with method, format and language
    url += "?method=getQuote&key=";
    url += EspApConfigurator[SET_SEED]->get();
    url += "&format=text&lang=";
    url += EspApConfigurator[SET_LANG]->get();

    DBLN(url);  // Show the URL on the serial port

    http.begin(url);

    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString();
        DBLN(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  parseQuote();
}


void parseQuote()
{
  // If the quote is too long then get another quote?
  int quoteLen = payload.length();
  DBLN(quoteLen);

  if (quoteLen > 130)
  {
    // if the quote is too long then we get another one until its short enough
    quoteLong = true;
  }
  else
  {
    // Here we want to strip out the quote and the author
    // This is in the format "quote (author)"
    // So parse the payload to find quote and author

    int openingBracket = payload.indexOf('(');
    int closingBracket = payload.indexOf(')');

    // Need to check that there was an author
    if (openingBracket > 0 && closingBracket > 0)
    {
      // This means the author is valid
      // Need to strip out the author
      author = payload.substring(openingBracket + 1, closingBracket);
      quote = payload.substring(0, openingBracket);
    }
    else
    {
      // This case there is no author
      quote = payload;
      author = "Anon" ;
    }
    DBLN(quote);     // For debug
    DBLN(author);    // For debug

    quoteLong = false;
    textDisplay = quote;
    // Here the quote is written to the display
  }
}
