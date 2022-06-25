
#include "Controller.h"
#include "util/FileSystem.h"
#include "hardware/connection.h"
#include "graphics/gui/gui.h"
#include "graphics/screens/welcomeScreen.h"
#include "graphics/screens/connectScreen.h"
#include "graphics/screens/homeScreen.h"
#include "graphics/icons/RGBIcon.h"

MessageLink* msglink;

GUI* gui;
WelcomeScreen* welcomeScreen;
ConnectScreen* connectScreen;
HomeScreen* homeScreen;
RGBIcon* rgbIcon;

void update_gui(void* data) {
  while (true)
    gui->update(true);   
}

void update_controller(void* data) {
  while (true)
    controller.update(true);
}

void setup() {
  controller.init();

  filesys.init();
  //filesys.clearFS();
  //filesys.logMap(filesys.map("/"));

  msglink = new MessageLink();
  msglink->init(true);

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

  xTaskCreatePinnedToCore(update_gui, "gui_task", 4096, NULL, 1, NULL, 1);
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

Rate debug(0.5f);
Rate speedRate(10);
Rate rate(60);
Rate pair(0.1f);
float speed = 0.0f;
Timer dc_timer;
Timer home_timer;

void loop() {
  if (rate.isReady()) {
    handle_power();

    Screen* change = gui->screenChanged();
    if (change == welcomeScreen) {
      // Entry into welcomeScreen

    } else if (change == connectScreen) {
      // Entry into connectScreen

    } else if (change == homeScreen) {
      // Entry into homeScreen
    }

    if (gui->isMainScreen(welcomeScreen)) {
      if (millis() > 3000)
        gui->transitionTo(connectScreen, TRANSITION_SLIDE_UP);
      
    } else if (gui->isMainScreen(connectScreen)) {      
      if (!msglink->is_paired()) {
        connectScreen->wifi.showIcon(&connectScreen->wifi.pairing);
        connectScreen->wifi.stopCycling();
        connectScreen->wifi.startBlinking();
      } else if (!msglink->is_connected()) {
        connectScreen->wifi.startCycling();
        connectScreen->wifi.stopBlinking();
      } else {
        connectScreen->wifi.showIcon(&connectScreen->wifi.wifiFull);
        connectScreen->wifi.stopCycling();
        connectScreen->wifi.startBlinking();
      }

      if (controller.rightButton.isTapped())
        home_timer.set(2000);

      if (home_timer.is_ringing())
        gui->transitionTo(homeScreen, TRANSITION_SLIDE_RIGHT);

    } else if (gui->isMainScreen(homeScreen)) {
      if (controller.rightButton.isTapped()) {
        connectScreen->wifi.showIcon(&connectScreen->wifi.wifiNone);
        connectScreen->wifi.startBlinking();
        dc_timer.set(2000);
        gui->transitionTo(connectScreen, TRANSITION_SLIDE_LEFT);
      }

      if (controller.mainButton.releasedElapsedTime() < 500) {
        if (speedRate.isReady()) { (speed += controller.wheelValue() - 0.5f) / 2.0f; }
        homeScreen->power.enable = true;
      } else
        homeScreen->power.enable = false;
      
      homeScreen->boardBattery.setBattery(controller.batteryLevel());
      homeScreen->power.setPower(controller.wheelValue());
      homeScreen->setSpeed(speed);
    }

    //if (debug.isReady())
      //controller.printStats();
    
    msglink->update();
    
    if (!msglink->is_paired() && !msglink->is_pairing())
      msglink->pair_master();

    rate.sleep();
  }
}
