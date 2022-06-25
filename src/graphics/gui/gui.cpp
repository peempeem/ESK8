#include "gui.h"

GUI::GUI(int rotation, int notificaitonHeight, int colorDepth) {
    display = new TFT_eSPI();
    display->init();
    display->setRotation(rotation);
    display->fillScreen(0);

    bool isEven = rotation % 2 == 0;
    dimensions.width = isEven ? TFT_WIDTH : TFT_HEIGHT;
    dimensions.height = isEven ? TFT_HEIGHT : TFT_WIDTH;

    notifications.dimensions.width = dimensions.width;
    notifications.dimensions.height = notificaitonHeight;
    dimensions.height -= notifications.dimensions.height;

    sprite = new TFT_eSprite(display);
    sprite->setColorDepth(colorDepth);
    sprite->createSprite(dimensions.width, dimensions.height);

    notificationSprite = new TFT_eSprite(display);
    notificationSprite->setColorDepth(colorDepth);
    notificationSprite->createSprite(notifications.dimensions.width, notifications.dimensions.height);
}

bool GUI::containsScreen(Screen* screen) {
    for (Screen* s : screens) {
        if (s == screen)
            return true;
    }
    return false;
}

void GUI::addScreen(Screen* screen) {
    if (!containsScreen(screen)) {
        screen->dimensions = dimensions;
        screen->point = Point{0, notifications.dimensions.height};
        screens.push_back(screen);
    }
}

void GUI::setMainScreen(Screen* screen) {
    if (containsScreen(screen)) {
        if (mainScreen != NULL)
            mainScreen->setVisability(INVISABLE);
        mainScreen = screen;
        mainScreen->setVisability(VISABLE);
        changes.push_back(screen);
    }
}

bool GUI::isMainScreen(Screen* screen) { return mainScreen == screen; }

void GUI::transitionTo(Screen* screen, int transition, int time) {
    if (containsScreen(screen) && screen != mainScreen && !transitions.isTransitioning()) {
        screen->setVisability(VISABLE);
        transitions.beginTransition(
            mainScreen,
            screen,
            transition,
            time
        );
        setMainScreen(screen);
    }
}

Screen* GUI::screenChanged() {
    int size = changes.size();
    if (size > 0) {
        Screen* change = changes[size - 1];
        changes.pop_back();
        return change;
    }
}

void GUI::update(bool sleep) {
    if (refreshRate.isReady()) {
        if (transitions.isTransitioning()) {
            transitions.draw(sprite);
        } else {
            if (mainScreen != NULL)
                mainScreen->draw(sprite);
            sprite->pushSprite(mainScreen->point.x, mainScreen->point.y);
        }
        notifications.draw(notificationSprite);
        notificationSprite->pushSprite(0, 0);
        if (sleep)
            refreshRate.sleep();
    }
}
