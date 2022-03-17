#ifndef ICONHANDLER_H
#define ICONHANDLER_H

#include "./icon.h"

class IconHandler {
    public:
        Dimension dimensions;
        Point point;
        Color backgroundColor = BLACK;
        bool fading = false;
        float fadeBias = 0.0f;

        IconHandler() {}

        void addIcon(Icon* icon);
        void showIcon(Icon* icon);
        void setFadeIcon(Icon* icon);
        Icon* currentIcon();
        void startBlinking(float hertz=1.0f);
        void stopBlinking();
        bool isBlinking();
        void draw(TFT_eSprite* sprite);
    protected:
        std::vector<Icon*> _icons;
        Rate blinkRate;
        int _currentIcon = -1;
        int _fadeIcon = -1;
        bool _blinking = false;
};

#endif
