#include "Controller.h"
#include <WiFi.h>


/*
 * DESCRIPTION: Initializes the hardware on the controller. This function should
 *              be called after board initialization.
 */
void Controller::init() {
    // enable serial debugging
    Serial.begin(115200);

    // enable battery and power sensors
    pinMode(PIN_IN, INPUT);
    pinMode(PIN_POWER, INPUT);
    pinMode(PIN_BATTERY, INPUT);

    // enable control wheel pin
    pinMode(PIN_WHEEL, INPUT);

    // initialize buttons
    mainButton.init();
    leftButton.init();
    rightButton.init();

    // set normal performance
    _powerState = STATE_DISCHARGING;
    setCpuFrequencyMhz(240);

    // enable wifi
    WiFi.mode(WIFI_AP);
}

/*
 * DESCRIPTION: Updates all hardware related functionality and should be called
 *              on a regular basis.
 */
void Controller::update() {
    if (sampleRate.isReady()) {
        // update buttons
        mainButton.update();
        leftButton.update();
        rightButton.update();

        // update control wheel
        _wheelValue = 1 - readAnalog(PIN_WHEEL) / 3.3f;

        // update power settings
        if (batterySampleRate.isReady()) {
            _batteryVoltage = readAnalog(PIN_BATTERY) * 1.3333f;
            charging = digitalRead(PIN_IN) == 1;

            // filter battery voltage and set battery level
            int temp = -18;
            if (_batteryVoltage <= 3.6f)
                temp = -10;
            float newBV = 1 / (1 + exp(temp * (_batteryVoltage - 3.6f)));
            _batteryLevel = _batteryLevel * 0.6f + 0.4f * newBV;

            bool newPowerState = STATE_DISCHARGING;
            if (charging)
                newPowerState = STATE_CHARGING;
            else if (_batteryLevel <= BATTERY_LOW)
                newPowerState = STATE_POWER_SAVING;

            // if new power state is different from current one, update
            // cpu speed accordingly to keep thermals and performance in check
            if (newPowerState != _powerState) {
                switch (newPowerState) {
                    case STATE_DISCHARGING:
                        setCpuFrequencyMhz(240);
                        break;
                    case STATE_CHARGING:
                    case STATE_POWER_SAVING:
                        setCpuFrequencyMhz(80);
                        break;
                }
                _powerState = newPowerState;
                _powerStateChanged = true;
            }
        }

        int scanned = WiFi.scanComplete();
        if (scanned == -2)
            WiFi.scanNetworks(true, true);
        else if (scanned >= 0) {
            _wifiNetworks = scanned;
            WiFi.scanDelete();
        }
    }
}

/*
 * DESCRIPTION: Getter function for wheel value
 * RETURNS:     value of wheel on a scale from 0 to 1
 */
float Controller::wheelValue() { return _wheelValue; }

/*
 * DESCRIPTION: Getter function for battery voltage
 * RETURNS:     battery voltage
 */
float Controller::batteryVoltage() { return _batteryVoltage; }

/*
 * DESCRIPTION: Getter function for battery level
 * RETURNS:     battery level (based on VOLTAGE_MIN and VOLTAGE_MAX)
 */
float Controller::batteryLevel() { return _batteryLevel; }

/*
 * DESCRIPTION: Getter function for checking if power state has changed
 * RETURNS:     true if power state has changed else false
 */
bool Controller::powerStateChanged() { return _powerStateChanged; }

/*
 * DESCRIPTION: Getter function for power state
 * RETURNS:     power state
 */
int Controller::powerState() {
    if (_powerStateChanged)
        _powerStateChanged = false;
    return _powerState;
}

/*
 * DESCRIPTION: Prints performance statistics to serial for debugging
 */
void Controller::printStats() {
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println(":::::::::::::: HARDWARE STATS ::::::::::::::");
    Serial.print("Battery Voltage  -> ");
    Serial.print(_batteryVoltage);
    Serial.print(" V (");
    Serial.print(100 * _batteryLevel);
    Serial.println("%)");
    Serial.print("Core Temperature -> ");
    Serial.print(coreTemperature());
    Serial.println(" C");
    Serial.print("Memory Usage     -> ");
    Serial.print(memoryUsage());
    Serial.print(" bytes (");
    Serial.print(memoryUsagePercent());
    Serial.println("%)");
    Serial.print("WiFi Networks    -> ");
    Serial.println(_wifiNetworks);
}
