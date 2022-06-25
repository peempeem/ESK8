#include "espnow.h"
#include "../util/Log.h"
#include <queue>

const static char* LOG_HEADER = "ESPNOW";

static std::queue<sent_msg_t> sent_msgs_queue;
static std::queue<recv_msg_t> recv_msgs_queue;

static void send_cb(const uint8_t* mac, esp_now_send_status_t status) {
    sent_msg_t msg;
    memcpy(msg.mac, mac, 6);
    msg.status = status;
    sent_msgs_queue.push(msg);
    log(DEBUG, LOG_HEADER, "SENT DATA");
    if (status != ESP_NOW_SEND_SUCCESS)
        log(ERROR, LOG_HEADER, "SEND FAILED");
}

static void recv_cb(const uint8_t* mac, const uint8_t* data, int len) {
    recv_msg_t msg;
    memcpy(msg.mac, mac, 6);
    msg.data = new uint8_t[len];
    memcpy(msg.data, data, len);
    msg.len = len;
    recv_msgs_queue.push(msg);
    log(DEBUG, LOG_HEADER, "RECV DATA");
}

bool espnow_init(bool sta, bool send_cb_enabled) {
    log(DEBUG, LOG_HEADER, "Initializing ESP-NOW -> ", false);

    if (sta)
        WiFi.mode(WIFI_STA);
    else {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(String(random(0, 100) * 100000000).c_str(), "password", 0);
    }
    
    if (esp_now_init() != ESP_OK) {
        logf();
        return false;
    }
    logs();

    if (send_cb_enabled) {
        if (esp_now_register_send_cb(send_cb) != ESP_OK)
            log(ERROR, LOG_HEADER, "Failed to register send callback");
    }
    if (esp_now_register_recv_cb(recv_cb) != ESP_OK)
        log(ERROR, LOG_HEADER, "Failed to register recv callback");
    return true;
}

bool add_peer(esp_now_peer_info_t* peer) {
    log(DEBUG, LOG_HEADER, "Connecting to Peer with MAC [", false);
    for (int i = 0; i < 6; i++) {
        logx(peer->peer_addr[i], false);
        if (i == 5)
            logc("] -> ", false);
        else
            logc(", ", false);
    }
    if (esp_now_add_peer(peer) != ESP_OK) {
        logf();
        return false;
    }
    logs();
    return true;
}

bool outbound_messages() { return !sent_msgs_queue.empty(); }

bool inbound_messages() { return !recv_msgs_queue.empty(); }

sent_msg_t get_outbound_message() {
    sent_msg_t msg;
    if (!sent_msgs_queue.empty()) {
        msg = sent_msgs_queue.front();
        sent_msgs_queue.pop();
    }
    return msg;
}

recv_msg_t get_inbound_message() {
    recv_msg_t msg;
    if (!recv_msgs_queue.empty()) {
        msg = recv_msgs_queue.front();
        recv_msgs_queue.pop();
    }
    return msg;
}

void dispose_msg(recv_msg_t* msg) { delete msg->data; }