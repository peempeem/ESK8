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

void Rate::sleep() {
    int time = millis();
    int stime = time - last - inverseRate;
    if (stime < 0)
        vTaskDelay(-stime / portTICK_PERIOD_MS);
}

void Rate::reset() { last = millis(); }

/*
 * DESCRIPTION: Gets the current progress towards the next scheduled time on
 *              a scaled of 0 to 1
 * INPUTS:      noChange    - if set, doesn't update last
 * RETURNS:     progress (0 to 1)
 */
float Rate::getStage(bool noChange) {
    if (!enabled)
        return 0;
    int time = millis();
    float stage = (time - last) / (float) inverseRate;
    if (stage > 1) {
        stage = 1;
        if (!noChange)
            last = time;
    }
    return stage;
}

/*
 * DESCRIPTION: Gets the current stage but in sin format
 * INPUTS:      noChange    - if set, doesn't update last
 * RETURNS:     sin(progress)
 */
float Rate::getStageSin(bool noChange) { return sin(getStage(noChange) * 2 * PI); }

/*
 * DESCRIPTION: Gets the current stage but in cos format
 * INPUTS:      noChange    - if set, doesn't update last
 * RETURNS:     cos(progress)
 */
float Rate::getStageCos(bool noChange) { return cos(getStage(noChange) * 2 * PI); }

Timer::Timer() { }

Timer::Timer(int ms) { set(ms); }

bool Timer::is_set() { return _is_set; }

void Timer::set(int ms) {
    start_time = millis();
    ring_time = ms + start_time;
    _is_set = true;
}

bool Timer::is_ringing() {
    if (_is_set && millis() >= ring_time)
        return true;
    return false;
}

void Timer::ring() { ring_time == millis(); }

float Timer::progress() {
    if (_is_set) {
        int time = millis();
        if (time >= ring_time)
            return 1;
        else
            return (time - start_time) / (float) (ring_time - start_time);
    }
    return 0;
}