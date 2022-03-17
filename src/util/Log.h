#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

#define DEBUG   0
#define WARN    1
#define ERROR   2

#define DIGIT_PAD 2

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
        Serial.print("] [ ");
        switch (type) {
            case DEBUG:
                Serial.print("DEBUG");
                break;
            case WARN:
                Serial.print("WARN ");
                break;
            default:
                Serial.print("ERROR");
                break;
        }
        Serial.print(" ] [ ");
        Serial.print(header);
        Serial.print(" ]: ");
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

#endif