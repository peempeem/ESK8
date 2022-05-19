#ifndef ICONHANDLER_H
#define ICONHANDLER_H

#include "icon.h"

class IconHandler {
    public:
        Dimension   dimensions;                 // display dimensions
        Point       point;                      // display placement
        Color       backgroundColor = BLACK;    // background color for blinking
        float       maxBlinkBias = 0.6f;        // max icon fade during blink
        Rate        blinkRate  = Rate(1.0f);    // blinking speed
        Rate        cycleRate  = Rate(1.0f);    // cycling speed

        IconHandler() {}

        void    addIcon(Icon* icon);
        void    setIcons(std::vector<Icon*> icons);
        void    showIcon(Icon* icon);
        Icon*   currentIcon();
        void    setCycleIcons(std::vector<Icon*> cycleIcons);
        void    startCycling();
        void    stopCycling();
        bool    isCycling();
        void    startBlinking();
        void    stopBlinking();
        bool    isBlinking();
        void    draw(TFT_eSprite* sprite);

    protected:
        std::vector<Icon*>  icons;              // list of all possible icons to display
        std::vector<Icon*>  cycleIcons;         // list of cycle icons
    
    private:
        int                 currentIcon = -1;   // index of icon being displayed
        bool                cycling = false;    // cycling flag
        bool                blinking = false;   // blinking flag
};

#endif
