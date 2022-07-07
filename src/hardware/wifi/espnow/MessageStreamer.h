#ifndef MESSAGE_STREAMER_H
#define MESSAGE_STREAMER_H

#include "msg_util.h"
#include <freertos/task.h>


class StreamerBase {
    public:
        Rate rate;

        StreamerBase(int cycles);

        virtual uint8_t* get();
        virtual int size();

        void lock();
        void unlock();

        void stop();
        
        bool is_expired();
        bool is_ready();
        bool is_stopped();    

    protected:
        portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
    
    private:
        int cycles;
        bool single = false;
        bool infinite = false;
        bool stopped = false;
        
        SendMessage send_msg;
};

template<class T>
class MessageStreamer : StreamerBase {
    public:
        T data;
        
        MessageStreamer(int cycles) : StreamerBase(cycles) { }

        void set() {
            lock();
            memcpy(&_data, &data, sizeof(T));
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