#ifndef HARDWARE_H
#define HARDWARE_H

#include "../rate.h"
#include "button.h"


#define PIN_IN              34  // pin for board input voltage
#define PIN_POWER           14  // pin for chip voltage
#define PIN_BATTERY         39
#define PIN_WHEEL           37
#define PIN_MAIN_BUTTON     13
#define PIN_LEFT_BUTTON     35
#define PIN_RIGHT_BUTTON    0

#define MAX_MEMORY          520000  // max dynamic memory of esp32

#define STATE_DISCHARGING   0
#define STATE_CHARGING      1
#define STATE_POWER_SAVING  2

#define VOLTAGE_MAX         4.2f    // battery max voltage
#define VOLTAGE_MIN         3.3f    // battery min voltage

#define BATTERY_LOW         0.2f


// class for interfacing with hardware on the controller
class Hardware {
    public:
        Rate    sampleRate = Rate(100);         // hardware main sample rate
        Rate    batterySampleRate = Rate(4);    // power-related sample rate

        Button  mainButton = Button(PIN_MAIN_BUTTON);
        Button  leftButton = Button(PIN_LEFT_BUTTON);
        Button  rightButton = Button(PIN_RIGHT_BUTTON);

        Hardware() {}

        void    init();
        void    update();
        float   wheelValue();
        float   batteryVoltage();
        float   batteryLevel();
        int     memoryUsage();
        float   memoryUsagePercent();
        float   coreTemperature();
        bool    powerStateChanged();
        int     powerState();
        void    printStats();

    private:
        float   _wheelValue = 0.0f;         // 0 to 1 value of control wheel
        float   _batteryVoltage = 0.0f;     // voltage of battery
        float   _batteryLevel = 0.0f;       // 0 to 1 value of battery

        bool    charging = false;                   // charging flag
        int     _powerState = STATE_DISCHARGING;    // current power state
        bool    _powerStateChanged = false;         // power state change flag
        int     _wifiNetworks = 0;
} static hardware;

float readAnalog(int pin);

#endif
