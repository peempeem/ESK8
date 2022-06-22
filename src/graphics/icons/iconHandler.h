#ifndef ICONHANDLER_H
#define ICONHANDLER_H

#include "icon.h"
#include "../drawable.h"

class IconHandler {
    public:
        Dimension   dimensions;                 // display dimensions
        Point       point;                      // display placement
        float       maxBlinkBias = 0.9f;        // max icon fade during blink
        Rate        blinkRate  = Rate(1.0f);    // blinking speed
        Rate        cycleRate  = Rate(1.0f);    // cycling speed
        bool        linearCycle = true;

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
        void    allocate();
        void    deallocate();
        void    draw(TFT_eSprite* sprite, int priority=2);

    protected:
        std::vector<Icon*>  _icons;             // list of all possible icons to display
        std::vector<Icon*>  _cycleIcons;        // list of cycle icons
    
    private:
        int     _currentIcon = -1;  // index of icon being displayed
        bool    cycling = false;    // cycling flag
        bool    blinking = false;   // blinking flag
};

#endif