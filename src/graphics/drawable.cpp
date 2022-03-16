#include "drawable.h"

Triangles::Triangles(int triangles) {
    for (int i = 0; i < triangles; i++) {
        TPoint tpoint {
            .p = {
                .x = random(0, 1001) / 1000.0f,
                .y = random(0, 1001) / 1000.0f
            },
            .v = {
                .x = random(1, 101) / 1000.0f,
                .y = random(1, 101) / 1000.0f
            }
        };
        if (random(0, 2))
            tpoint.v.x = -tpoint.v.x;
        if (random(0, 2))
            tpoint.v.y = -tpoint.v.y;
        points.push_back(tpoint);
    }
}


static void zone(TPoint& tpoint) {
    if (tpoint.p.x < 0) {
        tpoint.p.x = -tpoint.p.x;
        tpoint.v.x = -tpoint.v.x;
    } else if (tpoint.p.x > 1) {
        tpoint.p.x = 2 - tpoint.p.x;
        tpoint.v.x = -tpoint.v.x;
    }
    if (tpoint.p.y < 0) {
        tpoint.p.y = -tpoint.p.y;
        tpoint.v.y = -tpoint.v.y;
    } else if (tpoint.p.y > 1) {
        tpoint.p.y = 2 - tpoint.p.y;
        tpoint.v.y = -tpoint.v.y;
    }
}

static float distance(TPoint& tp1, TPoint& tp2) {
    return sqrt(pow(tp2.p.x - tp1.p.x, 2) + pow(tp2.p.y - tp1.p.y, 2));
}

static float closestPoint(TPoint& tp, std::vector<TPoint>& points) {
    int idx = -1;
    float min = -1;
    for (int i = 0; i < points.size(); i++) {
        float d = distance(tp, points[i]);
        if (min > d && d != 0 || idx == -1) {
            idx = i;
            min = d;
        }
    }
    return idx;
}

static bool alreadyConnected(Connection connection, std::vector<Connection>& connections) {
    for (int i = 0; i < connections.size(); i++) {
        if (connection == connections[i])
            return true;
    }
    return false;
}

void Triangles::draw(TFT_eSprite* sprite) {
    long time = millis();
    float dt = time - last_time;
    if (dt >= 1000)
        dt = 0;
    else
        dt /= 1000.0f;
    last_time = time;
    
    for (int i = 0; i < points.size(); i++) {
        points[i].p.x += points[i].v.x * dt;
        points[i].p.y += points[i].v.y * dt;
        zone(points[i]);
    }

    Color c = DARK_GRAY;
    std::vector<Connection> connections;
    Connection connection;
    
    for (int i = 0; i < points.size(); i++) {
        std::vector<TPoint> points_copy(points);
        connection.p1 = i;
        for (int a = 0; a < 3; a++) {
            connection.p2 = closestPoint(points[i], points_copy);
            if (connection.p2 != -1) {
                connections.push_back(connection);
                points_copy.erase(points_copy.begin() + connection.p2);
            } else
                break;
        }
    }

    float stage = (1 - rate.getStageCos()) / 2.0f;

    for (int i = 0; i < connections.size(); i++) {
        TPoint tp1 = points[connections[i].p1];
        TPoint tp2 = points[connections[i].p2];

        float d1 = 1 - fabs(stage - tp1.p.y);
        float d2 = 1 - fabs(stage - tp1.p.y);
        float bias = (d1 + d2) / 2.0f;
        bias = pow(bias, 5);

        sprite->drawLine(
            (int) (tp1.p.x * dimensions.width) + point.x,
            (int) (tp1.p.y * dimensions.height) + point.y,
            (int) (tp2.p.x * dimensions.width) + point.x,
            (int) (tp2.p.y * dimensions.height) + point.y,
            darkColor.blend(lightColor, bias).as16Bit()
        );
    }
}

void Screen::drawBackground(TFT_eSprite* sprite) {
    sprite->fillRect(0, 0, dimensions.width, dimensions.height, backgroundColor.as16Bit());

    int cSize = (int) (cornerSize * dimensions.width);
    sprite->drawFastVLine(
        0,
        0,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastHLine(
        0,
        0,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastVLine(
        dimensions.width - 1,
        0,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastHLine(
        dimensions.width - cSize,
        0,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastVLine(
        0,
        dimensions.height - cSize,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastHLine(
        0,
        dimensions.height - 1,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastVLine(
        dimensions.width - 1,
        dimensions.height - cSize,
        cSize,
        cornerColor.as16Bit()
    );
    sprite->drawFastHLine(
        dimensions.width - cSize,
        dimensions.height - 1,
        cSize,
        cornerColor.as16Bit()
    );

    float xScale = dimensions.width / (float) dots;
    float yScale = dimensions.height / (float) dots;
    float stage = dotsRate.getStageSin() * 0.5f + 0.5f;
    for (int y = 0; y < dots; y++) {
        float yy = ((y + 0.5f) * yScale);
        float blend = 1 - fabs(stage - yy / (float) dimensions.height);
        Color tempColor = dotsDefaultColor.blend(dotsBlendColor, blend);
        for (int x = 0; x < dots; x++) {
            float xx = ((x + 0.5f) * xScale);
            blend = 1 - fabs(stage - xx / (float) dimensions.width);
            sprite->drawPixel(
                (int) xx,
                (int) yy,
                tempColor.blend(dotsBlendColor, blend).as16Bit()
            );
        }
    }

    triangles.dimensions = dimensions;
    triangles.draw(sprite);
}
