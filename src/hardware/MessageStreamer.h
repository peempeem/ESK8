#ifndef MESSAGE_STREAMER_H
#define MESSAGE_STREAMER_H

#include "../util/rate.h"
#include "task.h"


class StreamerBase {
    public:
        Rate rate;

        StreamerBase() { }
        StreamerBase(float rate) { this->rate.setRate(rate) }

        virtual uint8_t* get();
        virtual int size();

        void lock() {
            portENTER_CRITICAL(&spinlock);
        }

        void unlock() {
            portEXIT_CRITICAL(&spinlock);
        }

    protected:
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
};

template<class T>
class MessageStreamer : StreamerBase {
    public:
        T data;
        
        MessageStreamer() : StreamerBase() { };
        MessageStreamer(float rate) : StreamerBase(rate) { };

        void set() {
            lock();
            _data = data;
            unlock();
        }

        uint8_t* get() {
            return (uint8_t*) &data;
        }

        int size() {
            return sizeof(T);
        }

    private:
        T _data;
};

#endif