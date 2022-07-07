#ifndef MAC_H
#define MAC_H

#include "util/Log.h"
#include <stdint.h>
#include <list>
#include <Arduino.h>

#define MAC_SIZE            6

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

class MACConnection {
    public:
        int timeout = 5000;

        MACConnection(MAC& mac);

        bool is(MAC& mac);
        void set_rssi(int rssi);
        int get_rssi();

    private:
        MAC mac;
        int rssi;
        int last;
};

class Whitelist {
    public:
        Whitelist();

        void add(MAC& mac);
        void remove(MAC& mac);
        bool contains(MAC& mac);
        void set_rssi(MAC& mac, int rssi);
        int get_rssi(MAC& mac);
    
    private:
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
        std::list<MACConnection> connections;

        bool _get(MAC& mac, std::list<MACConnection>::iterator& it);
};

#endif