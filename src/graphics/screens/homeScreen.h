#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "../drawable.h"
#include "../indicators/boardBatteryIndicator.h"
#include "../indicators/powerIndicator.h"

class HomeScreen : public Screen {
    public:
        BoardBatteryIndicator boardBattery;
        PowerIndicator power;
        Color mainTextColor = WHITE;
        Color accentTextColor = GRAY;

        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);

        void setSpeed(float speed);
        void setSpeedMode(int mode);
        void setNumOutbound(unsigned num);
    
    private:
        String speed = "0.0";
        String speedDesc = "mph";
        String speedMode = "M: 0";
        String outbound = "- 0 -";
};

#endif
