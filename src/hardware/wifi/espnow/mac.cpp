#include "mac.h"


// MAC class
MAC::MAC() {

}

MAC::MAC(const uint8_t* mac) {
    set(mac);
}

void MAC::set(const uint8_t* mac) {
    memcpy(this->mac, mac, MAC_SIZE);
}

void MAC::set(MAC& mac) {
    memcpy(this->mac, mac.get(), MAC_SIZE);
}

uint8_t* MAC::get() {
    return mac;
}

bool MAC::is(const uint8_t* mac) {
    return memcmp(this->mac, mac, MAC_SIZE) == 0;
}

bool MAC::is(MAC& mac) {
    return memcmp(this->mac, mac.get(), MAC_SIZE) == 0;
}

bool MAC::is(MAC* mac) {
    return memcmp(this->mac, mac->get(), MAC_SIZE) == 0;
}

void MAC::print() {
    for (int i = 0; i < MAC_SIZE; i++) {
        logx(mac[i], false);
        if (i != 5)
            logc(", ", false);
    }
}


// MACConnection
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
        return -100;
    return rssi;
}


// Whitelist
Whitelist::Whitelist() {

}

void Whitelist::add(MAC& mac) {
    if (!contains(mac))
        connections.emplace_back(mac);
}

void Whitelist::remove(MAC& mac) {
    std::list<MACConnection>::iterator it;
    if (_get(mac, it))
        connections.erase(it);
}

bool Whitelist::contains(MAC& mac) {
    std::list<MACConnection>::iterator it;
    return _get(mac, it);
}

void Whitelist::set_rssi(MAC& mac, int rssi) {
    std::list<MACConnection>::iterator it;
    if (_get(mac, it))
        it->set_rssi(rssi);
}

int Whitelist::get_rssi(MAC& mac) {
    std::list<MACConnection>::iterator it;
    if (_get(mac, it))
        return it->get_rssi();
    return -100;
}

bool Whitelist::_get(MAC& mac, std::list<MACConnection>::iterator& it) {
    for (it = connections.begin(); it != connections.end(); it++) {
        if (it->is(mac))
            return true;
    }
    return false;
}
