#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

#define DEBUG   0
#define WARN    1
#define ERROR   2

#define DIGIT_PAD 2

#define HEAD_PAD 12

template<class T>
static void log(int type, const char* header, T data, bool nl=true) {
    if (Serial) {
        long time = millis();
        Serial.write('[');
        Serial.print(time / 1000);
        Serial.write('.');
        int ms = time % 1000;
        if (ms < 100) {
            Serial.write('0');
            if (ms < 10)
                Serial.write('0');
        }
        Serial.print(ms);
        Serial.print("] [");
        Serial.print(xPortGetCoreID());
        Serial.write("|");
        int priority = uxTaskPriorityGet(NULL);
        if (priority < 10)
            Serial.write(" ");
        Serial.print(priority);
        Serial.write("|");
        switch (type) {
            case DEBUG:
                Serial.write("D");
                break;
            case WARN:
                Serial.write("W");
                break;
            default:
                Serial.write("E");
                break;
        }
        Serial.print("] [");
        Serial.print(header);
        for (int i = 0; i < HEAD_PAD - strlen(header); i++)
            Serial.write(" ");
        Serial.print("]: ");
        Serial.print(data);
        if (nl)
            Serial.write('\n');
    }
}

template<class T>
static void logc(T data, bool nl=true) {
    if (Serial) {
        Serial.print(data);
        if (nl)
            Serial.write('\n');
    }
}

static void logf() {
    if (Serial)
        Serial.println("[ FAILED ]");
}

static void logs() {
    if (Serial)
        Serial.println("[ SUCCESS ]");
}

template<class T>
static void logx(T number, bool nl=true) {
    if (Serial) {
        Serial.print(number, HEX);
        if (nl)
            Serial.write('\n');
    }
}

#endif