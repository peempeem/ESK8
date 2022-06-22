#ifndef PVAR_H
#define PVAR_H

#include "FileSystem.h"

template<class T>
class PVar {
    public:
        T data;

        PVar(const char* name) { _name = name; }

        bool load() {
            if (!filesys.readFile(_name, (uint8_t*) &data, sizeof(T))) {
                log(ERROR, _name, "Error retrieving data from filesystem");
                return false;
            }
            return true;
        }

        bool write() {
            if (!filesys.writeFile(_name, (uint8_t*) &data, sizeof(T))) {
                log(ERROR, _name, "Couldn't write to file");
                return false;
            }
            return true;
        }
    
    private:
        const char* _name;
};

#endif