#ifndef WIFIICON_H
#define WIFIICON_H

#include "iconHandler.h"

class WiFiIcon : public IconHandler {
    public:
        Icon wifiNone = Icon("/icons/WiFiNone.icon");
        Icon wifi1 = Icon("/icons/WiFi1.icon");
        Icon wifi2 = Icon("/icons/WiFi2.icon");
        Icon wifi3 = Icon("/icons/WiFi3.icon");
        Icon wifiFull = Icon("/icons/WiFiFull.icon");

        WiFiIcon() {
            std::vector<Icon*> icons = {
                &wifi1,
                &wifi2,
                &wifi3,
                &wifiFull
            };
            setIcons(icons);
            setCycleIcons(icons);
            addIcon(&wifiNone);
            showIcon(&wifiNone);
        };
};

#endif