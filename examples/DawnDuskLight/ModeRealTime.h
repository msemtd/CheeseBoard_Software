#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Mode.h>
#include <TimeLib.h>

const uint16_t LOCAL_NTP_PORT   = 8888;
const uint8_t  NTP_PACKET_SIZE  = 48;

//! \brief A real time class - set by NTP
//! * Fetches time and dat via NTP.
//! * Periodcally synchronizes local clock by NTP
class ModeRealTime_ : public Mode {
public:
    enum State {
        Wait,
        NtpWorking
    };

    ModeRealTime_();
    void begin();
    void modeStart();
    void modeStop();
    void modeUpdate();
    time_t unixTime();

    //! Get the number of seconds into the day (taking into account
    //! timezone and DST
    long daySeconds();

    //! Get time as "YYYY-MM-DD HH:MM:SS UT"
    String isoTimestamp();

    //! Get time of day as string (taking into account timezone and dst)
    //! \param includeSeconds include seconds in output
    String timeStr(bool includeSeconds=false);

    //! Get date as "Dayname YYYY-MM-DD", e.g. "Sunday 2017-10-22"
    String dateStr();

    //! \return true if we have a real time (i.e. NTP has been set since 
    //! instantiation
    bool haveRealTime() { return _state == NtpWorking; }

protected:
    unsigned long _lastNtpAttempt;
    unsigned long _lastNtpSuccess;
    time_t _unixTime;
    IPAddress _ntpServerIP;
    WiFiUDP _udp;
    byte _buf[NTP_PACKET_SIZE];   
    State _state;

    void ntpUpdate();
    void clearBuf();
    bool dnsLookup();
    void sendNtpPacket();
};

extern ModeRealTime_ ModeRealTime;

