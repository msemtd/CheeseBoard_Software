#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "GfxItem.h"

class GfxNetInfo : public GfxItem {
public:
    GfxNetInfo(String newSsid, int8_t newChannel, uint8_t newSignal);
    ~GfxNetInfo();
    GfxNetInfo(const GfxNetInfo& other);
    GfxNetInfo& operator=(const GfxNetInfo& other);
    bool operator==(const GfxNetInfo& other);

    // For GfxItem
    void draw(uint16_t xOffset=0, uint16_t yOffset=0);
    uint16_t width();
    uint16_t height();
    
    String ssid();                      //!< get the ssid
    int8_t channel();                   //!< get the channel
    uint8_t signal();                   //!< get the last signal strength
    uint32_t lastSeenMs();              //!< get last seen ms
    String lastSeen();                  //!< get last seen in human readable form

    //! Updates the ephemeral details for this ssid
    void update(int8_t newChannel, uint8_t newSignal);

protected:
    String _ssid;
    int8_t _channel;
    uint8_t _signal;
    uint32_t _lastSeenMs;

};

