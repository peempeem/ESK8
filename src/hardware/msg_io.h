#ifndef MSG_IO_H
#define MSG_IO_H

#include "../util/Log.h"
#include <Arduino.h>
#include <list>
#include <queue>
#include <string>
#include <esp_wifi.h>
#include <esp_now.h>

#define MAC_SIZE            6
#define ESP_NOW_MAX_SIZE    250


#define RESEND_TIMEOUT      500


typedef unsigned int uint;

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

struct buf_t {
    uint8_t* data;
    int len;
};

#define SEND_MSG_FIRST_HEADER   8
#define SEND_MSG_NEXT_HEADER    4

typedef struct SEND_MSG {
    struct HEADER {
        uint16_t id;
        uint8_t pkt;
        uint8_t len;
    } header;
    union DATA {
        struct FIRST {
            struct HEADER {
                uint16_t type;
                uint16_t size;
            } header;
            uint8_t data[ESP_NOW_MAX_SIZE - SEND_MSG_FIRST_HEADER];
        } first;
        struct NEXT {
            uint8_t data[ESP_NOW_MAX_SIZE - SEND_MSG_NEXT_HEADER];
        } next;
    } data;
} send_msg_t;

typedef struct RECV_MSG {
    MAC mac;
    int type;
    int len;
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
        int id;
        int current_pkt;
        int type;
        int current_size;
        int max_size;
        uint8_t* buffer;
        int last;
        bool built = false;
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
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
        std::list<RecvMsgBuilderList> inbound;
};

class Whitelist {
    public:
        Whitelist();

        void add(MAC& mac);
        void remove(MAC& mac);
        bool contains(MAC& mac);
    
    private:
        std::list<MAC> whitelist;
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
    
    private:
        int last;
        bool ready = true;
        std::list<SendMessage> msgs;
};

class OutboundMessages {
    public:
        OutboundMessages();

        void send(msg_t& msg);
        void update();
        void approve(MAC& mac, bool approve);
        bool is_ready(MAC& mac);
        
    private:
        uint16_t id = 0;
        std::list<MACSendMessages> outbound;
};

#endif