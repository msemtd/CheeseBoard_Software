#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class CbHC12Class {
public:
    const static uint16_t CheckTimeoutMs = 250;

public:
    CbHC12Class();
    void begin();

    /*!
     * Turn on or off command mode. When enabled, the HC12 accepts AT commands to control the device.
     * \param switchOn when true, enter AT command mode, when false leave command mode
     * \param force
     */
    void setCommandMode(bool switchOn, bool force=false);

    /*! Drop into AT mode, sent "AT", and verify we get "OK" back
     * \return true if OK, else false
     */
    bool check();

private:
    SoftwareSerial _serial;
    bool _cmdMode;

};

extern CbHC12Class CbHC12;

