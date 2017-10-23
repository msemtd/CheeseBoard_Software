#pragma once

#define SET_WAKE_TIME                   "Wake Time"
#define SET_WAKE_DURATION               "Wake Duration"
#define SET_SLEEP_DURATION              "Sleep Duration"
#define SET_MAX_BRIGHTNESS              "Maximum Brightness (percent)"
#define SET_SHOW_SECONDS                "Display Seconds on Clock"
#define SET_LATITUDE                    "Latitude (+ve N, -ve S)"
#define SET_LONGITUDE                   "Longitude (+ve E, -ve W)"
#define SET_TIMEZONE                    "Time Zone (e.g. -5 for EST, 3.5 for IST)"
#define SET_DST                         "Daylight Savings (ON=summer, OFF=winter)"
#define SET_NTP_SERVER                  "Time Server"

#define AP_SSID                         "DawnDuskLight"
#define AP_PASSPHRASE                   "restwell"

// How often to try to sync time with NTP server
const uint32_t NTP_REFRESH_PERIOD_S     = 1800;

// When NTP fails, how long to wait before re-try
const uint32_t NTP_RETRY_PERIOD_S       = 10;


