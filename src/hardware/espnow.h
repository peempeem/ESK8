#ifndef ESPNOW_H
#define ESPNOW_H

#include "msg_io.h"
#include <WiFi.h>


static Whitelist whitelist;

bool espnow_init();

bool add_peer(esp_now_peer_info_t* peer_info);

void send_msg(msg_t& msg);

bool get_inbound_message(recv_msg_t* msg);

void dispose_msg(recv_msg_t& msg);

#endif