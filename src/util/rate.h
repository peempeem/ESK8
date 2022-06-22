#ifndef RATE_H
#define RATE_H

// class used for scheduling tasks
class Rate {
    public:
        bool    enabled = true;     // rate enable flag

        Rate();
        Rate(float rate);

        void    setRate(float rate);
        bool    isReady();
        void    sleep();
        void    reset();

        float   getStage(bool noChange=false);
        float   getStageSin(bool noChange=false);
        float   getStageCos(bool noChange=false);

    private:
        int     inverseRate = 0;    // (1 / Rate) cache
        int     last = 0;           // last time
};

class Timer {
    public:
        Timer();
        Timer(int ms);

        void set(int ms);
        bool ringing();
    
    private:
        bool is_set = false;
        int ring_time = 0;
};

#endif
