#include "rgbIcon.h"


void RGBIcon::startCycle() {
    if (!cycling) {
        cycling = true;
        fading = true;
        showIcon(&rgb0);
        setFadeIcon(&rgb30);
    }
}

void RGBIcon::stopCycle() {
    cycling = false;
    fading = false;
}

void RGBIcon::update() {
    if (cycling) {
        float stage = rate.getStage();
        fadeBias = (stage * 12) - min((int) (stage * 12), 11);
        switch ((int) (stage * 12)) {
            case 0:
                showIcon(&rgb0);
                setFadeIcon(&rgb30);
                break;
            case 1:
                showIcon(&rgb30);
                setFadeIcon(&rgb60);
                break;
            case 2:
                showIcon(&rgb60);
                setFadeIcon(&rgb90);
                break;
            case 3:
                showIcon(&rgb90);
                setFadeIcon(&rgb120);
                break;
            case 4:
                showIcon(&rgb120);
                setFadeIcon(&rgb150);
                break;
            case 5:
                showIcon(&rgb150);
                setFadeIcon(&rgb180);
                break;
            case 6:
                showIcon(&rgb180);
                setFadeIcon(&rgb210);
                break;
            case 7:
                showIcon(&rgb210);
                setFadeIcon(&rgb240);
                break;
            case 8:
                showIcon(&rgb240);
                setFadeIcon(&rgb270);
                break;
            case 9:
                showIcon(&rgb270);
                setFadeIcon(&rgb300);
                break;
            case 10:
                showIcon(&rgb300);
                setFadeIcon(&rgb330);
                break;
            case 11:
                showIcon(&rgb330);
                setFadeIcon(&rgb0);
                break;
        }            
    }
    Serial.println(fadeBias);
}