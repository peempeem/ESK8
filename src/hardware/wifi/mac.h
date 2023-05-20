#ifndef MAC_H
#define MAC_H

#include "util/Log.h"
#include <stdint.h>
#include <list>
#include <Arduino.h>

#define MAC_SIZE 6

class MAC {
    public:
        MAC();
        MAC(const uint8_t* mac);

        void set(const uint8_t* mac);
        void set(MAC& mac);
        uint8_t* get();
        bool is(const uint8_t* mac);
        bool is(MAC& mac);
        bool is(MAC* mac);
        void print();

    private:
        uint8_t mac[MAC_SIZE];
};

#endif