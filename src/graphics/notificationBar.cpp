#include "notificationBar.h"

NotificationBar::NotificationBar() {
    batteryIcon.blendColor = backgroundColor;
    wifiIcon.blendColor = backgroundColor;
    _icons.push_back(&batteryIcon);
    _icons.push_back(&wifiIcon);
}

void NotificationBar::addIcon(IconHandler* icon) {
    for (IconHandler* i : _icons) {
        if (i == icon)
            return;
    }
    icon->blendColor = backgroundColor;
    _icons.push_back(icon);
}

void NotificationBar::removeIcon(IconHandler* icon) {
    for (int i = 0; i < _icons.size(); i++) {
        if (_icons[i] == icon && _icons[i] != &batteryIcon && _icons[i] != &wifiIcon)
            _icons.erase(_icons.begin() + i);
    }
}

void NotificationBar::draw(TFT_eSprite* sprite) {
    sprite->fillRect(0, 0, dimensions.width, dimensions.height, backgroundColor.as16Bit());
    int x = dimensions.width - dimensions.height;
    for (IconHandler* icon : _icons) {
        icon->dimensions.width = dimensions.height;
        icon->dimensions.height = dimensions.height;
        icon->point.x = x;
        icon->point.y = 0;
        icon->draw(sprite);
        x -= dimensions.height;
    }
}
