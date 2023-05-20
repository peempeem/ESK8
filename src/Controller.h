#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "util/rate.h"
#include "util/Log.h"
#include "hardware/hardware.h"
#include "hardware/adc.h"
#include "hardware/button.h"
#include "hardware/wifi/MACLinkListener.h"

#define PIN_IN              34  // pin for board input voltage
#define PIN_POWER           14  // pin for chip voltage
#define PIN_BATTERY         39
#define PIN_WHEEL           37
#define PIN_MAIN_BUTTON     13
#define PIN_LEFT_BUTTON     35
#define PIN_RIGHT_BUTTON    0

#define STATE_DISCHARGING   0
#define STATE_CHARGING      1
#define STATE_POWER_SAVING  2

#define VOLTAGE_MAX         4.2f    // battery max voltage
#define VOLTAGE_MIN         3.3f    // battery min voltage

#define BATTERY_LOW         0.2f

// class for interfacing with hardware on the controller
class Controller : public Hardware {
    public:
        Rate    sampleRate = Rate(100);         // hardware main sample rate
        Rate    batterySampleRate = Rate(4);    // power-related sample rate

        Button  mainButton = Button(PIN_MAIN_BUTTON);
        Button  leftButton = Button(PIN_LEFT_BUTTON);
        Button  rightButton = Button(PIN_RIGHT_BUTTON);

        Controller() {}

        void    init();
        void    update(bool sleep=false);
        float   wheelValue();
        float   batteryVoltage();
        float   batteryLevel();
        bool    powerStateChanged();
        int     powerState();
        void    printStats();
        void    consumeTaps();

    private:
        float   _wheelValue = 0.0f;         // 0 to 1 value of control wheel
        float   _batteryVoltage = 0.0f;     // voltage of battery
        float   _batteryLevel = 0.0f;       // 0 to 1 value of battery

        bool    charging = false;                   // charging flag
        int     _powerState = STATE_DISCHARGING;    // current power state
        bool    _powerStateChanged = false;         // power state change flag
} static controller;

#endif
