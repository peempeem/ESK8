#ifndef CONNECTION_H
#define CONNECTION_H

/*#include "util/pvar.h"
#include "util/rate.h"
#include "espnow.h"

class MessageLink {
    public:
        MessageLink();

        bool init(const char* name, const char* password);

        bool is_paired();
        bool is_pairing();

        void unpair(bool notify=true);
        bool start_pairing(const char* key, int timeout=10000);
        void enable_pairing(int timeout=10000);

        int send(uint8_t* data, int len);

        void update();

        bool is_connected();

    private:
        struct WiFiVar {
            bool paired = false;
            uint8_t mac[6];
        };

        PVar<WiFiVar> wifiPVar = PVar<WiFiVar>("/wifi.var");

        Timer pairing_timer;
        Timer heartbeat;
        Timer heartbeat_timeout;

        String pairing_key;

        bool _send(typed_msg_t* msg);
};*/

#endif