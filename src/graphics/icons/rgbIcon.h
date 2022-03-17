#ifndef RGBICON_H
#define RGBICON_H

#include "icons/iconHandler.h"
#include "icons/bitmaps/RGB0.h"
#include "icons/bitmaps/RGB60.h"
#include "icons/bitmaps/RGB120.h"
#include "icons/bitmaps/RGB180.h"
#include "icons/bitmaps/RGB240.h"
#include "icons/bitmaps/RGB300.h"

class RGBIcon : public IconHandler {
    public:
        RGB0 rgb0;
        RGB60 rgb60;
        RGB120 rgb120;
        RGB180 rgb180;
        RGB240 rgb240;
        RGB300 rgb300;

        Rate rate = Rate(4);

        RGBIcon() {
            addIcon(&rgb0);
            addIcon(&rgb60);
            addIcon(&rgb120);
            addIcon(&rgb180);
            addIcon(&rgb240);
            addIcon(&rgb300);
            showIcon(&rgb0);
        };

        void startCycle();
        void stopCycle();
        void update();
    
    private:
        bool cycling = false;
        int currentIcon;
};

#endif
