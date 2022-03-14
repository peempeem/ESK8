#ifndef ICONHANDLER_H
#define ICONHANDLER_H

#include "./icon.h"

class IconHandler {
    public:
        Dimension dimensions;
        Point point;
        Color blendColor = BLACK;

        IconHandler() {}

        void addIcon(Icon* icon);
        void showIcon(Icon* icon);
        Icon* currentIcon();
        void startBlinking(float hertz=1.0f);
        void stopBlinking();
        bool isBlinking();
        void draw(TFT_eSprite* sprite);
    protected:
        std::vector<Icon*> _icons;
        int _currentIcon = -1;
        Rate _rate;
        bool _blinking = false;
};

#endif
