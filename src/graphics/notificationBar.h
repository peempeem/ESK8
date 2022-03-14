#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include "./icons/batteryIcon.h"
#include "./icons/wifiIcon.h"

class NotificationBar {
    public:
        Dimension dimensions;
        BatteryIcon batteryIcon;
        WiFiIcon wifiIcon;
        Color backgroundColor = ALMOST_BLACK;

        NotificationBar();

        void addIcon(IconHandler* icon);
        void removeIcon(IconHandler* icon);
        void draw(TFT_eSprite* sprite);
    private:
        std::vector<IconHandler*> _icons;
};

#endif
