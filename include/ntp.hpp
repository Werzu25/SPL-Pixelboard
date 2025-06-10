#ifndef NTP_HPP
#define NTP_HPP

#include <ctime>

class Ntp {
public:
    bool begin();
    void printLocalTime();
    bool updateTime();
    time_t getCurrentTime();
    struct tm getCurrentTimeStruct();
};

#endif
