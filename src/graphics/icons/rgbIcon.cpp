#include "rgbIcon.h"


void RGBIcon::startCycle() {
    if (!cycling) {
        cycling = true;
        fading = true;
        currentIcon = 0;
        showIcon(&rgb0);
        setFadeIcon(&rgb60);
    }
}

void RGBIcon::stopCycle() {
    cycling = false;
    fading = false;
}

void RGBIcon::update() {
    if (cycling && rate.isReady()) {
        switch (currentIcon++) {
            case 0:
                showIcon(&rgb0);
                setFadeIcon(&rgb60);
                break;
            case 1:
                showIcon(&rgb60);
                setFadeIcon(&rgb120);
                break;
            case 2:
                showIcon(&rgb120);
                setFadeIcon(&rgb180);
                break;
            case 3:
                showIcon(&rgb180);
                setFadeIcon(&rgb240);
                break;
            case 4:
                showIcon(&rgb240);
                setFadeIcon(&rgb300);
                break;
            case 5:
                showIcon(&rgb300);
                setFadeIcon(&rgb0);
                currentIcon = 0;
                break;
        }
    }
    fadeBias = rate.getStage(true);
    Serial.println(fadeBias);
}