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
    uint16_t type;
    uint16_t len;
    uint8_t* data;
} recv_msg_t;

typedef struct {
    uint16_t frame_head;
    uint16_t duration;
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint8_t broadcast_address[6];
    uint16_t sequence_control;

    uint8_t category_code;
    uint8_t organization_identifier[3]; // 0x18fe34
    uint8_t random_values[4];
    struct {
        uint8_t element_id;                 // 0xdd
        uint8_t length;                     //
        uint8_t organization_identifier[3]; // 0x18fe34
        uint8_t type;                       // 4
        uint8_t version;
        uint8_t body[0];
    } vendor_specific_content;
} __attribute__ ((packed)) espnow_frame_format_t;

class Lock {
    public:
        Lock();

        void lock();
        void unlock();

    private:
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
};

class RecvMsgBuilder : public Lock {
    public:
        int timeout = 3000;

        RecvMsgBuilder(send_msg_t* msg);

        bool add(send_msg_t* msg);
        bool get(recv_msg_t& msg);
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
};

class RecvMessages {
    public:
        int timeout = 5000;

        RecvMessages();

        void add(send_msg_t* msg);
        bool get(recv_msg_t& msg);
        void preen();
        bool expired();

    private:
        std::list<RecvMsgBuilder> builders;
        int last;
};

class SendMessage {
    public:
        SendMessage(uint16_t id, uint16_t type, uint8_t* data, uint16_t len, int retries);

        void clear();
        bool get_msg(buf_t* msg);
        bool next();
        void try_again();
        bool is_stale();
        bool is_empty();

    private:
        int retries;
        int max_retries;
        std::queue<buf_t> parts;
};

class SendMessages {
    public:
        SendMessages();

        void create_msg(uint16_t type, uint8_t* data, uint16_t len, int retries);
        bool get_msg(buf_t* msg);
        void preen();
        void approve(bool approve);
        bool is_ready();
        void set_ready(bool ready);
        int current_msgs_count();
    
    private:
        uint16_t id = 0;
        int last;
        bool ready = true;
        std::list<SendMessage> msgs;
};

#endif