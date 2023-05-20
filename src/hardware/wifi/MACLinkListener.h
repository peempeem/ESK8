#ifndef MACLINKLISTENER_H
#define MACLINKLISTENER_H

#include "MACLink.h"
#include "WiFi.h"
#include <string>


class MACLinkListener : public Lock {
    public:
        MACLinkListener();

        bool init(const char* name);
        void set_cpu_mhz_safe(int mhz);

        void add(MACLink* link);
        void remove(MACLink* link);
        bool contains(MACLink* link);

        void update();

        bool scan(const char* key);
        bool is_scanning();
        bool get_scan_mac(MAC& mac);

        void start_pairing();
        void stop_pairing();
        bool is_pairing();
        bool get_pairing_mac(MAC& mac);
        void _add_pairing_mac(MAC& mac, const uint8_t* data, int len);

        bool _recv_cb(MAC& mac, const uint8_t* data, int len);
        void _send_cb(MAC& mac, esp_now_send_status_t status);
    
    private:
        bool initialied = false;
        bool pairing = false;
        bool scanning = false;
        String key;
        std::list<MACLink*> connections;
        std::list<MAC> pairing_list;
        std::list<MAC> scan_list;
        bool _get(MAC& mac, std::list<MACLink*>::iterator& it);
};

MACLinkListener* listener();

#endif