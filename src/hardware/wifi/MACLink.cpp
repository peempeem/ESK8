#include "MACLink.h"


MACLink::MACLink() {
    
}

MACLink::MACLink(MAC& mac, int dc_timeout_ms, int msg_timeout) {
    this->mac.set(mac);
    heartbeat.set(500);
    heartbeat.ring();
    this->msg_timeout.set(msg_timeout);
    this->msg_timeout.ring();
    dc_timer.set(dc_timeout_ms);
}

bool MACLink::is(MAC& mac) {
    return this->mac.is(mac);
}

MAC MACLink::get_mac() {
    return mac;
}

bool MACLink::is_connected() {
    return rssi > DC_RSSI;
}

int MACLink::get_rssi() {
    return rssi;
}

bool MACLink::send(uint16_t type, uint8_t* data, uint16_t len, int retries) {
    if (!is_connected())
        return false;
    _send(type + ReservedMsgs::SIZE, data, len, retries);
}

bool MACLink::get(recv_msg_t& msg) {
    if (recv_msgs.get(msg)) {
        msg.type -= ReservedMsgs::SIZE;
        recv_msgs.preen();
        return true;
    }
    recv_msgs.preen();
    return false;
}

void MACLink::dispose(recv_msg_t& msg) {
    delete[] msg.data;
}

void MACLink::update() {
    if (dc_timer.is_ringing())
        rssi = DC_RSSI;
    
    buf_t msg;
    if (send_msgs.is_ready() && send_msgs.get_msg(&msg)) {
        send_msgs.set_ready(false);
        esp_now_send(mac.get(), msg.data, msg.len);
        msg_timeout.reset();
        heartbeat.reset();
    }

    if (heartbeat.is_ringing())
        _send(ReservedMsgs::Heartbeat, NULL, 0, 0);
    
    if (msg_timeout.is_ringing()) {
        send_msgs.approve(false);
        msg_timeout.reset();
    }
}

int MACLink::get_num_msgs() {
    return send_msgs.current_msgs_count();
}

void MACLink::_recv_cb(const uint8_t* data, int len) {
    wifi_promiscuous_pkt_t* promiscuous_pkt = (wifi_promiscuous_pkt_t*) (data - sizeof(wifi_pkt_rx_ctrl_t) - sizeof(espnow_frame_format_t));
    rssi = promiscuous_pkt->rx_ctrl.rssi;

    send_msg_t* recv_msg = (send_msg_t*) data;

    if (len < sizeof(SEND_MSG::HEADER) || (int) (recv_msg->header.len + sizeof(SEND_MSG::HEADER)) != len)
        return;
    
    switch ((int) recv_msg->header.type) {
        case ReservedMsgs::Heartbeat:
            break;
        default:
            recv_msgs.add(recv_msg);
            break;
    }
    
    dc_timer.reset();
}

void MACLink::_send_cb(esp_now_send_status_t status) {
    send_msgs.approve(status == ESP_NOW_SEND_SUCCESS);
}

void MACLink::_send(uint16_t type, uint8_t* data, uint16_t len, int retries) {
    send_msgs.create_msg(type, data, len, retries);
}