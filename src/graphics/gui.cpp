#include "gui.h"

GUI::GUI(int rotation, int colorDepth) {
    display = new TFT_eSPI();
    display->init();
    display->setRotation(rotation);
    display->fillScreen(0);

    bool isEven = rotation % 2 == 0;
    _screenDimensions.width = isEven ? TFT_WIDTH : TFT_HEIGHT;
    _screenDimensions.height = isEven ? TFT_HEIGHT : TFT_WIDTH;

    notificationBar.dimensions.width = _screenDimensions.width;
    notificationBar.dimensions.height = 32;
    _screenDimensions.height -= notificationBar.dimensions.height;

    sprite = new TFT_eSprite(display);
    sprite->setColorDepth(colorDepth);
    sprite->createSprite(_screenDimensions.width, _screenDimensions.height);

    _notificationSprite = new TFT_eSprite(display);
    _notificationSprite->setColorDepth(colorDepth);
    _notificationSprite->createSprite(notificationBar.dimensions.width, notificationBar.dimensions.height);
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
        screen->dimensions = _screenDimensions;
        screen->point = Point{0, notificationBar.dimensions.height};
        screens.push_back(screen);
    }
}

void GUI::setMainScreen(Screen* screen) {
    if (containsScreen(screen))
        mainScreen = screen;
}

bool GUI::isMainScreen(Screen* screen) { return mainScreen == screen; }

void GUI::transitionTo(Screen* screen, int transition, int time) {
    if (containsScreen(screen) && screen != mainScreen && !transitions.isTransitioning()) {
        transitions.beginTransition(
            mainScreen,
            screen,
            transition,
            time
        );
        transitionScreen = screen;
    }
}

void GUI::update() {
    if (refreshRate.isReady()) {
        if (transitions.isTransitioning()) {
            if (transitions.draw(sprite))
                mainScreen = transitionScreen;
        }
        else {
            if (mainScreen != NULL)
                mainScreen->draw(sprite);
            sprite->pushSprite(mainScreen->point.x, mainScreen->point.y);
        }
        notificationBar.draw(_notificationSprite);
        _notificationSprite->pushSprite(0, 0);
    }
}
