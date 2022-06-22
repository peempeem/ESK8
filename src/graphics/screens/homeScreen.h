#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../drawable.h"
#include "../indicators/boardBatteryIndicator.h"
#include "../indicators/powerIndicator.h"

class HomeScreen : public Screen {
    public:
        BoardBatteryIndicator boardBattery;
        PowerIndicator power;
        Color speedColor = WHITE;
        Color speedDescColor = GRAY;

        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);

        void setSpeed(float speed);
    
    private:
        String speed = "0.0";
        String speedDesc = "mph";
};

#endif
