#include "msg_io.h"

// Lock

Lock::Lock() {

}

void Lock::lock() {
    portENTER_CRITICAL(&spinlock);
}

void Lock::unlock() {
    portEXIT_CRITICAL(&spinlock);
}


// RecvMsgBuilder
RecvMsgBuilder::RecvMsgBuilder(send_msg_t* msg) {
    if (msg->header.pkt == 0) {
        id = msg->header.id;
        current_pkt = msg->header.pkt;
        type = msg->header.type;
        current_size = (uint16_t) min((int) msg->header.len, (int) msg->header.size);
        max_size = msg->header.size;
        
        if (max_size == 0) {
            buffer = NULL;
            zero_size = true;
        } else {
            buffer = new uint8_t[max_size];
            memcpy(buffer, msg->data, current_size);
        }
        
        last = millis();
    }
}

bool RecvMsgBuilder::add(send_msg_t* msg) {
    lock();
    if (msg->header.pkt == current_pkt + 1 &&
        id == (int) msg->header.id && 
        !zero_size) {
        uint16_t copy_size = (uint16_t) min((int) msg->header.len, max_size - current_size);
        memcpy(buffer + (int) current_size, msg->data, (int) copy_size);
        current_size += copy_size;
        current_pkt++;
        last = millis();
        unlock();
        return true;
    }
    unlock();
    return false;
}

bool RecvMsgBuilder::get(recv_msg_t& msg) {
    lock();
    if (!built && current_size >= max_size) {
        msg.type = type;
        msg.data = buffer;
        msg.len = max_size;
        built = true;
        unlock();
        return true;
    }
    unlock();
    return false;
}

bool RecvMsgBuilder::is_expired() {
    return (millis() - last >= timeout) || built;
}

void RecvMsgBuilder::clear() {
    built = true;
    delete[] buffer;
}

bool RecvMsgBuilder::is_built() {
    return built;
}


// RecvMessages
RecvMessages::RecvMessages() {
    last = millis();
}

void RecvMessages::add(send_msg_t* msg) {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        // check if message is already being built
        if (it->add(msg))
            break;
    }
    // check if new message
    if (it == builders.end() && msg->header.pkt == 0)
        builders.emplace_back(msg);
    last = millis();
}

bool RecvMessages::get(recv_msg_t& msg) {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        if (it->get(msg))
            return true;
    }
    return false;
}

void RecvMessages::preen() {
    std::list<RecvMsgBuilder>::iterator it;
    for (it = builders.begin(); it != builders.end(); it++) {
        if (it->is_expired()) {
            if (!it->is_built())
                it->clear();
            it = builders.erase(it);
        }
    }
}

bool RecvMessages::expired() {
    return millis() - last >= timeout;
}


// SendMessage
SendMessage::SendMessage(uint16_t id, uint16_t type, uint8_t* data, uint16_t len, int retries) {
    this->retries = 0;
    max_retries = retries;

    uint16_t size = (uint16_t) min((int) sizeof(SEND_MSG::HEADER) + (int) len, ESP_NOW_MAX_SIZE);
    send_msg_t* send_msg = (send_msg_t*) new uint8_t[size];
    if (send_msg == NULL)
        return;
    
    send_msg->header.id = id;
    send_msg->header.pkt = 0;
    uint16_t copy_size = size - (uint16_t) sizeof(SEND_MSG::HEADER);
    send_msg->header.len = (uint8_t) copy_size;
    send_msg->header.type = type;
    send_msg->header.size = len;
    memcpy(send_msg->data, data, (int) copy_size);
    buf_t buf = {
        .data = (uint8_t*) send_msg,
        .len = (int) size
    };
    parts.push(buf);

    int bytes = (int) copy_size;
    uint8_t pkt = (uint8_t) 1;
    while (bytes < (int) len) {
        size = (uint16_t) min((int) sizeof(SEND_MSG::HEADER) + (int) len - bytes, ESP_NOW_MAX_SIZE);
        send_msg_t* send_msg = (send_msg_t*) new uint8_t[size];
        send_msg->header.id = (uint16_t) id;
        send_msg->header.pkt = pkt++;
        copy_size = size - (uint16_t) sizeof(SEND_MSG::HEADER);
        send_msg->header.len = (uint8_t) copy_size;
        send_msg->header.type = type;
        send_msg->header.size = len;
        memcpy(send_msg->data, data + bytes, (int) copy_size);
        buf.data = (uint8_t*) send_msg;
        buf.len = (int) size;
        bytes += (int) copy_size;
        parts.push(buf);
    }
}

void SendMessage::clear() {
    while (!parts.empty()) {
        delete[] parts.front().data;
        parts.pop();
    }
}

bool SendMessage::get_msg(buf_t* msg) {
    if (!is_stale() && !is_empty()) {
        msg->data = parts.front().data;
        msg->len = parts.front().len;
        return true;
    }
    return false;
}

bool SendMessage::next() {
    delete[] parts.front().data;
    parts.pop();
    retries = 0;
}

void SendMessage::try_again() {
    retries++;
}

bool SendMessage::is_stale() {
    return retries > max_retries;
}

bool SendMessage::is_empty() {
    return parts.empty();
}


// SendMessages
SendMessages::SendMessages() {
    last = millis();
}

void SendMessages::create_msg(uint16_t type, uint8_t* data, uint16_t len, int retries) {
    msgs.emplace_back(id++, type, data, len, retries);
}

bool SendMessages::get_msg(buf_t* msg) {
    if (msgs.size() == 0)
        return false;
    
    while (!msgs.empty()) {
        if (msgs.front().get_msg(msg))
            return true;
        
        msgs.front().clear();
        msgs.pop_front();
    }
    return false;
}

void SendMessages::preen() {
    if (msgs.size() == 0)
        return;
    
    if (msgs.front().is_stale() || msgs.front().is_empty()) {
        msgs.front().clear();
        msgs.erase(msgs.begin());
    }
}

void SendMessages::approve(bool approve) {
    if (msgs.size() == 0)
        return;
    
    if (approve)
        msgs.front().next();
    else
        msgs.front().try_again();
    preen();
    msgs.splice(msgs.end(), msgs, msgs.begin());
    set_ready(true);
}

bool SendMessages::is_ready() {
    if (millis() - last >= RESEND_TIMEOUT) {
        last = millis();
        return true;
    }
    return ready;
}

void SendMessages::set_ready(bool ready) {
    this->ready = ready;
    last = millis();
}

int SendMessages::current_msgs_count() {
    return msgs.size();
}