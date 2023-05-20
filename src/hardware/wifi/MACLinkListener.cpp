#include "MACLinkListener.h"

const static char* LOG_HEADER = "MACLinkListener";

MACLinkListener mac_link_listener;

MACLinkListener* listener() {
    return &mac_link_listener;
}

void send_cb(const uint8_t* mac, esp_now_send_status_t status) {
    MAC receiver(mac);
    mac_link_listener._send_cb(receiver, status);
}

void recv_cb(const uint8_t* mac, const uint8_t* data, int len) {
    MAC sender(mac);
    if (!mac_link_listener._recv_cb(sender, data, len))
        mac_link_listener._add_pairing_mac(sender, data, len);
    listener()->update();
}

void random_ascii(char* buf, int len) {
    for (int i = 0; i < len; i++)
        buf[i] = random(65, 91);
}

MACLinkListener::MACLinkListener() {

}

bool MACLinkListener::init(const char* name) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);

    int len = strlen(name);
    char name_buf[len + 9];
    strncpy(name_buf, name, len);
    name_buf[len] = ' ';
    random_ascii(name_buf + len, 7);
    name_buf[len + 7] = 0;

    char pass_buf[16];
    random_ascii(pass_buf, 15);
    pass_buf[15] = 0;

    WiFi.softAP(name_buf, pass_buf, 1, 0);

    if (esp_now_init() != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to init esp now");
        return false;
    }
    
    if (esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to set esp wifi protocol");
        return false;
    }

    if (esp_now_register_send_cb(send_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register send callback");
        return false;
    }
    
    if (esp_now_register_recv_cb(recv_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register recv callback");
        return false;
    }

    initialied = true;

    return true;
}

void MACLinkListener::set_cpu_mhz_safe(int mhz) {
    if (!initialied) {
        setCpuFrequencyMhz(mhz);
        return;
    }

    scanning = false;
    
    //String ssid = WiFi.softAPSSID();
    
    esp_now_peer_num_t peer_num;
    if (esp_now_get_peer_num(&peer_num) != ESP_OK)
        peer_num.total_num = 0;
        
    esp_now_peer_info_t peers[peer_num.total_num];
    for (int i = 0; i < peer_num.total_num; i++) {
        esp_now_fetch_peer(true, &peers[i]);
        esp_now_del_peer(peers[i].peer_addr);
    }
    esp_now_deinit();
    esp_wifi_stop();

    setCpuFrequencyMhz(mhz);

    esp_wifi_start();

    /*char pass_buf[16];
    random_ascii(pass_buf, 15);
    pass_buf[15] = 0;*/

    //WiFi.mode(WIFI_AP_STA);
    //WiFi.softAP(ssid.c_str(), pass_buf, 1, 0);
    if (esp_now_init() != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to init esp now");
        return;
    }
    
    if (esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to set esp wifi protocol");
        return;
    }

    if (esp_now_register_send_cb(send_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register send callback");
        return;
    }
    
    if (esp_now_register_recv_cb(recv_cb) != ESP_OK) {
        log(ERROR, LOG_HEADER, "Failed to register recv callback");
        return;
    }

    //init(ssid.c_str());

    for (int i = 0; i < peer_num.total_num; i++)
        esp_now_add_peer(&peers[i]);
}

void MACLinkListener::add(MACLink* link) {
    if (!contains(link)) {
        esp_now_peer_info_t peer;
        memset(&peer, 0, sizeof(peer));
        MAC mac = link->get_mac();
        memcpy(&peer.peer_addr, mac.get(), MAC_SIZE);
        peer.channel = 1;
        log(DEBUG, LOG_HEADER, "Connecting to peer with MAC [", false);
        mac.print();
        logc("]", false);
        if (esp_now_add_peer(&peer) != ESP_OK) {
            logf();
            return;
        }
        logs();
        lock();
        connections.push_back(link);
        unlock();
    }
    
}

void MACLinkListener::remove(MACLink* link) {
    std::list<MACLink*>::iterator it;
    MAC mac = link->get_mac();
    if (_get(mac, it)) {
        esp_now_del_peer(mac.get());
        lock();
        connections.erase(it);
        unlock();
    }
}

bool MACLinkListener::contains(MACLink* link) {
    std::list<MACLink*>::iterator it;
    MAC mac = link->get_mac();
    return _get(mac, it);
}

void MACLinkListener::update() {
    for (std::list<MACLink*>::iterator it = connections.begin(); it != connections.end(); it++)
        (**it).update();
    
    int networks = WiFi.scanComplete();
    if (networks >= 0) {
        if (!scanning)
            WiFi.scanDelete();
        for (int i = 0; i < networks; i++) {
            if (WiFi.SSID(i).indexOf(key) == 0) {
                uint8_t mac[MAC_SIZE];
                memcpy(mac, WiFi.BSSID(i), MAC_SIZE);
                mac[MAC_SIZE - 1] &= 0xFE;
                scan_list.emplace_back(mac);
            }
        }
        WiFi.scanDelete();
        scanning = false;
    }
}

bool MACLinkListener::scan(const char* key) {
    if (scanning)
        return false;
    
    log(DEBUG, LOG_HEADER, "Beginning scan for networks with [", false);
    logc(key, false);
    logc("] ...");
    this->key = key;
    WiFi.scanNetworks(true);
    scan_list.clear();
    scanning = true;
    return true;
}

bool MACLinkListener::is_scanning() {
    return scanning;
}

bool MACLinkListener::get_scan_mac(MAC& mac) {
    if (scan_list.empty())
        return false;
    mac.set(scan_list.front());
    scan_list.pop_front();
    return true;
}

void MACLinkListener::start_pairing() {
    pairing = true;
}

void MACLinkListener::stop_pairing() {
    pairing = false;
    pairing_list.clear();
}

bool MACLinkListener::is_pairing() {
    return pairing;
}

bool MACLinkListener::get_pairing_mac(MAC& mac) {
    if (pairing_list.empty())
        return false;
    mac.set(pairing_list.front());
    pairing_list.pop_front();
    return true;
}

void MACLinkListener::_add_pairing_mac(MAC& mac, const uint8_t* data, int len) {
    if (!pairing)
        return;

    for (std::list<MAC>::iterator it = pairing_list.begin(); it != pairing_list.end(); it++) {
        if (it->is(mac))
            return;
    }
    
    pairing_list.push_back(mac);
}

bool MACLinkListener::_recv_cb(MAC& mac, const uint8_t* data, int len) {
    std::list<MACLink*>::iterator it;
    if (_get(mac, it)) {
        (**it)._recv_cb(data, len);
        return true;
    }
    return false;
}

void MACLinkListener::_send_cb(MAC& mac, esp_now_send_status_t status) {
    std::list<MACLink*>::iterator it;
    if (_get(mac, it))
        (**it)._send_cb(status);
}

bool MACLinkListener::_get(MAC& mac, std::list<MACLink*>::iterator& it) {
    lock();
    for (it = connections.begin(); it != connections.end(); it++) {
        if ((**it).is(mac)) {
            unlock();
            return true;
        }
    }
    unlock();
    return false;
}

