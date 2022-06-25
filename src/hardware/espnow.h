#ifndef ESPNOW_H
#define ESPNOW_H

#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

typedef struct SENT_MSG {
    uint8_t mac[6];
    esp_now_send_status_t status;
} sent_msg_t;

typedef struct RECV_MSG {
    uint8_t mac[6];
    uint8_t* data;
    int len;
} recv_msg_t;

bool espnow_init(bool sta, bool send_cb_enabled=true);

bool add_peer(esp_now_peer_info_t* peer_info);

bool outbound_messages();
bool inbound_messages();

sent_msg_t get_outbound_message();
recv_msg_t get_inbound_message();

void dispose_msg(recv_msg_t* msg);

#endif