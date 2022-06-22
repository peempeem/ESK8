#include "icon.h" 

typedef struct ICON_PROPERTIES {
    int width, height;
} iprop_t;

Icon::Icon(const char* name) {
    _name = name;

    iprop_t prop;
    if (filesys.readFile(name, (uint8_t*) &prop, sizeof(prop))) {
        _width = prop.width;
        _height = prop.height;
    }
}

Icon::~Icon() { deallocate(); }

int Icon::width() { return _width; }

int Icon::height() { return _height; }

static void _allocate(void* data) {
    icon_data_t* idata = (icon_data_t*) data;
    if (idata->width <= 0 | idata->height <= 0 | *idata->bitmap != NULL) {
        *idata->allocating = false;
        vTaskDelete(0);
    }
    uint16_t* bitmap = new uint16_t[idata->width * idata->height];
    if (!filesys.readFile(idata->name, (uint8_t*) bitmap, idata->width * idata->height * 2, sizeof(iprop_t)))
        delete bitmap;
    else
        *idata->bitmap = bitmap;
    *idata->allocating = false;
    vTaskDelete(0);
}

bool Icon::allocate(bool async, int priority) {
    if (_allocating || allocated())
        return false;
    if (async) {
        _idata.width = _width;
        _idata.height = _height;
        _idata.name = _name;
        _idata.bitmap = &_bitmap;
        _idata.allocating = &_allocating;
        _allocating = true;

        xTaskCreate(_allocate, "icon_alloc", 2048, (void*) &_idata, priority, NULL);
        return true;
    } else {
        if (_width <= 0 || _height <= 0 || _bitmap != NULL)
            return false;
        _bitmap = new uint16_t[_width * _height];
        if (!filesys.readFile(_name, (uint8_t*) _bitmap, _width * _height * 2, sizeof(iprop_t))) {
            delete _bitmap;
            _bitmap = NULL;
            return false;
        }
        return true;
    }
}

bool Icon::allocated() { return _bitmap != NULL; }

void Icon::deallocate() {
    delete _bitmap;
    _bitmap = NULL;
}

uint16_t* Icon::bitmap() { return _bitmap; }
