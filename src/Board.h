#ifndef BOARD_H
#define BOARD_H

#include "util/rate.h"
#include "util/Log.h"
#include "hardware/hardware.h"
#include "hardware/button.h"
#include "hardware/wifi/MACLinkListener.h"

#define PIN_BUTTON  27

class Board : public Hardware {
    public:
        Button button = Button(PIN_BUTTON);
        

    private:
};

#endif