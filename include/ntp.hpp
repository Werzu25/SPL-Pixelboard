#ifndef NTP_HPP
#define NTP_HPP

#include <Arduino.h>
#include <time.h>
/**
 * Initialize the time service and synchronize with NTP server
 * @return true if time synchronization succeeded, false otherwise
 */
bool begin();

/**
 * Update the time from NTP server
 * @return true if time update succeeded, false otherwise
 */
bool updateTime();

/**
 * Get the current time as a time_t value
 * @return current time
 */
time_t getCurrentTime();

/**
 * Get the current time as a tm struct
 * @return current time struct
 */
struct tm getCurrentTimeStruct();

#endif // TIME_SERVICE_HPP