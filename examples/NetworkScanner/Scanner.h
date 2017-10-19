#pragma once

#include <ESP8266WiFi.h>

//! \brief Mode which does nothing (except debug out)
class ScannerClass {
public:
    typedef void (*t_scanCb)(int16_t);

public:
    ScannerClass();

    //! Call from setup() to initialize the object
    void begin(t_scanCb scanCb=NULL);

    //! Call frequently (e.g. from loop()) to keep things up to date
    void update();

    //! begin asynchronous scanning for wifi networks
    void startScan();

    void setApDetails(const char* ssid, const char* pass);

protected:
    t_scanCb _scanCb;
    bool scanning;
    bool wantScan;

};

extern ScannerClass Scanner;

