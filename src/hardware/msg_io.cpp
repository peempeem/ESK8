#include "msg_io.h"


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

// RecvMsgBuilder
RecvMsgBuilder::RecvMsgBuilder(send_msg_t* msg) {
    if (msg->header.pkt == 0) {
        id = (int) msg->header.id;
        type = (int) msg->data.first.header.type;
        max_size = (int) msg->data.first.header.size;
        current_size = min((int) msg->header.len, max_size);
        current_pkt = 0;
        buffer = new uint8_t[max_size];
        memcpy(buffer, msg->data.first.data, current_size);
        last = millis();
    } else
        max_size = -1;
}

bool RecvMsgBuilder::add(send_msg_t* msg) {
    if (max_size != -1 && 
        msg->header.pkt == current_pkt + 1 &&
        id == (int) msg->header.id) {
        int copy_size = min((int) msg->header.len, max_size - current_size);
        memcpy(buffer + current_size, msg->data.next.data, copy_size);
        current_size += copy_size;
        current_pkt++;
        last = millis();
        return true;
    }    
    return false;
}

bool RecvMsgBuilder::get(recv_msg_t* msg) {
    if (max_size != -1 && current_size >= max_size) {
        msg->type = type;
        msg->data = buffer;
        msg->len = max_size;
        built = true;
        return true;
    }
    return false;
}

bool RecvMsgBuilder::is_expired() {
    return (millis() - last >= timeout) || (max_size == - 1);
}

void RecvMsgBuilder::clear() {
    max_size = -1;
    delete[] buffer;
}

bool RecvMsgBuilder::is_built() {
    return built;
}


// RecvMsgBuilderList
RecvMsgBuilderList::RecvMsgBuilderList(MAC& mac) {
    this->mac.set(mac.get());
    last = millis();
}

void RecvMsgBuilderList::add(send_msg_t* msg) {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        // check if message is already being built
        if (it->add(msg))
            break;
    }
    // check if new message
    if (it == builders.end() && msg->header.pkt == 0) {
        RecvMsgBuilder recv(msg);
        builders.push_back(recv);
    }
        
    last = millis();
}

bool RecvMsgBuilderList::get(recv_msg_t* msg) {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        if (it->get(msg)) {
            msg->mac.set(mac);
            return true;
        }
    }
    return false;
}

void RecvMsgBuilderList::preen() {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        if (it->is_expired() || it->is_built()) {
            if (!it->is_built())
                it->clear();
            it = builders.erase(it);
        }
    }
}

bool RecvMsgBuilderList::expired() {
    return millis() - last >= timeout;
}

// InboundMessages
InboundMessages::InboundMessages() {

}

void InboundMessages::add(MAC& mac, send_msg_t* msg) {
    std::list<RecvMsgBuilderList>::iterator it;
    for (it = inbound.begin(); it != inbound.end(); it++) {
        // check if mac address matches
        if (it->mac.is(mac)) {
            it->add(msg);
            recv_msg_t recv;
            if (it->get(&recv))
                msgs.push(recv);
            break;
        }
    }
    // check if new mac
    if (it == inbound.end()) {
        RecvMsgBuilderList rmbl(mac);
        rmbl.add(msg);
        recv_msg_t recv;
        if (rmbl.get(&recv))
            msgs.push(recv);
        inbound.push_back(rmbl);
    }
}

void InboundMessages::preen() {
    std::list<RecvMsgBuilderList>::iterator it;
    for (it = inbound.begin(); it != inbound.end(); it++)
        it->preen();
}


bool InboundMessages::get(recv_msg_t* msg) {
    if (!msgs.empty()) {
        memcpy(msg, &msgs.front(), sizeof(recv_msg_t));
        msgs.pop();
        return true;
    }
    return false;
}

// Whitelist
Whitelist::Whitelist() {

}

void Whitelist::add(MAC& mac) {
    if (!contains(mac))
        whitelist.push_back(mac);
}

void Whitelist::remove(MAC& mac) {
    std::list<MAC>::iterator it;
    for (it = whitelist.begin(); it != whitelist.end(); it++) {
        if (it->is(mac)) {
            whitelist.erase(it);
            break;
        }
    }
}

bool Whitelist::contains(MAC& mac) {
    std::list<MAC>::iterator it;
    for (it = whitelist.begin(); it != whitelist.end(); it++) {
        if (it->is(mac))
            return true;
    }
    return false;
}


