#include "espnow.h"

const static char* LOG_HEADER = "ESPNOW";

InboundMessages inbound;
OutboundMessages outbound;

typedef struct RECV_CB_MSGS {
    MAC mac;
    buf_t buf;
} recv_cb_msgs_t;

std::queue<recv_cb_msgs_t> recv_cb_msgs;


void sender_thread(void* data) {
    while (true) {
        outbound.update();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void send_cb(const uint8_t* mac, esp_now_send_status_t status) {
    log(DEBUG, "SEND CB", "SEND CB");
    MAC recv_mac(mac);

    outbound.approve(recv_mac, status == ESP_NOW_SEND_SUCCESS);
}

void recv_cb(const uint8_t* mac, const uint8_t* data, int len) {
    send_msg_t* sent_msg = (send_msg_t*) data;
    if (len == 0 || 
        len < sizeof(SEND_MSG::HEADER))
        return;

    inbound.preen();
    inbound.add(recv_cb_msgs.front().mac, sent_msg);
}

void promiscuous_rx_cb(void* buf, wifi_promiscuous_pkt_type_t type) {

    // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
    if (type != WIFI_PKT_MGMT)
        return;

    wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*) buf;
    //log(DEBUG, "RSSI", pkt->rx_ctrl.rssi);
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
    
    if (esp_wifi_set_promiscuous(true) != ESP_OK)
        log(ERROR, LOG_HEADER, "Failed to set promiscuous mode");
    
    if (esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb))
        log(ERROR, LOG_HEADER, "Failed to register promiscuous_rx callback");
        
    
    xTaskCreate(sender_thread, "sender_thread", 4096, NULL, 5, NULL);
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

void send_msg(msg_t& msg) {
    msg.type += 0;
    outbound.send(msg);
}

bool get_inbound_message(recv_msg_t* msg) {
    return inbound.get(msg);
}

void dispose_msg(recv_msg_t& msg) {
    delete[] msg.data;
}