#ifndef ICON_H
#define ICON_H

#include <util/FileSystem.h>

typedef struct ICON_DATA {
    int width, height;
    const char* name;
    uint16_t** bitmap;
    bool* allocating;
} icon_data_t;

class Icon {
    public:
        Icon(const char* name);
        ~Icon();

        int         width();
        int         height();
        bool        allocate(bool async, int priority=2);
        bool        allocated();
        void        deallocate();
        uint16_t*   bitmap();
    
    protected:
        int         _width  = -1;
        int         _height = -1;
        uint16_t*   _bitmap = NULL;
        const char* _name;
        bool        _allocating = false;

        icon_data_t _idata;
};

#endif
