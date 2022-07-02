#include "Controller.h"
#include "util/FileSystem.h"
//#include "hardware/connection.h"
#include "hardware/espnow.h"

const static uint8_t c_mac[6] = {0x94, 0xb9, 0x7e, 0xd2, 0x28, 0x84};
MAC controller_mac(c_mac);

//MessageLink* msglink;

void setup() {
  controller.init();
  filesys.init();
  //filesys.logMap(filesys.map("/"));
  //filesys.clearFS();

  //msglink = new MessageLink();
  //msglink->init("Board ", "password");

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  WiFi.softAP("BOARD LMAO", "password", 1, 0);

  espnow_init();
  
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(peer));
  memcpy(peer.peer_addr, c_mac, 6);
  add_peer(&peer);
  
  whitelist.add(controller_mac);
}

Rate main_rate(100);
Rate send(1);
Rate debug(0.1f);

void loop() {
  if (main_rate.isReady()) {
    if (debug.isReady())
      controller.printStats();
    
    recv_msg_t recv_msg;
    if (get_inbound_message(&recv_msg)) {
      if (recv_msg.type == 0) {
        bool passed = true;
        if (recv_msg.len == 4000) {        
          for (int i = 0; i < 4000; i++) {
            if (recv_msg.data[i] != (uint8_t) i) {
              passed = false;
              break;
            }
          }
        } else
          passed = false;
        
        if (passed)
          log(DEBUG, "test 0", "passed");
        else
          log(DEBUG, "test 0", "failed");
      } else if (recv_msg.type == 1) {
        bool passed = true;
        if (recv_msg.len == 300) {
          for (int i = 0; i < 300; i++) {
            if (recv_msg.data[i] != (uint8_t) i) {
              passed = false;
              break;
            }
          }
        } else
          passed = false;

        if (passed)
          log(DEBUG, "test 1", "passed");
        else
          log(DEBUG, "test 1", "failed");
      }
      dispose_msg(recv_msg);
    }
    
    main_rate.sleep();
  }
}