// SendMessage
SendMessage::SendMessage(int id, msg_t& msg) {
    this->priority = priority;
    this->retries = 0;
    max_retries = retries;

    int size = min(SEND_MSG_FIRST_HEADER + (int) msg.len, ESP_NOW_MAX_SIZE);
    send_msg_t* send_msg = (send_msg_t*) new uint8_t[size];
    send_msg->header.id = (uint16_t) id;
    send_msg->header.pkt = (uint8_t) 0;
    int copy_size = size - SEND_MSG_FIRST_HEADER;
    send_msg->header.len = (uint8_t) copy_size;
    send_msg->data.first.header.type = msg.type;
    send_msg->data.first.header.size = msg.len;
    memcpy(send_msg->data.first.data, msg.data, copy_size);
    buf_t buf = {
        .data = (uint8_t*) send_msg,
        .len = size
    };
    parts.push(buf);

    int bytes = copy_size;
    uint8_t pkt = 1;
    while (bytes < msg.len) {
        size = min(SEND_MSG_NEXT_HEADER + (int) msg.len - bytes, ESP_NOW_MAX_SIZE);
        send_msg_t* send_msg = (send_msg_t*) new uint8_t[size];
        send_msg->header.id = (uint16_t) id;
        send_msg->header.pkt = pkt++;
        copy_size = size - SEND_MSG_NEXT_HEADER;
        send_msg->header.len = (uint16_t) copy_size;
        memcpy(send_msg->data.next.data, msg.data + bytes, copy_size);
        buf.data = (uint8_t*) send_msg;
        buf.len = size;
        bytes += copy_size;
        parts.push(buf);
    }

}

void SendMessage::clear() {
    while (!parts.empty()) {
        delete[] parts.front().data;
        parts.pop();
    }
}

int SendMessage::get_priority() {
    return priority;
}

bool SendMessage::get_msg(buf_t* msg) {
    if (!is_stale() && !is_empty()) {
        msg->data = parts.front().data;
        msg->len = parts.front().len;
        retries++;
        return true;
    }
    return false;
}

bool SendMessage::next() {
    delete[] parts.front().data;
    parts.pop();
    retries = 0;
}

bool SendMessage::is_stale() {
    return retries > max_retries;
}

bool SendMessage::is_empty() {
    return parts.empty();
}


// MACSendMessages
MACSendMessages::MACSendMessages() {
    last = millis();
}

void MACSendMessages::create_msg(int id, msg_t& msg) {
    msgs.emplace_back(id, msg);
}

bool MACSendMessages::get_msg(buf_t* msg) {
    if (msgs.size() == 0)
        return false;
    
    return msgs.front().get_msg(msg);
}

void MACSendMessages::preen() {
    if (msgs.size() == 0)
        return;
    
    if (msgs.front().is_stale() || msgs.front().is_empty()) {
        msgs.front().clear();
        msgs.erase(msgs.begin());
    }
}

void MACSendMessages::approve(bool approve) {
    if (msgs.size() == 0)
        return;
    
    if (approve)
        msgs.front().next();
    preen();
    msgs.splice(msgs.end(), msgs, msgs.begin());
    set_ready(true);
}

bool MACSendMessages::is_ready() {
    if (millis() - last > RESEND_TIMEOUT) {
        last = millis();
        return true;
    }
    return ready;
}

void MACSendMessages::set_ready(bool ready) {
    this->ready = ready;
    last = millis();
}


// OutboundMessages
OutboundMessages::OutboundMessages() {

}

void OutboundMessages::send(msg_t& msg) {
    bool new_mac = true;
    std::list<MACSendMessages>::iterator it;
    for (it = outbound.begin(); it != outbound.end(); it++) {
        if (it->mac.is(msg.mac)) {
            it->create_msg(id++, msg);
            new_mac = false;
            break;
        }
    }
    if (new_mac) {
        MACSendMessages macsm;
        macsm.mac.set(msg.mac);
        macsm.create_msg(id++, msg);
        outbound.push_back(macsm);
    }
}

void OutboundMessages::update() {
    std::list<MACSendMessages>::iterator it;
    buf_t msg;
    for (it = outbound.begin(); it != outbound.end(); it++) {
        if (it->is_ready()) {
            if (!it->get_msg(&msg))
                it->preen();
            else {
                it->set_ready(false);
                esp_now_send(it->mac.get(), msg.data, msg.len);
            }   
        }
    }
}

void OutboundMessages::approve(MAC& mac, bool approve) {
    std::list<MACSendMessages>::iterator it;
    for (it = outbound.begin(); it != outbound.end(); it++) {
        if (it->mac.is(mac)) {
            it->approve(approve);
            break;
        }
    }
}

bool OutboundMessages::is_ready(MAC& mac) {
    bool ready;
    std::list<MACSendMessages>::iterator it;
    for (it = outbound.begin(); it != outbound.end(); it++) {
        if (it->mac.is(mac)) {
            ready = it->is_ready();
            break;
        }
    }
    return ready;
}
