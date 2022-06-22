#include "iconHandler.h"

void IconHandler::addIcon(Icon* icon) {
    for (Icon* i : _icons) {
        if (i == icon)
            return;
    }
    _icons.push_back(icon);
}

void IconHandler::setIcons(std::vector<Icon*> icons) { _icons = icons; }

void IconHandler::showIcon(Icon* icon) {
    for (int i = 0; i < _icons.size(); i++) {
        if (_icons[i] == icon) {
            _currentIcon = i;
            icon->allocate(true);
            return;
        }
    }
}

Icon* IconHandler::currentIcon() {
    if (_currentIcon < 0)
        return NULL;
    return _icons[_currentIcon];
}

void IconHandler::setCycleIcons(std::vector<Icon*> cycleIcons) {
    if (!cycling)
        _cycleIcons = cycleIcons;
}

void IconHandler::startCycling() {
    if (!cycling && _cycleIcons.size() > 1) {
        cycling = true;
        cycleRate.reset();
    }
}

void IconHandler::stopCycling() { cycling = false; }

bool IconHandler::isCycling() { return cycling; }

void IconHandler::startBlinking() { blinking = true; }

void IconHandler::stopBlinking() { blinking = false; }

bool IconHandler::isBlinking() { return blinking; }

void IconHandler::allocate() {
    for (Icon* icon : _icons)
        icon->allocate(true);
}

void IconHandler::deallocate() {
    for (Icon* icon : _icons)
        icon->deallocate();
}

void IconHandler::draw(TFT_eSprite* sprite, int priority) {
    if (!cycling && _currentIcon < 0)
        return;
    
    float   blinkBias;
    float   cycleBias;
    Icon*   icon;
    Icon*   cIcon;

    if (blinking)
        blinkBias = maxBlinkBias * (1 - blinkRate.getStageCos()) / 2.0f;
    
    if (cycling) {
        float stage = cycleRate.getStage();
        float sp = stage * _cycleIcons.size();
        int s = (int) sp;
        cycleBias = sp - s;
        if (!linearCycle)
            cycleBias = 1 / (1 + exp(-10 * (cycleBias - 0.5f)));
        if (s == _cycleIcons.size())
            s = 0;
        icon = _cycleIcons[s];
        if (s < _cycleIcons.size() - 1)
            cIcon = _cycleIcons[s + 1];
        else 
            cIcon = _cycleIcons[0];
        cIcon->allocate(true, priority);

        if (s < _cycleIcons.size() - 2)
            _cycleIcons[s + 2]->allocate(true, priority);
        else
            _cycleIcons[1]->allocate(true, priority);
    } else
        icon = _icons[_currentIcon];
    icon->allocate(true, priority);

    if (!cycling || cIcon->bitmap() == NULL) {
        const uint16_t* bitmap = icon->bitmap();
        if (bitmap == NULL)
            return;
        for (int dy = 0; dy < dimensions.height; dy++) {
            int sy = (dy * icon->height()) / dimensions.height;
            int ddy = point.y + dy;
            for (int dx = 0; dx < dimensions.width; dx++) {
                int sx = (dx * icon->width()) / dimensions.width;
                int ddx = point.x + dx;
                uint16_t c15 = bitmap[icon->width() * sy + sx];
                if (!c15IsTransparent(c15)) {
                    uint16_t c16 = cvtC15toC16(c15);
                    if (blinking) {
                        Color cb = Color(sprite->readPixel(ddx, ddy));
                        c16 = Color(c16).blend(cb, blinkBias).as16Bit();
                    }            
                    sprite->drawPixel(ddx, ddy, c16);
                }
            }
        }
    } else {
        const uint16_t* bitmap1 = icon->bitmap();
        const uint16_t* bitmap2 = cIcon->bitmap();
        if (bitmap1 == NULL)
            return;
        for (int dy = 0; dy < dimensions.height; dy++) {
            int sy1 = (dy * icon->height()) / dimensions.height;
            int sy2 = (dy * cIcon->height()) / dimensions.height;
            int ddy = point.y + dy;
            for (int dx = 0; dx < dimensions.width; dx++) {
                int sx1 = (dx * icon->width()) / dimensions.width;
                int sx2 = (dx * cIcon->width()) / dimensions.width;
                int ddx = point.x + dx;
                uint16_t c15_1 = bitmap1[icon->width() * sy1 + sx1];
                uint16_t c15_2 = bitmap2[cIcon->width() * sy2 + sx2];
                bool t1 = c15IsTransparent(c15_1);
                bool t2 = c15IsTransparent(c15_2);
                if (t1 && t2)
                    continue;
                Color c;
                Color cb = Color(sprite->readPixel(ddx, ddy));
                // fade from background to c2
                if (t1) {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = cb.blend(c2, cycleBias);
                }
                // fade to background from c1
                else if (t2)
                    c = Color(cvtC15toC16(c15_1)).blend(cb, cycleBias);
                // fade from c1 to c2
                else {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = Color(cvtC15toC16(c15_1)).blend(c2, cycleBias);
                }
                if (blinking)
                    c = c.blend(cb, blinkBias);
                sprite->drawPixel(ddx, ddy, c.as16Bit());
            }
        }
    }
}
