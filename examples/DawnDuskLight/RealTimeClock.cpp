#include <MutilaDebug.h>
#include <Millis.h>
#include <EspApConfigurator.h>
#include <TimeLib.h>
//#include <TimeLib.h>
#include "RealTimeClock.h"
#include "Config.h"

/*
 * NTP code taken from the TimeNTP_ESP8266WiFi example in TimeLib version 1.5
 * by Michael Margolis & Paul Stoffregen
 * http://playground.arduino.cc/code/time
 *
 */

char* const RealTimeClock_::DayNames[] = {"Noday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturaday"};

RealTimeClock_ RealTimeClock;


RealTimeClock_::RealTimeClock_() :
    _lastNtpAttempt(0),
    _lastNtpSuccess(0),
    _unixTime(0),
    _state(RealTimeClock_::Wait)
{
    setUpdatePeriod(1000);
    clearBuf();
}

void RealTimeClock_::begin()
{
    DBLN(F("RealTimeClock::begin"));
    _udp.begin(LOCAL_NTP_PORT);
}

void RealTimeClock_::modeStart()
{
    DBLN(F("RealTimeClock::modeStart"));
    clearBuf();
}

void RealTimeClock_::modeStop()
{
    DBLN(F("RealTimeClock::modeStop"));
}

void RealTimeClock_::modeUpdate()
{
    // If we managed to set time using NTP in the last NTP_REFRESH_PERIOD_S seconds
    // we don't need to do anything - just return
    if (Millis() < _lastNtpSuccess + (NTP_REFRESH_PERIOD_S*1000) && _lastNtpSuccess != 0) {
        return;
    }

    if (Millis() >= _lastNtpAttempt + (NTP_RETRY_PERIOD_S*1000) || _lastNtpAttempt == 0) {
        ntpUpdate();
    }
}

void RealTimeClock_::ntpUpdate()
{
    //DBLN(F("RealTimeClock_::ntpUpdate"));
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
        if (size >= NTP_PACKET_SIZE) {
            DBLN(F("NTP RX"));
            _udp.read(_buf, NTP_PACKET_SIZE); // read packet into the buffer
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
            _state = NtpWorking;
            return;
        }
    }
    DBLN("NTP failed");
}

time_t RealTimeClock_::unixTime()
{
    if (_unixTime == 0) return 0;
    return _unixTime + ((Millis() - _lastNtpSuccess) / 1000);
}

long RealTimeClock_::daySeconds()
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

String RealTimeClock_::isoTimestamp()
{
    char buf[24];
    snprintf(buf, 24, "%04d-%02d-%02d %02d:%02d:%02d UT", 
             year(unixTime()), 
             month(unixTime()), 
             day(unixTime()), 
             hour(unixTime()), 
             minute(unixTime()), 
             second(unixTime()));
    return String(buf);
}

String RealTimeClock_::timeStr(bool includeSeconds)
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

String RealTimeClock_::dateStr()
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

void RealTimeClock_::clearBuf()
{
    memset(_buf, 0, NTP_PACKET_SIZE * sizeof(byte));
}


bool RealTimeClock_::dnsLookup()
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

void RealTimeClock_::sendNtpPacket()
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
    _lastNtpAttempt = Millis();
    _udp.beginPacket(_ntpServerIP, 123); // port 123 = NTP
    _udp.write(_buf, NTP_PACKET_SIZE);
    _udp.endPacket();
}

