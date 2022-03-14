#ifndef GUI_H
#define GUI_H

#include "transitions.h"
#include "notificationBar.h"

class GUI {
    public:
        Rate refreshRate = Rate(60);
        NotificationBar notificationBar;

        GUI(int rotation=2, int colorDepth=16);

        bool containsScreen(Screen* screen);
        void addScreen(Screen* screen);
        void setMainScreen(Screen* screen);
        bool isMainScreen(Screen* screen);
        void transitionTo(Screen* screen, int transitionType, int time=250);
        void update();

    private:
        TFT_eSPI* display;
        TFT_eSprite* sprite = NULL;
        TFT_eSprite* _notificationSprite = NULL;
        Screen* mainScreen = NULL;
        Screen* transitionScreen = NULL;
        std::vector<Screen*> screens;
        Dimension _screenDimensions;
        ScreenTransition transitions;
};

#endif
