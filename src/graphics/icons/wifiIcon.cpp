#include "wifiIcon.h"


void WiFiIcon::startCycle() {
    if (!cycling) {
        cycling = true;
        currentIcon = 0;
    }
}

void WiFiIcon::stopCycle() { cycling = false; }

void WiFiIcon::update() {
    if (cycling && rate.isReady()) {
        switch (currentIcon++) {
            case 0:
                showIcon(&wifi1);
                break;
            case 1:
                showIcon(&wifi2);
                break;
            case 2:
                showIcon(&wifi3);
                break;
            case 3:
                showIcon(&wifiFull);
                currentIcon = 0;
                break;
        }
    }
}