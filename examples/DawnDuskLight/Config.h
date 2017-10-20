#pragma once

#define SET_WAKE_TIME                   "Wake Time"
#define SET_WAKE_DURATION               "Wake Duration"
#define SET_SLEEP_DURATION              "Sleep Duration"
#define SET_LATITUDE                    "Latitude (+ve N, -ve S)"
#define SET_DST                         "Daylight Savings (ON=summer, OFF=winter)"
#define SET_TIMEZONE                    "Time Zone"
#define SET_NTP_SERVER                  "Time Server"

#define AP_SSID                         "DawnDuskLight"
#define AP_PASSPHRASE                   "restwell"

// How often to try to sync time with NTP server
const uint32_t NTP_REFRESH_PERIOD_S     = 1800;

// When NTP fails, how long to wait before re-try
const uint32_t NTP_RETRY_PERIOD_S       = 10;


