#ifndef PVAR_H
#define PVAR_H

#include "FileSystem.h"

template<class T>
class PVar {
    public:
        T data;

        PVar(const char* name, ESK8FS* fs) {
            this->name = name;
            this->fs = fs;
        }

        bool load() {
            if (!fs->readFile(name, (uint8_t*) &data, sizeof(T))) {
                log(ERROR, name, "Error retrieving data from filesystem");
                return false;
            }
            return true;
        }

        bool write() {
            if (!fs->writeFile(name, (uint8_t*) &data, sizeof(T))) {
                log(ERROR, name, "Couldn't write to file");
                return false;
            }
            return true;
        }
    
    private:
        const char* name;
        ESK8FS* fs;
};

#endif