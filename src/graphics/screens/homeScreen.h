#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../drawable.h"
#include "../indicators/boardBatteryIndicator.h"
#include "../indicators/powerIndicator.h"

class HomeScreen : public Screen {
    public:
        BoardBatteryIndicator boardBattery;
        PowerIndicator power;
        Color textColor = WHITE;

        void draw(TFT_eSprite* sprite);

        void setSpeed(float speed);
    
    private:
        String speed = "22.4";
};

#endif
