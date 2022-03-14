#include "rate.h"
#include <Arduino.h>

/*
 * DESCRIPTION: Default Rate class constructor. Rate is disabled out 
 *              of the box.
 */
Rate::Rate() { enabled = false; };

/*
 * DESCRIPTION: Constructs the Rate class with a frequency. Rate is enabled
                out of the box.
 * INPUTS:      rate - frequency of intended task
 */
Rate::Rate(float rate) { inverseRate = 1000 / rate; }

/*
 * DESCRIPTION: Constructs the Rate class with a frequency
 * INPUTS:      rate - frequency of intended task
 */
void Rate::setRate(float rate) {
    inverseRate = 1000 / rate;
    enabled = true;
}

/*
 * DESCRIPTION: Checks if the right amount of time has passed since last
 *              schedule
 * RETURNS:     true if task is approved to run, else false
 */
bool Rate::isReady() {
    if (!enabled)
        return false;
    int time = millis();
    if (time - last > inverseRate) {
        last = time;
        return true;
    }
    return false;
}

/*
 * DESCRIPTION: Gets the current progress towards the next scheduled time on
 *              a scaled of 0 to 1
 * RETURNS:     progress (0 to 1)
 */
float Rate::getStage() {
    if (!enabled)
        return 0;
    int time = millis();
    float stage = (time - last) / (float) inverseRate;
    if (stage > 1) {
        stage = 1;
        last = time;
    }
    return stage;
}

/*
 * DESCRIPTION: Gets the current stage but in sin format
 * RETURNS:     sin(progress)
 */
float Rate::getStageSin() { return sin(getStage() * 2 * PI); }

/*
 * DESCRIPTION: Gets the current stage but in cos format
 * RETURNS:     cos(progress)
 */
float Rate::getStageCos() { return cos(getStage() * 2 * PI); }