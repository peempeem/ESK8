#include "connection.h"
#include "espnow.h"

const static char* LOG_HEADER = "MessageLink";
const static uint8_t pairing_mac[6] = {0x42, 0x65, 0x61, 0x6e, 0x73, 0x21};   // Beans!

const static uint8_t c_mac[6] = {0x94, 0xb9, 0x7e, 0xd2, 0x28, 0x84};
const static uint8_t b_mac[6] = {0x08, 0x3a, 0xf2, 0x69, 0xd7, 0x58};

#define MSG_HEARTBEAT       0
#define MSG_PAIRING_REQ     1
#define MSG_PAIRING_ACK     2
#define MSG_PAIRING_NACK    3
#define MSG_USER            8

#define HEARTBEAT_MS    1000

MessageLink::MessageLink() { }

bool MessageLink::init(bool sta) {
    this->sta = sta;
    if (!wifiPVar.load() || !wifiPVar.data.paired)
        log(WARN, LOG_HEADER, "No recent connection found");

    return espnow_init(sta);
}

bool MessageLink::is_paired() {
    return wifiPVar.data.paired;
}

bool MessageLink::is_pairing() {
    return pairing_timer.is_set() && !pairing_timer.is_ringing();
}

void delete_peers() {
    esp_now_peer_info_t peer;
    
    while (esp_now_fetch_peer(true, &peer) == ESP_OK)
        esp_now_del_peer(peer.peer_addr);
}

void randomize_mac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = random(0, 256);
    }
    mac[0] &= 0xFE;
}

wifi_interface_t MessageLink::_ifidx() {
    if (sta)
        return WIFI_IF_STA;
    else
        return WIFI_IF_AP;
}

void MessageLink::pair_master(int timeout) {
    log(DEBUG, LOG_HEADER, "Pairing as master");
    pairing_timer.set(timeout);
    
    delete_peers();
    wifiPVar.data.paired = false;
    randomize_mac(wifiPVar.data.this_mac);
    memcpy(wifiPVar.data.peer_mac, pairing_mac, 6);

    esp_err_t err = esp_wifi_set_mac(_ifidx(), wifiPVar.data.this_mac);

    if (err != ESP_OK) {
        log(ERROR, LOG_HEADER, esp_err_to_name(err));
        pairing_timer.ring();
        return;
    }

    wifiPVar.write();

    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(peer));
    memcpy(peer.peer_addr, pairing_mac, 6);
    peer.channel = 1;
    peer.encrypt = false;
    //peer.ifidx = _ifidx();
    if (!add_peer(&peer))
        log(ERROR, LOG_HEADER, "Failed to add pairing peer");
    else {
        typed_msg_t msg = {
            .type = MSG_PAIRING_REQ,
            .len = 0
        };
        _send(&msg);
    }

    Serial.println(WiFi.macAddress());
}

void MessageLink::pair_slave(int timeout) {
    log(DEBUG, LOG_HEADER, "Pairing as slave");
    pairing_timer.set(timeout);

    delete_peers();
    wifiPVar.data.paired = false;
    memcpy(wifiPVar.data.this_mac, pairing_mac, 6);
    
    if (esp_wifi_set_mac(_ifidx(), wifiPVar.data.this_mac) != ESP_OK)
        pairing_timer.ring();
    else
        wifiPVar.write();
    
    Serial.println(WiFi.macAddress());
}

int MessageLink::send(uint8_t* data, int len) {
    typed_msg_t msg;
    msg.type = MSG_USER;
    msg.len = min(248, len);
    memcpy(msg.data, data, msg.len);
    if (esp_now_send(wifiPVar.data.peer_mac, (uint8_t*) &msg, msg.len + 2) == ESP_OK)
        return msg.len;
    return 0;
}

bool MessageLink::_send(typed_msg_t* msg) {
    esp_err_t err = esp_now_send(wifiPVar.data.peer_mac, (uint8_t*) &msg, msg->len + 2);
    if (err != ESP_OK)
        log(ERROR, LOG_HEADER, esp_err_to_name(err));
        return false;
    return true;
}

void MessageLink::update() {
    if (outbound_messages()) {
        sent_msg_t message = get_outbound_message();
    }
    if (inbound_messages()) {
        recv_msg_t message = get_inbound_message();
        typed_msg_t* msg = (typed_msg_t*) message.data;

        switch (msg->type) {
            case MSG_HEARTBEAT: {
                log(DEBUG, LOG_HEADER, "Received heartbeat message");
                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_REQ: {
                typed_msg_t rsp = {
                    .type = MSG_PAIRING_NACK,
                    .len = 0
                };
                if (is_pairing()) {
                    randomize_mac(wifiPVar.data.this_mac);
                    if (esp_wifi_set_mac(_ifidx(), wifiPVar.data.this_mac) != ESP_OK)
                        log(ERROR, LOG_HEADER, "Could not change mac 2");
                    else {
                        memcpy(wifiPVar.data.peer_mac, message.mac, 6);
                        wifiPVar.data.paired = true;
                        wifiPVar.write();

                        rsp.type = MSG_PAIRING_ACK;
                        

                        esp_now_peer_info_t peer;
                        memset(&peer, 0, sizeof(peer));
                        memcpy(peer.peer_addr, wifiPVar.data.peer_mac, 6);
                        peer.channel = 1;
                        peer.encrypt = false;
                        //peer.ifidx = _ifidx();
                        add_peer(&peer);
                    }
                }
                _send(&rsp);
                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_ACK: {
                if (is_pairing()) {
                    memcpy(wifiPVar.data.peer_mac, message.mac, 6);
                    wifiPVar.data.paired = true;
                    wifiPVar.write();
                }
                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_NACK: {
                dispose_msg(&message);
                break;
            }
            
            case MSG_USER: {
                dispose_msg(&message);
                break;
            }
            
            default: {
                dispose_msg(&message);
                break;
            }
        }
    }

    if (is_paired() && (!heartbeat.is_set() || heartbeat.is_ringing())) {
        heartbeat.set(HEARTBEAT_MS);
        typed_msg_t msg = {
            .type = MSG_HEARTBEAT,
            .len = 0
        };
        _send(&msg);
        //log(DEBUG, LOG_HEADER, "Sent heartbeat message");
    }
}


bool MessageLink::is_connected() {
    return false;
}