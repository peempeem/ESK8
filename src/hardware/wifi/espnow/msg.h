#ifndef ESPNOW_MSG_H
#define ESPNOW_MSG_H

#include "mac.h"

#define ESP_NOW_MAX_SIZE    250

typedef struct BUF {
    uint8_t* data;
    int len;
} buf_t;

typedef struct MSG {
    int type;
    uint8_t* data;
    int len;
} msg_t;

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

#endif