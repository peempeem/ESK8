#ifndef ICON_H
#define ICON_H

#include "../../drawable.h"

class Icon {
    public:
        Icon() {}
        Icon(int width, int height);

        int width();
        int height();
        virtual const uint16_t* getBitmap() { return NULL; }
    protected:
        int _width, _height;
};

#endif
