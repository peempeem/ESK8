#include "connection.h"

/*const static char* LOG_HEADER = "MessageLink";
const static uint8_t pairing_mac[6] = {0x42, 0x65, 0x61, 0x6e, 0x73, 0x21};   // Beans!

const static uint8_t c_mac[6] = {0x94, 0xb9, 0x7e, 0xd2, 0x28, 0x84};
const static uint8_t b_mac[6] = {0x08, 0x3a, 0xf2, 0x69, 0xd7, 0x58};

#define MSG_HEARTBEAT       0
#define MSG_PAIRING_REQ     1
#define MSG_PAIRING_ACK     2
#define MSG_PAIRING_NACK    3
#define MSG_UNPAIR          4
#define MSG_USER            8

#define HEARTBEAT_MS        500
#define HEARTBEAT_TIMEOUT   1100

MessageLink::MessageLink() { }

void random_ascii(char* buf, int len) {
    for (int i = 0; i < len; i++)
        buf[i] = random(65, 91);
}

esp_now_peer_info_t make_peer(uint8_t* mac, int channel, int encrypt) {
    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(peer));
    memcpy(peer.peer_addr, mac, 6);
    peer.channel = 0;
    peer.encrypt = 0;
    return peer;
}

bool MessageLink::init(const char* name, const char* password) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();

    int len = strlen(name);
    char buf[len + 8];
    strncpy(buf, name, len);
    random_ascii(buf + len, 7);
    buf[len + 7] = 0;

    WiFi.softAP(buf, password, 0, 0);

    if (espnow_init()) {
        if (!wifiPVar.load() || !wifiPVar.data.paired)
            log(WARN, LOG_HEADER, "No recent connection found");
        else {
            esp_now_peer_info_t peer = make_peer(wifiPVar.data.mac, 0, 0);
            add_peer(&peer);
        }
        return true;
    } else {
        log(ERROR, LOG_HEADER, "Could not initialize esp now");
    }

    return false;
}

bool MessageLink::is_paired() {
    return wifiPVar.data.paired;
}

bool MessageLink::is_pairing() {
    return !pairing_timer.is_ringing();
}

void randomize_mac(uint8_t* mac) {
    for (int i = 0; i < 6; i++) {
        mac[i] = random(0, 256);
    }
    mac[0] &= 0xFE;
}

void delete_peers() {
    esp_now_peer_info_t peer;
    while (esp_now_fetch_peer(true, &peer) == ESP_OK)
        esp_now_del_peer(peer.peer_addr);
}

void MessageLink::unpair(bool notify) {
    if (notify && is_connected()) {
        typed_msg_t msg = {
            .type = MSG_UNPAIR,
            .len = 0
        };
        _send(&msg);
    }
    delete_peers();
    if (wifiPVar.data.paired) {
        wifiPVar.data.paired = false;
        wifiPVar.write();
    }
}

bool MessageLink::start_pairing(const char* key, int timeout) {
    if (is_paired() || is_pairing())
        return false;
    
    log(DEBUG, LOG_HEADER, "start pairing");
    pairing_timer.set(timeout);

    pairing_key = key;

    WiFi.scanNetworks(true);
    return true;
}

void MessageLink::enable_pairing(int timeout) {
    pairing_timer.set(timeout);
}

int MessageLink::send(uint8_t* data, int len) {

}

bool MessageLink::_send(msg_t* msg, uint priority, uint retries) {
    MAC mac(wifiPVar.data.mac);
    send_msg(mac, priority, retries, msg);
}

void MessageLink::update() {
    if (!is_paired() && pairing_timer.is_ringing()) {
        esp_now_peer_num_t num;
        esp_err_t err = esp_now_get_peer_num(&num);
        if (err == ESP_OK) {
            if (num.total_num > 0) {
                log(DEBUG, LOG_HEADER, "pairing timeout");
                unpair();
            }
        }
    }

    int networks = WiFi.scanComplete();
    if (networks >= 0) {
        if (!is_pairing())
            WiFi.scanDelete();
        else {
            int len = pairing_key.length();
            int net = -1;
            int rssi;
            for (int i = 0; i < networks; i++) {
                String ssid = WiFi.SSID(i);

                if (ssid.indexOf(pairing_key) == 0) {
                    int signal = WiFi.RSSI();
                    if (net == -1 || signal > rssi) {
                        net = i;
                        rssi = signal;
                    }
                }
            }
            if (net == -1) {
                WiFi.scanDelete();
                pairing_timer.ring();
            } else {
                esp_now_peer_info_t peer = make_peer(WiFi.BSSID(net), 0, 0);
                WiFi.scanDelete();
                memcpy(wifiPVar.data.mac, peer.peer_addr, 6);
                add_peer(&peer);
                

                typed_msg_t msg {
                    .type = MSG_PAIRING_REQ,
                    .len = 0
                };
                _send(&msg);
            }
        }
    }

    while (outbound_messages()) {
        sent_msg_t message = get_outbound_message();
    }

    while (inbound_messages()) {
        recv_msg_t message = get_inbound_message();
        typed_msg_t* msg = (typed_msg_t*) message.data;
        
        bool paired_mac = is_paired() && (memcmp(wifiPVar.data.mac, message.mac, 6) == 0);

        switch (msg->type) {
            case MSG_HEARTBEAT: {
                if (paired_mac) {
                    log(DEBUG, LOG_HEADER, "RESET TIMEOUT");
                    heartbeat_timeout.set(HEARTBEAT_TIMEOUT);
                }
                    
                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_REQ: {
                typed_msg_t msg = {
                    .type = MSG_PAIRING_NACK,
                    .len = 0
                };
                if (is_pairing()) {
                    log(DEBUG, LOG_HEADER, "paired");

                    memcpy(wifiPVar.data.mac, message.mac, 6);
                    wifiPVar.data.paired = true;
                    wifiPVar.write();

                    pairing_timer.ring();

                    msg.type = MSG_PAIRING_ACK;
                }

                esp_now_peer_info_t peer = make_peer(message.mac, 0, 0);
                if (!paired_mac)
                    add_peer(&peer);
                _send(&msg);

                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_ACK: {
                log(DEBUG, LOG_HEADER, "paired");

                delete_peers();

                memcpy(wifiPVar.data.mac, message.mac, 6);
                wifiPVar.data.paired = true;
                wifiPVar.write();

                esp_now_peer_info_t peer = make_peer(message.mac, 0, 0);
                add_peer(&peer);

                pairing_timer.ring();
                dispose_msg(&message);
                break;
            }
            
            case MSG_PAIRING_NACK: {
                pairing_timer.ring();
                dispose_msg(&message);
                break;
            }
            
            case MSG_USER: {
                if (paired_mac) {

                }
                dispose_msg(&message);
                break;
            }

            case MSG_UNPAIR: {
                unpair(false);
                dispose_msg(&message);
                break;                
            }
            
            default: {
                dispose_msg(&message);
                break;
            }
        }
    }

    if (is_paired() && heartbeat.is_ringing()) {
        heartbeat.set(HEARTBEAT_MS);
        typed_msg_t msg = {
            .type = MSG_HEARTBEAT,
            .len = 0
        };
        _send(&msg);
    }
}


bool MessageLink::is_connected() {
    return is_paired() && !heartbeat_timeout.is_ringing();
}*/