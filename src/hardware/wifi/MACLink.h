#ifndef MACLINK_H
#define MACLINK_H

#include "msg_io.h"
#include "util/rate.h"

#define DC_RSSI -100

enum ReservedMsgs {
    Heartbeat,

    SIZE = Heartbeat + 1
};

class MACLink : public Lock {
    public:
        MACLink();
        MACLink(MAC& mac, int dc_timeout_ms=2000, int msg_timeout=250);

        bool is(MAC& mac);
        MAC get_mac();
        bool is_connected();
        int get_rssi();
        bool send(uint16_t type, uint8_t* data, uint16_t len, int retries);
        bool get(recv_msg_t& msg);
        void dispose(recv_msg_t& msg);
        void update();
        int get_num_msgs();

        void _recv_cb(const uint8_t* data, int len);
        void _send_cb(esp_now_send_status_t status);

    private:
        MAC mac;
        int rssi = DC_RSSI;

        RecvMessages recv_msgs;
        SendMessages send_msgs;

        Timer heartbeat;
        Timer dc_timer;
        Timer msg_timeout;

        void _send(uint16_t type, uint8_t* data, uint16_t len, int retries);
};

#endif