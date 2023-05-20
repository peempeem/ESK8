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
