#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <MutilaDebug.h>
#include <Millis.h>

#include "Scanner.h"

ScannerClass Scanner;

ScannerClass::ScannerClass() 
{
}

void ScannerClass::begin(t_scanCb scanCb)
{
    DBLN(F("S:Scanner::begin()"));

    _scanCb = scanCb;
    
    // We're not scanning yet, but we'd like to
    scanning = false;
    wantScan = true;

    // Make sure the WiFi is turned on
    WiFi.forceSleepWake();

    // Put ESP into Station Mode (required to scan)
    WiFi.mode(WIFI_STA);

    DBLN(F("E:Scanner::begin()"));
}

void ScannerClass::update()
{
    if (scanning) {
        int8_t netCount = WiFi.scanComplete();
        if (netCount >= 0) {
            scanning = false;
            DB(F("Scan complete, found "));
            DB(netCount);
            DBLN(F(" net(s)"));
            if (_scanCb != NULL) {
                _scanCb(netCount);
            }
        }
    } else if (wantScan) {
        startScan();
    }
}

void ScannerClass::startScan()
{
    if (scanning) {
        DBLN(F("[still scanning]"));
        return;
    }
    wantScan = false;
    scanning = true;
    WiFi.scanNetworks(true);
}

