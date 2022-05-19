#ifndef RGBICON_H
#define RGBICON_H

#include "icons/iconHandler.h"
#include "icons/bitmaps/RGB0.h"
#include "icons/bitmaps/RGB30.h"
#include "icons/bitmaps/RGB60.h"
#include "icons/bitmaps/RGB90.h"
#include "icons/bitmaps/RGB120.h"
#include "icons/bitmaps/RGB150.h"
#include "icons/bitmaps/RGB180.h"
#include "icons/bitmaps/RGB210.h"
#include "icons/bitmaps/RGB240.h"
#include "icons/bitmaps/RGB270.h"
#include "icons/bitmaps/RGB300.h"
#include "icons/bitmaps/RGB330.h"

class RGBIcon : public IconHandler {
    public:
        RGB0    rgb0;
        RGB30   rgb30;
        RGB60   rgb60;
        RGB90   rgb90;
        RGB120  rgb120;
        RGB150  rgb150;
        RGB180  rgb180;
        RGB210  rgb210;
        RGB240  rgb240;
        RGB270  rgb270;
        RGB300  rgb300;
        RGB330  rgb330;

        Rate rate = Rate(0.1f);

        RGBIcon() {
            addIcon(&rgb0);
            addIcon(&rgb30);
            addIcon(&rgb60);
            addIcon(&rgb90);
            addIcon(&rgb120);
            addIcon(&rgb150);
            addIcon(&rgb180);
            addIcon(&rgb210);
            addIcon(&rgb240);
            addIcon(&rgb270);
            addIcon(&rgb300);
            addIcon(&rgb330);
            showIcon(&rgb0);
        };

        void startCycle();
        void stopCycle();
        void update();
    
    private:
        bool cycling = false;
        int nextIcon;
};

#endif
