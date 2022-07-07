#ifndef MSG_UTIL_H
#define MSG_UTIL_H

#include "msg.h"
#include "../../../util/rate.h"
#include <queue>

class SendMessage {
    public:
        SendMessage();

        void make(int id, int retries, msg_t& msg);
        void clear();
        bool get_msg(buf_t* msg);
        void next();
        bool is_made();

    private:
        int retries = 0;
        int max_retries;
        bool made = false;
        std::queue<buf_t> parts;
};

#endif