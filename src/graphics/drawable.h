#ifndef SCREENS_H
#define SCREENS_H

#include "../util/rate.h"
#include "color.h"
#include <TFT_eSPI.h>

struct Point { int x, y; };
struct FPoint { float x, y; };
struct Dimension { int width, height; };

struct TPoint { FPoint p, v; };

struct Connection {
    int p1, p2;
    
    bool operator == (const Connection& other) const {
        return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
    }
};


class Triangles {
    public:
        Dimension dimensions;
        Point point;
        Color darkColor = DARK_GRAY;
        Color lightColor = TURQUOISE;
        Rate rate = Rate(0.3f);

        Triangles(int triangles);

        void draw(TFT_eSprite* sprite);
    public:
        long last_time;
        std::vector<TPoint> points;
        
};

#define VISABLE     0
#define INVISABLE   1

class Screen {
    public:
        Dimension dimensions;
        Point point;
        Color backgroundColor = BLACK;
        Color cornerColor = GRAY;
        Color dotsDefaultColor = LIGHT_GRAY;
        Color dotsBlendColor = ALMOST_BLACK;
        float cornerSize = 0.1f;
        int dots = 15;
        Rate dotsRate = Rate(0.4f);
        Triangles triangles = Triangles(10);

        virtual void setVisability(int visability) {}
        virtual void draw(TFT_eSprite* sprite) {}
        void drawBackground(TFT_eSprite* sprite, bool draw_triangles=true);
};

class Indicator {
    public:
        Dimension dimensions;
        FPoint center;

        virtual void draw(TFT_eSprite* sprite);

    protected:
        virtual float width();
        virtual float height();
};

#endif
