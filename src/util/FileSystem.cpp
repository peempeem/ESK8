#include "FileSystem.h"

const static char* LOG_HEADER = "FileSystem";

bool ESK8FS::init(bool format) {
    log(DEBUG, LOG_HEADER, "Loading filesystem -> ", false);

    if (!fs.begin(true)) {
        logf();
        log(ERROR, LOG_HEADER, "Failed to mount SPIFFS filesystem (bad flash?)");
        return false;
    }
    logs();
    initialized = true;
    return true;
}

bool ESK8FS::readFile(const char* path, uint8_t* buf, int size) {
    File file = fs.open(path);
    if (!file || file.read(buf, size) != size) {
        log(ERROR, LOG_HEADER, path, false);
        logc(" does not exist");
        return false;
    }
    file.close();
    return true;
}

bool ESK8FS::writeFile(const char* path, uint8_t* buf, int size) {
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        log(WARN, LOG_HEADER, "File does not exist, creating ", false);
        logc(path);
    }
    file.write(buf, size);
    file.close();
    return true;
}

bool ESK8FS::deleteFile(const char* path) {
    if (fs.remove(path)) {
        log(DEBUG, LOG_HEADER, "Removed ", false);
        logc(path);
        return true;
    }
    log(DEBUG, LOG_HEADER, "Failed to remove ", false);
    logc(path);
    return false;
}

std::vector<FileData> ESK8FS::map(const char* path) {
    std::vector<FileData> files;
    if (initialized) {
        File root = fs.open(path);
        if (!root.isDirectory())
            return files;
        File file = fs.open(path);
        FileData fd;
        while (file) {
            strcpy(fd.name, file.name());
            fd.size = file.size();
            file.close();
            files.push_back(fd);
            file = root.openNextFile();
        }
        root.close();
    }
    return files;
}

bool ESK8FS::clearFS() {
    log(DEBUG, LOG_HEADER, "Clearing filesystem...");
    std::vector<FileData> files = map("/");
    for (int i = 0; i < files.size(); i++) {
        if (strlen(files[i].name) == 1)
            continue;
        if (!deleteFile(files[i].name)) {
            log(ERROR, LOG_HEADER, "Couldn't clear filesystem");
            return false;
        }       
    }
    log(DEBUG, LOG_HEADER, "Cleared filesystem");
    return true;
}

void ESK8FS::logMap(std::vector<FileData> files) {
    log(DEBUG, LOG_HEADER, "Enumerating files...");
    char buf[12];
    for (int i = 0; i < files.size(); i++) {
        log(DEBUG, LOG_HEADER, "Size: ", false);
        int len = snprintf(buf, 12, "%d", files[i].size);
        logc(buf, false);
        int j;
        for (j = 0; j < 11 - len; j++)
            buf[j] = ' ';
        buf[j] = 0;
        logc(buf, false);
        logc(files[i].name);
    }
    log(DEBUG, LOG_HEADER, "Done enumerating");
}