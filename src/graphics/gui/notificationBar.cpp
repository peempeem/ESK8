#include "notificationBar.h"

NotificationBar::NotificationBar() {
    battery.blendColor = backgroundColor;
    wifi.blendColor = backgroundColor;
    icons.push_back(&battery);
    icons.push_back(&wifi);
}

void NotificationBar::addIcon(IconHandler* icon) {
    for (IconHandler* i : icons) {
        if (i == icon)
            return;
    }
    icon->blendColor = backgroundColor;
    icons.push_back(icon);
}

void NotificationBar::removeIcon(IconHandler* icon) {
    for (int i = 0; i < icons.size(); i++) {
        if (icons[i] == icon && icons[i] != &battery && icons[i] != &wifi)
            icons.erase(icons.begin() + i);
    }
}

void NotificationBar::draw(TFT_eSprite* sprite) {
    sprite->fillRect(0, 0, dimensions.width, dimensions.height, backgroundColor.as16Bit());
    int x = dimensions.width - dimensions.height;
    for (IconHandler* icon : icons) {
        icon->dimensions.width = dimensions.height;
        icon->dimensions.height = dimensions.height;
        icon->point.x = x;
        icon->point.y = 0;
        icon->draw(sprite);
        x -= dimensions.height;
    }
}
