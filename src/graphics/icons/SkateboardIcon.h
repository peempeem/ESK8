#ifndef SKATEBOARDICON_H
#define SKATEBOARDICON_H

#include "iconHandler.h"

class SkateboardIcon : public IconHandler {
    public:
        Icon skateboard = Icon("/icons/Skateboard.icon");

        SkateboardIcon() {
            addIcon(&skateboard);
            showIcon(&skateboard);
        };
};

#endif