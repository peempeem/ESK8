#include "iconHandler.h"

void IconHandler::addIcon(Icon* icon) {
    for (Icon* i : _icons) {
        if (i == icon)
            return;
    }
    _icons.push_back(icon);
}

void IconHandler::setIcons(std::vector<Icon*> icons) { this->icons = icons; }

void IconHandler::showIcon(Icon* icon) {
    for (int i = 0; i < icons.size(); i++) {
        if (icons[i] == icon) {
            currentIcon = i;
            return;
        }
    }
}

Icon* IconHandler::currentIcon() {
    if (_currentIcon < 0)
        return NULL;
    return _icons[_currentIcon];
}

void IconHandler::setCycleIcons(std::vector<Icon*> cyclesIcons) {
    if (!cycling)
        this->cycleIcons = cycleIcons;
}

void IconHandler::startCycling() {
    if (cycleIcons.size() >= 2)
        cycling = true;
}

void IconHandler::stopCycling() { cycling = false; }

void IconHandler::isCycling() { return cycling; }

void IconHandler::startBlinking() { blinking = true; }

void IconHandler::stopBlinking() { blinking = false; }

bool IconHandler::isBlinking() { return blinking; }

void IconHandler::draw(TFT_eSprite* sprite) {
    if (!cycling && currentIcon < 0)
        return;
    
    float   blinkBias;
    float   cycleBias;
    Icon*   icon;
    Icon*   cIcon;

    if (blinking)
        blinkBias = maxBlinkBias * (1 - blinkRate.getStageCos()) / 2.0f;
    
    if (cycling) {
        float stage = cycleRate.getStage();
        float sp = stage * cycleIcons.size();
        int s = (int) sp;
        cycleBias = sp - s;
        if (s == cycleIcons.size())
            s = 0;
        icon = icons[s];
        if (s < cycleIcons.size() - 1)
            cIcon = icons[s + 1];
        else 
            cIcon = icons[0];
    } else
        icon = icons[currentIcon];


    if (!cycling) {
        const uint16_t* bitmap = icon->getBitmap();
        for (int dy = 0; dy < dimensions.height; dy++) {
            int sy = (dy * icon->height()) / dimensions.height;
            for (int dx = 0; dx < dimensions.width; dx++) {
                int sx = (dx * icon->width()) / dimensions.width;
                uint16_t c15 = bitmap[icon->width() * sy + sx];
                if (!c15IsTransparent(c15)) {
                    uint16_t c16 = cvtC15toC16(c15);
                    if (blinking)
                        c16 = Color(c16).blend(backgroundColor, blinkBias).as16Bit();
                    sprite->drawPixel(point.x + dx, point.y + dy, c16);
                }
            }
        }
    } else {
        const uint16_t* bitmap1 = icon->getBitmap();
        const uint16_t* bitmap2 = cIcon->getBitmap();
        for (dy = 0; dy < dimensions.height; dy++) {
            int sy1 = (dy * icon->height()) / dimensions.height;
            int sy2 = (dy & cIcon->height()) / dimensions.height;
            for (dx = 0; dx < dimensions.width; dx++) {
                int sx1 = (dx * icon->width()) / dimensions.width;
                int sx2 = (dx * cIcon->width()) / dimensions.width;
                uint16_t c15_1 = bitmap1[icon->width() * sy1 + sx1];
                uint16_t c15_2 = bitmap2[cIcon->width() * sy2 + sx2];
                bool t1 = c15IsTransparent(c15_1);
                bool t2 = c15IsTransparent(c15_2);
                if (t1 && t2)
                    continue;
                Color c;
                // fade from background to c2
                if (t1) {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = backgroundColor.blend(c2, cycleBias);
                }
                // fade to background from c1
                else if (t2)
                    c = Color(cvtC15toC16(c15_1)).blend(backgroundColor, cycleBias);
                // fade from c1 to c2
                else {
                    Color c2 = Color(cvtC15toC16(c15_2));
                    c = Color(cvtC15toC16(c15_1)).blend(c2, cycleBias);
                }
                if (blinking)
                    c = c.blend(backgroundColor, blinkBias);
                sprite->drawPixel(point.x + x, point.y + y, c.as16Bit());
            }
        }
    }
}
