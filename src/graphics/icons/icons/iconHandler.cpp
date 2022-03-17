#include "iconHandler.h"

void IconHandler::addIcon(Icon* icon) {
    for (Icon* i : _icons) {
        if (i == icon)
            return;
    }
    _icons.push_back(icon);
}

void IconHandler::showIcon(Icon* icon) {
    for (int i = 0; i < _icons.size(); i++) {
        if (_icons[i] == icon) {
            _currentIcon = i;
            return;
        }
    }
}

void IconHandler::setFadeIcon(Icon* icon) {
    for (int i = 0; i < _icons.size(); i++) {
        if (_icons[i] == icon) {
            _fadeIcon = i;
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
        blinkRate.setRate(hertz);
        _blinking = true;
        for (Icon* icon : _icons) {
            icon->backgroundColor = backgroundColor;
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
        float bias = 0.5f - blinkRate.getStageCos() / 2.0f;
        if (_blinking)
            icon->blendBias = bias;
        if (fading && !(_fadeIcon == -1)) {
            icon->fading = true;
            icon->fadeBias = fadeBias;
            icon->fadeIcon = _icons[_fadeIcon];
        }
        icon->draw(sprite);
        if (_blinking)
            icon->blendBias = 0.0f;
        if (fading) {
            icon->fading = false;
            icon->fadeBias = 0.0f;
            icon->fadeIcon = NULL;
        }
    }
}
