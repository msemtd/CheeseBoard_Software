#include <MutilaDebug.h>
#include <Millis.h>
#include <CheeseboardConfig.h>
#include "CbHC12.h"

CbHC12Class CbHC12(HC12_RX_PIN, HC12_TX_PIN, HC12_SET_PIN);

CbHC12Class::CbHC12Class(uint8_t rxPin, uint8_t txPin, uint8_t setPin) : 
    SoftwareSerial(rxPin, txPin),
    _setPin(setPin)
{
}

void CbHC12Class::begin(long speed)
{
    SoftwareSerial::begin(speed);
    pinMode(_setPin, OUTPUT);
    setCommandMode(false, true);
}

void CbHC12Class::setCommandMode(bool switchOn, bool force)
{
    // Note: set pin's logic is inverted = set to:
    // LOW for set mode
    // HIGH for transparent mode
    if (switchOn && (!_cmdMode || force)) {
        _cmdMode = true;
        digitalWrite(_setPin, !_cmdMode);
        delay(40);
    }
    else if (!switchOn && (_cmdMode || force)) {
        _cmdMode = false;
        digitalWrite(_setPin, !_cmdMode);
        delay(80);
    }
    DB(F("CbHC12Class::setCommandMode _cmdMode="));
    DBLN(_cmdMode);
}

bool CbHC12Class::check()
{
    bool oldCmdMode = _cmdMode;
    setCommandMode(true);
    const char expect[] = "OK\r\n";
    uint8_t idx = 0;
    unsigned long start = Millis();
    println("AT");
    while (Millis() - start < CheckTimeoutMs && idx < 4) {
        if (available()) {
            if (expect[idx] == read()) {
                idx++;
            } else {
                break;
            }
        }
    }
    setCommandMode(oldCmdMode);
    return idx == 4;
}


