#ifndef TIME_SERVICE_HPP
#define TIME_SERVICE_HPP

#include <Arduino.h>
#include <time.h>

class TimeService {
public:
    /**
     * Initialize the time service and synchronize with NTP server
     * @return true if time synchronization succeeded, false otherwise
     */
    bool begin();
    
    /**
     * Print the current local time to the serial monitor
     */
    void printLocalTime();
    
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
};

#endif // TIME_SERVICE_HPP