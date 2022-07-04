
#include "Controller.h"
#include "util/FileSystem.h"
//#include "hardware/connection.h"
#include "graphics/gui/gui.h"
#include "graphics/screens/welcomeScreen.h"
#include "graphics/screens/connectScreen.h"
#include "graphics/screens/homeScreen.h"
#include "graphics/icons/RGBIcon.h"
#include "hardware/espnow.h"

//MessageLink* msglink;

GUI* gui;
WelcomeScreen* welcomeScreen;
ConnectScreen* connectScreen;
HomeScreen* homeScreen;
RGBIcon* rgbIcon;

const static uint8_t b_mac[6] = {0x08, 0x3a, 0xf2, 0x69, 0xd7, 0x58};
MAC board_mac(b_mac);

void update_controller(void* data) {
  while (true)
    controller.update(true);
}

void setup() {
  controller.init();
  filesys.init();
  //filesys.clearFS();
  //filesys.logMap(filesys.map("/"));

  //msglink = new MessageLink();
  //msglink->init("Controller ", "password");

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  WiFi.softAP("CONTROLLER", "password", 1, 0);

  espnow_init();
  
  esp_now_peer_info_t peer;
  memset(&peer, 0, sizeof(peer));
  memcpy(peer.peer_addr, b_mac, 6);
  add_peer(&peer);
  
  get_whitelist()->add(board_mac);

  gui = new GUI();
  welcomeScreen = new WelcomeScreen();
  connectScreen = new ConnectScreen();
  homeScreen = new HomeScreen();
  rgbIcon = new RGBIcon();
  gui->addScreen(welcomeScreen);
  gui->addScreen(connectScreen);
  gui->addScreen(homeScreen);
  gui->setMainScreen(welcomeScreen);

  gui->notifications.addIcon(rgbIcon);
  rgbIcon->startCycling();
  rgbIcon->cycleRate.setRate(0.8f);

  connectScreen->wifi.cycleRate.setRate(0.2f);
  connectScreen->wifi.linearCycle = false;
  connectScreen->setVisability(VISABLE);

  xTaskCreatePinnedToCore(update_controller, "controller_task", 2048, NULL, 2, NULL, 0);
}

void handle_power() {
  if (controller.powerStateChanged()) {
    switch (controller.powerState()) {
      case STATE_DISCHARGING:
        gui->refreshRate.setRate(60);
        break;
      case STATE_CHARGING:
        gui->refreshRate.setRate(40);
        break;
      case STATE_POWER_SAVING:
        gui->refreshRate.setRate(30);
        break;
    }
  }

  if (controller.batteryLevel() >= 0.75f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery.full);
    if (controller.powerState() == STATE_CHARGING)
      gui->notifications.battery.startBlinking();
    else
      gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.5f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._75);
    gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.25f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._50);
    gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.10f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._25);
    gui->notifications.battery.stopBlinking();
  } else {
    gui->notifications.battery.showIcon(&gui->notifications.battery.empty);
    gui->notifications.battery.startBlinking();
  }
}

Rate debug(0.1f);
Rate speedRate(10);
Rate rate(100);
Rate pair(0.1f);
float speed = 0.0f;
Timer dc_timer;
Timer home_timer;
Rate send(0.2f);
Rate send2(30.0f);

void loop() {
  if (rate.isReady()) {
    handle_power();

    Screen* change = gui->screenChanged();
    if (change == welcomeScreen) {
      // Entry into welcomeScreen

    } else if (change == connectScreen) {
      home_timer.silence();

    } else if (change == homeScreen) {

    }

    if (gui->isMainScreen(welcomeScreen)) {
      if (millis() > 4000)
        gui->transitionTo(connectScreen, TRANSITION_SLIDE_UP);
      
    } else if (gui->isMainScreen(connectScreen)) {      
      //if (!msglink->is_paired()) {
      if (true) {
        connectScreen->wifi.showIcon(&connectScreen->wifi.pairing);
        connectScreen->wifi.stopCycling();
        connectScreen->wifi.startBlinking();
      } //else if (!msglink->is_connected()) {
      else if (true) {
        connectScreen->wifi.startCycling();
        connectScreen->wifi.stopBlinking();
      } else {
        connectScreen->wifi.showIcon(&connectScreen->wifi.wifiFull);
        connectScreen->wifi.stopCycling();
        connectScreen->wifi.startBlinking();
      }

      //if (msglink->is_connected())
        //home_timer.set(2000, true);

      if (home_timer.is_ringing())
        gui->transitionTo(homeScreen, TRANSITION_SLIDE_RIGHT);

    } else if (gui->isMainScreen(homeScreen)) {

      if (controller.mainButton.hasTapEvent()) {
        TapEvent taps = controller.mainButton.getTapEvent();
        //if (taps.taps >= 4)
          //msglink->unpair();
          
      }

      //if (!msglink->is_connected())
        //gui->transitionTo(connectScreen, TRANSITION_SLIDE_LEFT);

      if (controller.mainButton.releasedElapsedTime() < 500) {
        if (speedRate.isReady()) { (speed += controller.wheelValue() - 0.5f) / 2.0f; }
        homeScreen->power.enable = true;
      } else
        homeScreen->power.enable = false;
      
      homeScreen->boardBattery.setBattery(controller.batteryLevel());
      homeScreen->power.setPower(controller.wheelValue());
      homeScreen->setSpeed(speed);
    }

    if (debug.isReady())
      controller.printStats();
    
    //msglink->update();
    
    /*if (!msglink->is_paired() && !msglink->is_pairing()) {
      if (msglink->start_pairing("Board ")) {

      }
    }*/

    recv_msg_t recv_msg;
    if (get_inbound_message(&recv_msg)) {

      dispose_msg(recv_msg);
    }

    
    if (send.isReady()) {
      uint8_t buf[4000];
      for (int i = 0; i < 4000; i++)
        buf[i] = i;
      msg_t msg = {
        .mac = board_mac,
        .type = 0,
        .data = buf,
        .len = 4000,
        .priority = 0,
        .retries = 4
      };
      uint8_t buf2[300];
      for (int i = 0; i < 300; i++)
        buf2[i] = i;
      msg_t msg2 = {
        .mac = board_mac,
        .type = 1,
        .data = buf2,
        .len = 300,
        .priority = 0,
        .retries = 4
      };
      send_msg(msg);
      send_msg(msg2);
    }

    

    //rate.sleep();
    
    int rssi = get_whitelist()->get_rssi(board_mac);

    if (rssi > 15) {
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifiFull);
    } else if (rssi > 5) {
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi3);
    } else if (rssi > 0) {
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi2);
    } else if (rssi > -100) {
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi1);
    } else {
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifiNone);
    }

    
  }
  if (send2.isReady()) {
    msg_t msg = {
      .mac = board_mac,
      .type = 2,
      .data = NULL,
      .len = 0,
      .priority = 0,
      .retries = 0
    };
    send_msg(msg);
  }

  gui->update();
  sender_update();
}
