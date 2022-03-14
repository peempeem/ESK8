#include "hardware.h"

// include esp32 function to read core temperature
extern "C" {
  uint8_t temprature_sens_read();
}

// function to read core temperature
uint8_t temprature_sens_read();

// dummy constructor
Hardware::Hardware() {}

/*
 * DESCRIPTION: Calculates current dynamic memory usage using esp32 function
 * RETURNS:     memory usage
 */
int Hardware::memoryUsage() { return MAX_MEMORY - ESP.getFreeHeap(); }

/*
 * DESCRIPTION: Calculates current dynamic memory use percentage using 
 *              esp32 function
 * RETURNS:     memory use percentage
 */
float Hardware::memoryUsagePercent() { return 100 * (1 - ESP.getFreeHeap() / (float) MAX_MEMORY); }

/*
 * DESCRIPTION: Calculates current core temperature in degrees Celsius using
                esp32 function
 * RETURNS:     core temperature
 */
float Hardware::coreTemperature() { return (temprature_sens_read() - 32) / 1.8f; }