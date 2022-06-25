#include "Controller.h"

const static char* LOG_HEADER = "Controller";

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
}

void Controller::update(bool sleep) {
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

        /*int scanned = WiFi.scanComplete();
        if (scanned == -2)
            WiFi.scanNetworks(true, true);
        else if (scanned >= 0) {
            _wifiNetworks = scanned;
            WiFi.scanDelete();
        }*/
        if (sleep)
            sampleRate.sleep();
    }
}

float Controller::wheelValue() { return _wheelValue; }

float Controller::batteryVoltage() { return _batteryVoltage; }

float Controller::batteryLevel() { return _batteryLevel; }

bool Controller::powerStateChanged() { return _powerStateChanged; }

int Controller::powerState() {
    if (_powerStateChanged)
        _powerStateChanged = false;
    return _powerState;
}

void Controller::printStats() {
    log(DEBUG, LOG_HEADER, ":::::::::::::: HARDWARE STATS ::::::::::::::");
    log(DEBUG, LOG_HEADER, "Battery Voltage  -> ", false);
    logc(_batteryVoltage, false);
    logc(" V (", false);
    logc(100 * _batteryLevel, false);
    logc("%)");
    log(DEBUG, LOG_HEADER, "Core Temperature -> ", false);
    logc(coreTemperature(), false);
    logc(" C");
    log(DEBUG, LOG_HEADER, "Memory Usage     -> ", false);
    logc(memoryUsage(), false);
    logc(" bytes (", false);
    logc(memoryUsagePercent(), false);
    logc("%)");
}
