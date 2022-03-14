#include "drawable.h"

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
}
