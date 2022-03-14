#include "rate.h"

Rate::Rate(float rate) { inverseRate = 1000 / rate; }

void Rate::setRate(float rate) {
    inverseRate = 1000 / rate;
    enabled = true;
}

bool Rate::isReady() {
    if (!enabled)
        return false;
    int t = millis();
    if (t - last > inverseRate) {
        last = t;
        return true;
    }
    return false;
}

float Rate::getStage() {
    if (!enabled)
        return 0;
    int t = millis();
    int diff = t - last;
    float stage = diff / (float) inverseRate;
    if (stage > 1) {
        stage = 1;
        last = t;
    }
    return stage;
}
