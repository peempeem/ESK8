#include "iconHandler.h"

void IconHandler::addIcon(Icon* icon) {
    for (Icon* i : _icons) {
        if (i == icon)
            return;
    }
    _icons.push_back(icon);
}

void IconHandler::showIcon(Icon* icon) {
    if (icon == _icons[_currentIcon])
        return;
    for (int i = 0; i < _icons.size(); i++) {
        if (_icons[i] == icon) {
            _currentIcon = i;
            return;
        }
    }
}

Icon* IconHandler::currentIcon() {
    if (_currentIcon < 0)
        return NULL;
    return _icons[_currentIcon];
}

void IconHandler::startBlinking(float hertz) {
    if (!_blinking) {
        _rate.setRate(hertz);
        _blinking = true;
        for (Icon* icon : _icons) {
            icon->blendColor = blendColor;
            icon->blending = true;
        }
    }
}

void IconHandler::stopBlinking() {
    _blinking = false;
    for (Icon* icon : _icons)
        icon->blending = false;
}

bool IconHandler::isBlinking() { return _blinking; }

void IconHandler::draw(TFT_eSprite* sprite) {
    if (_currentIcon >= 0) {
        Icon* icon = _icons[_currentIcon];
        icon->dimensions = dimensions;
        icon->point = point;
        if (_blinking)
            icon->blendBias = 0.5f - _rate.getStageCos() / 2.0f;
        icon->draw(sprite);
    }
}
