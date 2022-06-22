#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "../drawable.h"

#define TRANSITION_NONE         0
#define TRANSITION_SLIDE_RIGHT  1
#define TRANSITION_SLIDE_LEFT   2
#define TRANSITION_SLIDE_UP     3
#define TRANSITION_SLIDE_DOWN   4

class ScreenTransition {
    public:
        ScreenTransition() { }

        void beginTransition(Screen* screen1, Screen* screen2, int transition, int time);
        bool isTransitioning();
        bool draw(TFT_eSprite* sprite);

    private:
        Screen* _screen1 = NULL;
        Screen* _screen2 = NULL;
        int _transition;
        long start;
        int _time;
        bool transitioning = false;
};

#endif
