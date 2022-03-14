#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

#define MAX_MEMORY          520000  // max dynamic memory of esp32

// base hardware class
class Hardware {
    public:
        Hardware() {}

        int     memoryUsage();
        float   memoryUsagePercent();
        float   coreTemperature();
};

#endif