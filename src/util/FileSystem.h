#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "SPIFFS.h"
#include "Log.h"

#define MAX_PATH    64

struct FileData {
    char name[MAX_PATH];
    int size;
};

class ESK8FS {
    public:
        ESK8FS() {}

        bool init(bool format=false);
        bool readFile(const char* path, uint8_t* buf, int size, int seek=0);
        bool writeFile(const char* path, uint8_t* buf, int size, int seek=0);
        bool deleteFile(const char* path);
        std::vector<FileData> map(const char* path);
        bool clearFS();
        void logMap(std::vector<FileData> files);


    private:
        bool initialized = false;
        fs::SPIFFSFS fs = SPIFFS;
} static filesys;

#endif