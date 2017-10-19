#include <CbDebug.h>
#include <CbMillis.h>
#include "GfxNetInfo.h"
#include "GfxTextBox.h"
#include "GfxSignalStrength.h"
#include "CbOled.h"
#include "Config.h"

GfxNetInfo::GfxNetInfo(String newSsid, int8_t newChannel, uint8_t newSignal) :
    _ssid(newSsid),
    _channel(newChannel),
    _signal(newSignal),
    _lastSeenMs(CbMillis())
{
}

GfxNetInfo::~GfxNetInfo()
{
}

GfxNetInfo::GfxNetInfo(const GfxNetInfo& other)
{
    _ssid = other._ssid;
    _channel = other._channel;
    _signal = other._signal;
    _lastSeenMs = other._lastSeenMs;
}

GfxNetInfo& GfxNetInfo::operator=(const GfxNetInfo& other)
{
    _ssid = other._ssid;
    _channel = other._channel;
    _signal = other._signal;
    _lastSeenMs = other._lastSeenMs;
    return *this;
}

void GfxNetInfo::draw(uint16_t xOffset, uint16_t yOffset)
{
    GfxSignalStrength gfxSig;
    GfxTextBox gfxSeen(20, lastSeen());
    GfxTextBox gfxSsid(width() - 20 - gfxSig.width(), ssid());

    gfxSig.setSignal(signal());

    gfxSig.draw(xOffset, yOffset);
    gfxSeen.draw(xOffset+gfxSig.width(), yOffset);
    gfxSsid.draw(xOffset+gfxSig.width()+gfxSeen.width(), yOffset);
}

uint16_t GfxNetInfo::height()
{
    return CBOLED_MESSAGE_FONT_HEIGHT + (2*CBOLED_MESSAGE_FONT_VSEP);
}

uint16_t GfxNetInfo::width()
{
    return 128;
}

String GfxNetInfo::ssid()
{
    return _ssid;
}

int8_t GfxNetInfo::channel()
{
    return _channel;
}

uint8_t GfxNetInfo::signal()
{
    return _signal;
}

uint32_t GfxNetInfo::lastSeenMs()
{
    return _lastSeenMs;
}

String GfxNetInfo::lastSeen()
{
    uint32_t ago = (CbMillis() - _lastSeenMs) / 1000;
    String s;
    if (ago >= 3600) {
        s += String(ago/3600);
        s += 'h';
    } else if (ago >= 60) {
        s += String(ago/60);
        s += 'm';
    } else {
        s += String(ago);
        s += 's';
    }
    return s;
}

void GfxNetInfo::update(int8_t newChannel, uint8_t newSignal)
{
    _channel = newChannel;
    _signal = newSignal;
    _lastSeenMs = CbMillis();
}

bool GfxNetInfo::operator==(const GfxNetInfo& other)
{
    return _ssid == other._ssid;
}

