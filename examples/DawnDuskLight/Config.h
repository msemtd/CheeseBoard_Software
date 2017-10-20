#pragma once

#define SET_NTP_SERVER                  "Time Server"
#define SET_LATITUDE                    "Latitude (+ve N, -ve S)"
#define SET_TIMEZONE                    "Time Zone"

// How often to try to sync time with NTP server
const uint32_t NTP_REFRESH_PERIOD_S     = 1800;

// When NTP fails, how long to wait before re-try
const uint32_t NTP_RETRY_PERIOD_S       = 10;

