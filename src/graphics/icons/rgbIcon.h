#ifndef RGBICON_H
#define RGBICON_H

#include "iconHandler.h"

class RGBIcon : public IconHandler {
    public:
        Icon _0 = Icon("/icons/RGB0.icon");
        Icon _30 = Icon("/icons/RGB30.icon");
        Icon _60 = Icon("/icons/RGB60.icon");
        Icon _90 = Icon("/icons/RGB90.icon");
        Icon _120 = Icon("/icons/RGB120.icon");
        Icon _150 = Icon("/icons/RGB150.icon");
        Icon _180 = Icon("/icons/RGB180.icon");
        Icon _210 = Icon("/icons/RGB210.icon");
        Icon _240 = Icon("/icons/RGB240.icon");
        Icon _270 = Icon("/icons/RGB270.icon");
        Icon _300 = Icon("/icons/RGB300.icon");
        Icon _330 = Icon("/icons/RGB330.icon");

        Rate rate = Rate(0.1f);

        RGBIcon() {
            std::vector<Icon*> icons {
                &_0,
                &_30,
                &_60,
                &_90,
                &_120,
                &_150,
                &_180,
                &_210,
                &_240,
                &_270,
                &_300,
                &_330
            };
            setIcons(icons);
            setCycleIcons(icons);
            showIcon(&_0);
        };
};

#endif