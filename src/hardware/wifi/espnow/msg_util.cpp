#include "msg_util.h"

// SendMessage
SendMessage::SendMessage() { }

void SendMessage::make(int id, int retries, msg_t& msg) {
    max_retries = retries;

    uint16_t size = (uint16_t) min((int) sizeof(SEND_MSG::HEADER) + (int) msg.len, ESP_NOW_MAX_SIZE);
    send_msg_t* send_msg = (send_msg_t*) new (std::nothrow) uint8_t[size];
    if (send_msg == NULL)
        return;
    
    send_msg->header.id = (uint16_t) id;
    send_msg->header.pkt = (uint8_t) 0;
    uint16_t copy_size = size - (uint16_t) sizeof(SEND_MSG::HEADER);
    send_msg->header.len = (uint8_t) copy_size;
    send_msg->header.type = msg.type;
    send_msg->header.size = msg.len;
    memcpy(send_msg->data, msg.data, (int) copy_size);
    buf_t buf = {
        .data = (uint8_t*) send_msg,
        .len = (int) size
    };
    parts.push(buf);

    int bytes = (int) copy_size;
    uint8_t pkt = (uint8_t) 1;
    while (bytes < (int) msg.len) {
        size = (uint16_t) min((int) sizeof(SEND_MSG::HEADER) + (int) msg.len - bytes, ESP_NOW_MAX_SIZE);
        send_msg_t* send_msg = (send_msg_t*) new uint8_t[size];
        send_msg->header.id = (uint16_t) id;
        send_msg->header.pkt = pkt++;
        copy_size = size - (uint16_t) sizeof(SEND_MSG::HEADER);
        send_msg->header.len = (uint8_t) copy_size;
        send_msg->header.type = msg.type;
        send_msg->header.size = msg.len;
        memcpy(send_msg->data, msg.data + bytes, (int) copy_size);
        buf.data = (uint8_t*) send_msg;
        buf.len = (int) size;
        bytes += (int) copy_size;
        parts.push(buf);
    }
    made = true;
}

void SendMessage::clear() {
    while (!parts.empty()) {
        delete[] parts.front().data;
        parts.pop();
    }
    made = false;
}

bool SendMessage::get_msg(buf_t* msg) {
    if (!parts.empty() && retries <= max_retries) {
        msg->data = parts.front().data;
        msg->len = parts.front().len;
        retries++;
        return true;
    }
    return false;
}

void SendMessage::next() {
    if (parts.empty()) 
        return;
    delete[] parts.front().data;
    parts.pop();
    retries = 0;
}

bool SendMessage::is_made() {
    return made;
}