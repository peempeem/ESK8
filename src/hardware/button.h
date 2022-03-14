#ifndef BUTTON_H
#define BUTTON_H

#include <vector>

struct TapEvent {
    int taps;       // number of consecutive taps
    long start;     // time of first tap
    long end;       // time of last tap
};

// class for interfacing with asynchronus human input
class Button {
    public:
        int debounceTimeout = 5;    // timeout for button debouncing in milliseconds
        int multiTapTimeout = 300;  // timeout for tap detection in milliseconds

        Button(int pin, bool activeLow=true, bool recordTaps=true);

        void        init();
        void        update();
        bool        isPressed();
        bool        isReleased();
        bool        isTapped();
        bool        hasTapEvent();
        TapEvent    getTapEvent();
        long        pressedElapsedTime();
        long        releasedElapsedTime();

    private:
        int         pin;                    // button pin on board
        bool        activeLow;              // determines button press conditions
        bool        recordTaps;             // determines if multitap functionality is enabled
        bool        newTap = false;         // flag for getting new taps
        int         pressed;                // defining what pressed value is
        int         released;               // defining what released value is
        long        time = 0;               // time of last update
        int         lastState = 0;          // last button press state
        long        lastStateChange = 0;    // time since last button press state change
        long        lastTap = 0;            // time since last tap detected

        TapEvent    currentTapEvent{0, 0, 0};   // stores current tap event data
        std::vector<TapEvent>   tapEvents;      // vector for storing tap events
};

#endif
