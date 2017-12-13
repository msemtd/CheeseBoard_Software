#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

/*! \brief Class for providing real time clock function, backed by NTP
 *
 * NTP code taken from the TimeNTP_ESP8266WiFi example in TimeLib version 1.5
 * by Michael Margolis & Paul Stoffregen http://playground.arduino.cc/code/time
 *
 */

class RealTimeClockClass {
public:
    static const char* DayNames[];
    static const uint16_t LocalNtpPort   = 8888;
    static const uint8_t  NtpPacketSize  = 48;

public:
    enum State {
        Wait,
        NtpOn,
        NtpOff
    };

    RealTimeClockClass();
    void begin();
    void update();

    //! Prevent the RealTimeClock trying to do Ntp operations on the network 
    void suspendNtp() { _state = NtpOff; }

    //! Prevent the RealTimeClock trying to do Ntp operations on the network 
    void resumeNtp() { _state = NtpOn; }

    //! \return true if we have a real time (i.e. NTP has been set since instantiation)
    bool haveRealTime() { return _lastNtpSuccess != 0; }

    //! Fetch the time/date in seconds since epoch, as per unix tradition
    time_t unixTime();

    //! Get the number of seconds into the day (taking into account timezone and DST)
    long daySeconds();

    //! Get time of day as string (taking into account timezone and dst)
    //! \param includeSeconds include seconds in output
    //!
    //! Example output where includeSeconds is false: "0:01" "9:45" "12:11" "16:20"
    //! Example output where includeSeconds is true: "0:03:19" "9:45:00" "12:12:01" "23:59:59"
    String timeStr(bool includeSeconds=false);

    //! Get date as "Dayname YYYY-MM-DD", e.g. "Sunday 2017-10-22"
    //! Sorry, no i18n support at this time. Hah, "at this time"... Very punny.
    String dateStr();

    //! \return number of seconds until the clock next reads timeStr, or 100000 if
    //!         we don't know (e.g. we haven't got real time yet)
    //! \param timeStr may or may not include seconds
    uint32_t secondsUntilNext(String timeStr);

protected:
    uint32_t _lastNtpAttempt;
    uint32_t _lastNtpSuccess;
    time_t _unixTime;
    IPAddress _ntpServerIP;
    WiFiUDP _udp;
    byte _buf[NtpPacketSize];   
    State _state;

    void ntpUpdate();
    void clearBuf();
    bool dnsLookup();
    void sendNtpPacket();
};

//! Global instance of RealTimeClockClass
extern RealTimeClockClass RealTimeClock;

