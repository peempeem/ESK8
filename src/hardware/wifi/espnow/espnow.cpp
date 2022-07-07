#include "espnow.h"

const static char* LOG_HEADER = "ESPNOW";

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
        uint8_t lenght;                     //
        uint8_t organization_identifier[3]; // 0x18fe34
        uint8_t type;                       // 4
        uint8_t version;
        uint8_t body[0];
    } vendor_specific_content;
} __attribute__ ((packed)) espnow_frame_format_t;


InboundMessages inbound;
OutboundMessages outbound;
Whitelist whitelist;


Whitelist* get_whitelist() {
    return &whitelist;
}


void sender_update() {
    outbound.update();
}

void send_cb(const uint8_t* mac, esp_now_send_status_t status) {
    MAC receiver(mac);

    outbound.approve(receiver, status == ESP_NOW_SEND_SUCCESS);
}

void recv_cb(const uint8_t* mac, const uint8_t* data, int len) {
    MAC sender(mac);
    if (!whitelist.contains(sender))
        return;
    
    wifi_promiscuous_pkt_t* promiscuous_pkt = (wifi_promiscuous_pkt_t*)(data - sizeof(wifi_pkt_rx_ctrl_t) - sizeof(espnow_frame_format_t));
    whitelist.set_rssi(sender, promiscuous_pkt->rx_ctrl.rssi);
    
    send_msg_t* sent_msg = (send_msg_t*) data;
    if (len == 0 || 
        len < sizeof(SEND_MSG::HEADER))
        return;

    inbound.preen();
    inbound.add(sender, sent_msg);
}

bool espnow_init() {    
    if (esp_now_init() != ESP_OK)
        return false;
    
    if (esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR) != ESP_OK)
        return false;

    if (esp_now_register_send_cb(send_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register send callback");
        return false;
    }
    
    if (esp_now_register_recv_cb(recv_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register recv callback");
        return false;
    }

    return true;
}

bool add_peer(esp_now_peer_info_t* peer) {
    MAC* mac = (MAC*) peer->peer_addr;
    log(DEBUG, LOG_HEADER, "Connecting to peer with MAC [", false);
    mac->print();
    logc("] -> ", false);
    if (esp_now_add_peer(peer) != ESP_OK) {
        logf();
        return false;
    }
    logs();
    return true;
}

bool send_msg(msg_t& msg) {
    msg.type += 10;
    return outbound.send(msg);
}

bool get_inbound_message(recv_msg_t* msg) {
    bool ret = inbound.get(msg);
    msg->type -= 10;
    return ret;
}

void dispose_msg(recv_msg_t& msg) {
    delete[] msg.data;
}