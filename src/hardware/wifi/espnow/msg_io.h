#ifndef MSG_IO_H
#define MSG_IO_H

#include "mac.h"
#include <queue>
#include <esp_wifi.h>
#include <esp_now.h>


#define ESP_NOW_MAX_SIZE    250
#define RESEND_TIMEOUT      500


struct buf_t {
    uint8_t* data;
    int len;
};

typedef struct SEND_MSG {
    struct HEADER {
        uint16_t id;
        uint8_t pkt;
        uint8_t len;
        uint16_t type;
        uint16_t size;
    } header;
    uint8_t data[ESP_NOW_MAX_SIZE - sizeof(SEND_MSG::HEADER)];
} send_msg_t;

typedef struct RECV_MSG {
    MAC mac;
    uint16_t type;
    uint16_t len;
    uint8_t* data;
} recv_msg_t;

typedef struct MSG {
    MAC mac;
    uint16_t type;
    uint8_t* data;
    uint16_t len;
    int priority;
    int retries;
} msg_t;

class RecvMsgBuilder {
    public:
        int timeout = 3000;

        RecvMsgBuilder(send_msg_t* msg);

        bool add(send_msg_t* msg);
        bool get(recv_msg_t* msg);
        bool is_expired();
        void clear();
        bool is_built();

    private:
        uint16_t id;
        uint8_t current_pkt;
        uint16_t type;
        uint16_t current_size;
        uint16_t max_size = -1;
        uint8_t* buffer;
        bool zero_size = false;
        int last;
        bool built = false;
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
};

class RecvMsgBuilderList {
    public:
        MAC mac;
        int timeout = 5000;

        RecvMsgBuilderList(MAC& mac);

        void add(send_msg_t* msg);
        bool get(recv_msg_t* msg);
        void preen();
        bool expired();

    private:
        std::list<RecvMsgBuilder> builders;
        int last;
};

class InboundMessages {
    public:
        InboundMessages();

        void add(MAC& mac, send_msg_t* msg);
        void preen();
        bool get(recv_msg_t* msg);

    private:
        std::queue<recv_msg_t> msgs;
        std::list<RecvMsgBuilderList> inbound;
};

class SendMessage {
    public:
        SendMessage(int id, msg_t& msg);

        void clear();
        int get_priority();
        bool get_msg(buf_t* msg);
        bool next();
        bool is_stale();
        bool is_empty();

    private:
        int priority;
        int retries;
        int max_retries;
        std::queue<buf_t> parts;
};

class MACSendMessages {
    public:
        MAC mac;

        MACSendMessages();

        void create_msg(int id, msg_t& msg);
        bool get_msg(buf_t* msg);
        void preen();
        void approve(bool approve);
        bool is_ready();
        void set_ready(bool ready);
        int current_msgs_count();
    
    private:
        int last;
        bool ready = true;
        std::list<SendMessage> msgs;
};

class OutboundMessages {
    public:
        int max_mac_msgs = 20;

        OutboundMessages();

        bool send(msg_t& msg);
        void update();
        void approve(MAC& mac, bool approve);
        bool is_ready(MAC& mac);
        
    private:
        uint16_t id = 0;
        std::list<MACSendMessages> outbound;

        bool _get(MAC& mac, std::list<MACSendMessages>::iterator& it);
};

#endif