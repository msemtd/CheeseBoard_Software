#include <MutilaDebug.h>
#include <Millis.h>
#include <CheeseboardConfig.h>
#include "CbHC12.h"

CbHC12Class CbHC12;

CbHC12Class::CbHC12Class() : 
    _serial(HC12_RX_PIN, HC12_TX_PIN)
{
}

void CbHC12Class::begin()
{
    pinMode(HC12_SET_PIN, OUTPUT);
    _serial.begin(HC12_BAUD);
    setCommandMode(false, true);
}

void CbHC12Class::setCommandMode(bool switchOn, bool force)
{
    // Note: set pin's logic is inverted = set to:
    // LOW for set mode
    // HIGH for transparent mode
    if (switchOn && (!_cmdMode || force)) {
        _cmdMode = true;
        digitalWrite(HC12_SET_PIN, !_cmdMode);
        delay(40);
    }
    else if (!switchOn && (_cmdMode || force)) {
        _cmdMode = false;
        digitalWrite(HC12_SET_PIN, !_cmdMode);
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
    _serial.println("AT");
    while (Millis() - start < CheckTimeoutMs && idx < 4) {
        if (_serial.available()) {
            if (expect[idx] == _serial.read()) {
                idx++;
            } else {
                break;
            }
        }
    }
    setCommandMode(oldCmdMode);
    return idx == 4;
}


