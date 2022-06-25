#include "welcomeScreen.h"

void WelcomeScreen::setVisability(int visability) {

}

void WelcomeScreen::draw(TFT_eSprite* sprite) {
    if (!fade_in.is_set())
        fade_in.set(1000);
    if (!circles.is_set())
        circles.set(250);
    
    Point c = {dimensions.width / 2, dimensions.height / 2};

    float dr = -360 / (float) spokes;
    float spin_angle = (int) (spokes * spin_rate.getStage()) * dr;
    float progress = fade_in.progress();

    for (int spoke = 0; spoke < spokes; spoke++) {
        float angle = dr * spoke + spin_angle + start_angle;
        float x_rot = cos(angle * PI / 180.0f);
        float y_rot = sin(angle * PI / 180.0f);
        Point start = {c.x + load_circle_min * x_rot, c.y + load_circle_min * y_rot};
        Point end = {c.x + load_circle_max * x_rot, c.y + load_circle_max * y_rot};

        Color color = accent_color.blend(main_color, (spoke + 1) / (float) spokes);
        color = color.blend(BLACK, 1 - progress);

        sprite->drawLine(start.x, start.y, end.x, end.y, color.as16Bit());
    }

    if (circles.is_ringing()) {
        int circles_start = load_circle_max + circle_space;
        float bias = (circle_radius - circles_start) / (float) (c.y - circles_start);
        sprite->drawCircle(c.x, c.y, circle_radius, main_color.blend(accent_color, bias).as16Bit());
        circle_radius += circle_space;
    }   
}