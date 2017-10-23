#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <PersistentSettingTime.h>
#include <TimeLib.h>
#include "RealTimeClock.h"
#include "Config.h"

// Days of week - modify this for other languages.
char* const RealTimeClockClass::DayNames[] = {"", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturaday"};

// Global instance
RealTimeClockClass RealTimeClock;

RealTimeClockClass::RealTimeClockClass() :
    _lastNtpAttempt(0),
    _lastNtpSuccess(0),
    _unixTime(0),
    _state(Wait)
{
    clearBuf();
}

void RealTimeClockClass::begin()
{
    DBLN(F("RealTimeClock::begin"));
    _udp.begin(LocalNtpPort);
    clearBuf();
}

void RealTimeClockClass::update()
{
    if (_state == NtpOff) {
        return;
    }

    if (_lastNtpSuccess == 0 || Millis() > _lastNtpSuccess + (NTP_REFRESH_PERIOD_S*1000)) {
        // It's time we tried to get the time by Ntp, either because we have not yet
        // got it or because it's been more than NTP_REFRESH_PERIOD_S seconds since we
        // did, so we should refresh it
        if (_lastNtpAttempt == 0 || Millis() > _lastNtpAttempt + (NTP_RETRY_PERIOD_S*1000)) {
            // We throttle our attempts to no more than once every NTP_RETRY_PERIOD_S seconds...
            _lastNtpAttempt = Millis();
            ntpUpdate();
        }
    }
}

void RealTimeClockClass::ntpUpdate()
{
    //DBLN(F("RealTimeClockClass::ntpUpdate"));
    if (!EspApConfigurator.isConnected()) {
        //DBLN(F("no network connection"));
        return;
    }

    // Find the IP of the timeserver
    dnsLookup();

    // If we have an IP for the timeserver, proceed
    if (_ntpServerIP == IPAddress()) {
        return;
    }

    // Clear previously RX packets
    while (_udp.parsePacket() > 0) {;}

    // Send a request
    sendNtpPacket();

    // TODO: re-write this in a non-blocking way?
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
        int size = _udp.parsePacket();
        if (size >= NtpPacketSize) {
            DBLN(F("NTP RX"));
            _udp.read(_buf, NtpPacketSize); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 =  (unsigned long)_buf[40] << 24;
            secsSince1900 |= (unsigned long)_buf[41] << 16;
            secsSince1900 |= (unsigned long)_buf[42] << 8;
            secsSince1900 |= (unsigned long)_buf[43];
            
            unsigned long newUnix = secsSince1900 - 2208988800UL;
            int diff = newUnix - unixTime();
            _unixTime = newUnix;
            _lastNtpSuccess = Millis();
            DB(F("NTP success: "));
            DB(_unixTime);
            DB(F(" (diff="));
            DB(diff);
            DBLN(')');
            _state = NtpOn;
            return;
        }
    }
    DBLN("NTP failed");
}

time_t RealTimeClockClass::unixTime()
{
    if (_unixTime == 0) return 0;
    return _unixTime + ((Millis() - _lastNtpSuccess) / 1000);
}

long RealTimeClockClass::daySeconds()
{
    time_t unixtime = unixTime();

    // Adjust for timezone
    unixtime += EspApConfigurator[SET_TIMEZONE]->get().toInt() * 3600;

    // Additional offset for daylight savings if set
    unixtime += EspApConfigurator[SET_DST]->get().toInt() ? 3600 : 0;

    long result = hour(unixtime)*3600;
    result += minute(unixtime)*60;
    result += second(unixtime);
    return result;
}

String RealTimeClockClass::timeStr(bool includeSeconds)
{
    char buf[9];
    time_t unixtime = unixTime();

    // Adjust for timezone
    unixtime += EspApConfigurator[SET_TIMEZONE]->get().toInt() * 3600;

    // Additional offset for daylight savings if set
    unixtime += EspApConfigurator[SET_DST]->get().toInt() ? 3600 : 0;
    
    if (includeSeconds) {
        snprintf(buf, 9, "%d:%02d:%02d", 
                 hour(unixtime), 
                 minute(unixtime),
                 second(unixtime));
    } else {
        snprintf(buf, 9, "%d:%02d", 
                 hour(unixtime), 
                 minute(unixtime));
    }
    return String(buf);
}

String RealTimeClockClass::dateStr()
{
    char buf[21];
    time_t unixtime = unixTime();

    // Adjust for timezone
    unixtime += EspApConfigurator[SET_TIMEZONE]->get().toInt() * 3600;

    // Additional offset for daylight savings if set
    unixtime += EspApConfigurator[SET_DST]->get().toInt() ? 3600 : 0;
    
    snprintf(buf, 21, "%s %04d-%02d-%02d", 
             DayNames[weekday(unixtime)],
             year(unixtime), 
             month(unixtime), 
             day(unixtime));
    return String(buf);
}

void RealTimeClockClass::clearBuf()
{
    memset(_buf, 0, NtpPacketSize * sizeof(byte));
}


bool RealTimeClockClass::dnsLookup()
{
    DB(F("RealTimeClock::dnsLookup for "));
    DBLN(EspApConfigurator[SET_NTP_SERVER]->get());
    if (!WiFi.hostByName(EspApConfigurator[SET_NTP_SERVER]->get().c_str(), _ntpServerIP)) { // Get the IP address of the NTP server
        DB(F("DNS lookup failure for: "));
        DBLN(EspApConfigurator[SET_NTP_SERVER]->get());
        return false;
    } else {
        DB(F("DNS lookup success for: "));
        DB(EspApConfigurator[SET_NTP_SERVER]->get());
        DB(F(", IP: "));
        DBLN(_ntpServerIP);
        return true;
    }
}

void RealTimeClockClass::sendNtpPacket()
{
    DBLN(F("RealTimeClock::sendNtpPacket"));

    clearBuf();

    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    _buf[0] = 0b11100011;   // LI, Version, Mode
    _buf[1] = 0;            // Stratum, or type of clock
    _buf[2] = 6;            // Polling Interval
    _buf[3] = 0xEC;         // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    _buf[12]  = 49;
    _buf[13]  = 0x4E;
    _buf[14]  = 49;
    _buf[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    _udp.beginPacket(_ntpServerIP, 123); // port 123 = NTP
    _udp.write(_buf, NtpPacketSize);
    _udp.endPacket();
}

uint32_t RealTimeClockClass::secondsUntilNext(String timeStr)
{
    if (!haveRealTime()) {
        return 100000;
    }

    int32_t until = timeStrToSeconds(timeStr) - daySeconds();
    while (until < 0) {
        until += (3600*24);
    }
    return until;
}

