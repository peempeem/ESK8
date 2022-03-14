#include "transitions.h"

void ScreenTransition::beginTransition(Screen* screen1, Screen* screen2, int transition, int time) {
    if (!transitioning) {
        _screen1 = screen1;
        _screen2 = screen2;
        _transition = transition;
        _time = time;
        start = millis();
        transitioning = true;
    }
}

bool ScreenTransition::isTransitioning() { return transitioning; }

bool ScreenTransition::draw(TFT_eSprite* sprite) {
    if (transitioning) {
        long time = millis();
        float progress = (time - start) / (float) _time;
        if (progress >= 1)
            progress = 1;
        progress = sin(progress * PI / 2.0f);

        if (_screen1 != NULL) {
            _screen1->draw(sprite);
            switch (_transition) {
                case TRANSITION_PUSH_RIGHT:
                    sprite->pushSprite(
                        (int) (-_screen1->dimensions.width * progress) + _screen1->point.x,
                         _screen1->point.y
                     );
                    break;
                case TRANSITION_PUSH_LEFT:
                    sprite->pushSprite(
                        (int) (_screen1->dimensions.width * progress) + _screen1->point.x,
                        _screen1->point.y
                    );
                    break;
                case TRANSITION_PUSH_UP:
                    sprite->pushSprite(
                        _screen1->point.x,
                        (int) (-_screen1->dimensions.height * progress) + _screen1->point.y
                    );
                    break;
                case TRANSITION_PUSH_DOWN:
                    sprite->pushSprite(
                        _screen1->point.x,
                        (int) (_screen1->dimensions.height * progress) + _screen1->point.y);
                    break;
            }
        }

        if (_screen2 != NULL) {
            _screen2->draw(sprite);
            switch (_transition) {
                case TRANSITION_PUSH_RIGHT:
                case TRANSITION_SLIDE_RIGHT:
                    sprite->pushSprite(
                        (int) (_screen2->dimensions.width * (1 - progress)) + _screen2->point.x,
                        _screen2->point.y
                    );
                    break;
                case TRANSITION_PUSH_LEFT:
                case TRANSITION_SLIDE_LEFT:
                    sprite->pushSprite(
                        (int) (_screen2->dimensions.width * (progress - 1)) + _screen2->point.x,
                        _screen2->point.y);
                    break;
                case TRANSITION_PUSH_UP:
                case TRANSITION_SLIDE_UP:
                    sprite->pushSprite(
                        _screen2->point.x,
                        (int) (_screen2->dimensions.height * (1 - progress)) + _screen2->point.y
                    );
                    break;
                case TRANSITION_PUSH_DOWN:
                case TRANSITION_SLIDE_DOWN:
                    sprite->pushSprite
                        (_screen2->point.x,
                        (int) (_screen2->dimensions.height * (progress - 1)) + _screen2->point.y
                    );
                    break;
                default:
                    sprite->pushSprite(_screen2->point.x, _screen2->point.y);
                    transitioning = false;
                    return true;
            }
        }
        if (time - start >= _time) {
            transitioning = false;
            return true;
        }
        return false;
    }
}
