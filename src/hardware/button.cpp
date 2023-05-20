#include "button.h"
#include <Arduino.h>

/*
 * DESCRIPTION: Button class constructor
 * INPUTS:      pin         - pin number on board
 *              activeLow   - set if button press is low
 *              recordTaps  - set if you want to buffer tap events
 */
Button::Button(int pin, bool activeLow, bool recordTaps) {
    this->pin = pin;
    this->activeLow = activeLow;
    this->recordTaps = recordTaps;

    // define pressed and released values
    if (activeLow) {
        pressed = 0;
        released = 1;
    } else {
        pressed = 1;
        released = 0;
    }
}

/*
 * DESCRIPTION: Initializes the button. This function should be called
 *              after board initialization.
 */
void Button::init() {
    pinMode(pin, INPUT_PULLUP);     // internal pullup resistor mostlikely needed
    lastState = digitalRead(pin);
    time = millis();
    lastStateChange = time;
    lastTap = time;
}

/*
 * DESCRIPTION: Updates all button functionality and should be called 
 *              on a regular basis
 */
void Button::update() {
    time = millis();
    // check for debounce conditions
    if (time - lastStateChange >= debounceTimeout) {
        int pinVal = digitalRead(pin);
        // no change in button state
        if (lastState == pinVal) {
            // end tap event if tap timed out
            if (recordTaps && currentTapEvent.taps != 0 && time - lastTap >= multiTapTimeout) {
                currentTapEvent.end = time;
                tapEvents.push_back(currentTapEvent);
                currentTapEvent = {0, 0, 0};
            }
        }
        // button state changed
        else {
            if (pinVal == pressed) {
                // update tap events if recording taps
                if (recordTaps) {
                    if (currentTapEvent.taps == 0)
                        currentTapEvent.start = time;
                    currentTapEvent.taps++;
                    lastTap = time;
                }
                newTap = true;
            }
            lastState = pinVal;
            lastStateChange = time;
        }
    }
}

/*
 * DESCRIPTION: Getter function for button presses
 * RETURNS:     true if button is pressed else false
 */
bool Button::isPressed() { return lastState == pressed; }

/*
 * DESCRIPTION: Getter function for button releases
 * RETURNS:     true if button is released else false
 */
bool Button::isReleased() { return lastState == released; }

/*
 * DESCRIPTION: Getter function for new button taps
 * RETURNS:     true if new button tap else false
 */
bool Button::isTapped() {
    if (newTap) {
        newTap = false;
        return true;
    }
    return false;
}

/*
 * DESCRIPTION: Getter function for availablilty of new tap events
 * RETURNS:     true if tap events have happened else false
 */
bool Button::hasTapEvent() { return !tapEvents.empty(); }

/*
 * DESCRIPTION: Getter function for new tap events
 * RETURNS:     tap event in the order it happened
 */
TapEvent Button::getTapEvent() {
    TapEvent tapEvent{0, 0, 0};
    // return first tap in list if list is not empty
    if (!tapEvents.empty()) {
        tapEvent = tapEvents[0];
        tapEvents.erase(tapEvents.begin());
    }
    return tapEvent;
}

/*
 * DESCRIPTION: Getter function for elapsed time (milliseconds) since last button press
 * RETURNS:     time since last button press
 */
long Button::pressedElapsedTime() {
    if (isReleased())
        return 0;
    return time - lastStateChange;
}

/*
 * DESCRIPTION: Getter function for elapsed time (milliseconds) since last button release
 * RETURNS:     time since last button release
 */
long Button::releasedElapsedTime() {
    if (isPressed())
        return 0;
    return time - lastStateChange;
}

void Button::clearTapEvents() {
    tapEvents.clear();
}