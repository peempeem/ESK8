#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "../drawable.h"

class WelcomeScreen : public Screen {
    public:
        Rate spin_rate = Rate(1);
        float load_circle_min = 12.0f;
        float load_circle_max = 36.0f;

        Color main_color = SUNSET_ORANGE;
        Color accent_color = SUNSET_PURPLE;

        float start_angle = -90.0f;

        int spokes = 16;
        int circle_space = 13;


        void setVisability(int visability);
        void draw(TFT_eSprite* sprite);

    private:
        Timer fade_in, circles;

        int circle_radius = load_circle_max + circle_space;
};

#endif