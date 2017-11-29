#include <MutilaDebug.h>
#include <Millis.h>
#include <CbOledDisplay.h>
#include <CbLeds.h>
#include <EspApConfigurator.h>
#include "ClockDisplay.h"
#include "SetupMode.h"
#include "RealTimeClock.h"
#include "StandbyMode.h"
#include "ModeManager.h"
#include "Config.h"

SetupModeClass SetupMode;

SetupModeClass::SetupModeClass()
{
}

void SetupModeClass::begin()
{
    DB(F("SetupMode::begin"));
    // We don't need to to spin the CPU for this
    setUpdatePeriod(100);
}

//! Called when this mode is activated
void SetupModeClass::modeStart()
{
    DB(F("SetupMode::modeStart"));
    EspApConfigurator.setApMode(true);
    ClockDisplay.disable();
    CbOledDisplay.clearBuffer();
    String message(F("Access Point Mode activated, connect to "));
    message += AP_SSID;
    message += F(" with password \"");
    message += AP_PASSPHRASE;
    message += F("\"\nhttp://192.168.4.1/");
    CbOledDisplay.drawText(message.c_str(), 'C', 'M');
    CbOledDisplay.sendBuffer();
}

//! Called when this mode is de-activated
void SetupModeClass::modeStop()
{
    DB(F("SetupMode::modeStop"));
}

//! Called periodically while mode is active
void SetupModeClass::modeUpdate()
{
    // Wait for AP mode to terminate, and then revert to StandbyMode
    if (EspApConfigurator.inApMode()) {
        return;
    }

    ModeManager.switchMode(&StandbyMode);
}

void SetupModeClass::pushEvent(uint16_t durationMs)
{
    DB(F("SetupModeClass::pushEvent ms="));
    DBLN(durationMs);

    // Cancel AP mode
    EspApConfigurator.setApMode(false);
    ModeManager.switchMode(&StandbyMode);
}

void SetupModeClass::twistEvent(int8_t diff, int32_t value)
{
    DB(F("SetupModeClass::twistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

void SetupModeClass::pushTwistEvent(int8_t diff, int32_t value)
{
    DB(F("SetupModeClass::pushTwistEvent diff="));
    DB(diff);
    DB(F(" value="));
    DBLN(value);
}

