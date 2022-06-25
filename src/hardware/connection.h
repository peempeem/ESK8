#ifndef CONNECTION_H
#define CONNECTION_H

#include "util/pvar.h"
#include "util/rate.h"
#include "espnow.h"
#include <cstdint>

struct WiFiVar {
    bool paired = false;
    uint8_t this_mac[6];
    uint8_t peer_mac[6];
};

class MessageLink {
    public:
        MessageLink();

        bool init(bool sta);

        bool is_paired();
        bool is_pairing();
        void pair_master(int timeout=5000);
        void pair_slave(int timeout=5000);

        int send(uint8_t* data, int len);

        void update();

        bool is_connected();

    private:
        bool sta;
        bool pairing = false;
        PVar<WiFiVar> wifiPVar = PVar<WiFiVar>("/wifi.var");

        Timer heartbeat;
        Timer heartbeat_timeout;

        typedef struct TYPED_MSG {
            uint8_t type;
            uint8_t len;
            uint8_t data[248];
        } typed_msg_t;

        wifi_interface_t _ifidx();
        bool _send(typed_msg_t* msg);
};

#endif