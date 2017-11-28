#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class CbHC12Class : public SoftwareSerial {
public:
    const static uint16_t CheckTimeoutMs = 250;

public:
    CbHC12Class(uint8_t rxPin, uint8_t txPin, uint8_t setPin);
    void begin(long speed);

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
    const uint8_t _setPin;
    bool _cmdMode;

};

extern CbHC12Class CbHC12;

