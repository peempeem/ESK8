#include "MACConnection.h"

MACConnection::MACConnection(MAC& mac) {
    this->mac.set(mac);
    last = millis();
}

bool MACConnection::is(MAC& mac) {
    return this->mac.is(mac);
}

void MACConnection::set_rssi(int rssi) {
    this->rssi = rssi;
    last = millis();
}

int MACConnection::get_rssi() {
    if (millis() - last >= timeout)
        return RSSI_UNCONNECTED;
    return rssi;
}

void MACConnection::add_streamer(StreamerBase* streamer) {
    streamers.push_back(streamer);
}

bool MACConnection::get_msg(msg_t& msg) {
    if (recv_msgs.empty())
        return false;
    msg = recv_msgs.front();
    recv_msgs.pop();
    return true;
}