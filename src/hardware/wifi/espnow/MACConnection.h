#ifndef MACCONNECTION_H
#define MACCONNECTION_H

#include "MessageStreamer.h"

#define RSSI_UNCONNECTED    -100

class MACConnection {
    public:
        int timeout = 5000;

        MACConnection(MAC& mac);

        bool is(MAC& mac);
        void set_rssi(int rssi);
        int get_rssi();

        void add_streamer(StreamerBase* streamer);
        bool get_msg(msg_t& msg);
        void dispose_msg(msg_t& msg);

        void approve(bool approve);


    private:
        MAC mac;
        int rssi;
        int last;

        std::queue<msg_t> recv_msgs;
        std::list<StreamerBase*> streamers;
};

#endif