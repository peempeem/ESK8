#include "MessageStreamer.h"

StreamerBase::StreamerBase(int cycles) {
    if (cycles <= 0)
        infinite = true;
    else if (cycles == 1)
        single = true;
    this->cycles = cycles;
}

uint8_t* StreamerBase::get() {
    return NULL;
}

int StreamerBase::size() {
    return 0;
}

void StreamerBase::lock() {
    portENTER_CRITICAL(&spinlock);
}

void StreamerBase::unlock() {
    portEXIT_CRITICAL(&spinlock);
}

void StreamerBase::stop() {
    stopped = true;
}

bool StreamerBase::is_expired() {
    if (infinite)
        return false;
    return --cycles < 0;
}

bool StreamerBase::is_ready() {
    if (single)
        return true;
    return rate.isReady();
}

bool StreamerBase::is_stopped() {
    return stopped;
}
