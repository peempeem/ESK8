#include "Controller.h"
#include "util/FileSystem.h"
#include "util/pvar.h"
#include "graphics/gui/gui.h"
#include "graphics/screens/welcomeScreen.h"
#include "graphics/screens/connectScreen.h"
#include "graphics/screens/homeScreen.h"
#include "graphics/icons/RGBIcon.h"


GUI* gui;
WelcomeScreen* welcomeScreen;
ConnectScreen* connectScreen;
HomeScreen* homeScreen;
RGBIcon* rgbIcon;

MACLink board_link;

struct WiFiVar {
    bool paired = false;
    MAC mac;
};

struct VescValues {
  float batteryVoltage;
  float rpm;
  long  tachometerAbs;
  float tempMosfet;
};

PVar<WiFiVar> wifi_pvar = PVar<WiFiVar>("/wifi.var");

void update_controller(void* data) {
  while (true)
    controller.update(true);
}

void setup() {
  controller.init();
  filesys.init();
  //filesys.clearFS();
  //filesys.logMap(filesys.map("/"));

  listener()->init("Controller");

  //filesys.deleteFile("/wifi.var");
  
  if (wifi_pvar.load()) {
    if (wifi_pvar.data.paired) {
      board_link = MACLink(wifi_pvar.data.mac);
      listener()->add(&board_link);
    }
  }

  gui = new GUI();
  welcomeScreen = new WelcomeScreen();
  connectScreen = new ConnectScreen();
  homeScreen = new HomeScreen();
  //rgbIcon = new RGBIcon();
  gui->addScreen(welcomeScreen);
  gui->addScreen(connectScreen);
  gui->addScreen(homeScreen);
  gui->setMainScreen(welcomeScreen);

  //gui->notifications.addIcon(rgbIcon);
  //rgbIcon->startCycling();
  //rgbIcon->cycleRate.setRate(0.33f);

  connectScreen->wifi.cycleRate.setRate(0.2f);
  connectScreen->wifi.linearCycle = false;
  connectScreen->setVisability(VISABLE);

  xTaskCreatePinnedToCore(update_controller, "controller_task", 2048, NULL, 2, NULL, 0);
}

void handle_power() {
  if (controller.powerStateChanged()) {
    switch (controller.powerState()) {
      case STATE_DISCHARGING:
        //gui->display_on();
        gui->refreshRate.setRate(60);
        break;
      case STATE_CHARGING:
        //gui->display_off();
        gui->refreshRate.setRate(40);
        break;
      case STATE_POWER_SAVING:
        //gui->display_on();
        gui->refreshRate.setRate(30);
        break;
    }
  }

  if (controller.batteryLevel() >= 0.8f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery.full);
    if (controller.powerState() == STATE_CHARGING)
      gui->notifications.battery.startBlinking();
    else
      gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.65f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._75);
    gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.35f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._50);
    gui->notifications.battery.stopBlinking();
  } else if (controller.batteryLevel() >= 0.2f) {
    gui->notifications.battery.showIcon(&gui->notifications.battery._25);
    gui->notifications.battery.stopBlinking();
  } else {
    gui->notifications.battery.showIcon(&gui->notifications.battery.empty);
    gui->notifications.battery.startBlinking();
  }
}

Rate debug(0.1);
Rate rate(100);
Timer dc_timer;
Timer home_timer;
Rate speed_rate(20);

float speed = 0;

struct SpeedMode {
  float max_accel;
  float max_break;
  float max_speed;
  float max_reverse;
  float max_coast;
};

SpeedMode speed_modes[] = {
  {
    .max_accel = 0.001f,
    .max_break = 0.002f,
    .max_speed = 0.2f,
    .max_reverse = -0.05f,
    .max_coast = 0.0009f
  },
  {
    .max_accel = 0.0015f,
    .max_break = 0.0025f,
    .max_speed = 0.5f,
    .max_reverse = -0.07f,
    .max_coast = 0.0012f
  },
  {
    .max_accel = 0.0025f,
    .max_break = 0.004f,
    .max_speed = 0.8f,
    .max_reverse = -0.1f,
    .max_coast = 0.0017f
  },
  {
    .max_accel = 0.003f,
    .max_break = 0.0035f,
    .max_speed = 1.0f,
    .max_reverse = -0.15f,
    .max_coast = 0.002f
  }
};

int speed_mode = 0;
int max_modes = 4;

void loop() {
  if (rate.isReady()) {
    handle_power();

    Screen* change = gui->screenChanged();
    if (change == welcomeScreen) {

    } else if (change == connectScreen) {
      home_timer.silence();

    } else if (change == homeScreen) {

    }

    if (gui->isMainScreen(welcomeScreen)) {
      if (millis() > 3000)
        gui->transitionTo(connectScreen, TRANSITION_SLIDE_UP);
      
    } else if (gui->isMainScreen(connectScreen)) {     
      // paired: wait for connection
      if (wifi_pvar.data.paired) {
        // connnected: go to main screen
        if (board_link.is_connected()) {
          connectScreen->wifi.showIcon(&connectScreen->wifi.wifiFull);
          connectScreen->wifi.stopCycling();
          connectScreen->wifi.startBlinking();
          connectScreen->text = "FOUND";
          home_timer.set(1000, true);
        }
        
        // not connected: wait for connection
        else {
          connectScreen->wifi.startCycling();
          connectScreen->wifi.stopBlinking();
          connectScreen->text = "PAIRED";
        }
      }

      // not paired: scan and pair
      else {
        connectScreen->wifi.showIcon(&connectScreen->wifi.pairing);
        connectScreen->wifi.stopCycling();
        connectScreen->wifi.startBlinking();
        connectScreen->text = "PAIRING";

        if (!listener()->is_scanning()) {
          if (listener()->get_scan_mac(wifi_pvar.data.mac)) {
            wifi_pvar.data.paired = true;
            wifi_pvar.write();
            board_link = MACLink(wifi_pvar.data.mac);
            listener()->add(&board_link);
          } else
            listener()->scan("Board");
        }
      }

      if (home_timer.is_ringing()) {
        gui->transitionTo(homeScreen, TRANSITION_SLIDE_RIGHT);
        board_link.send(9, (uint8_t*) &speed_modes[speed_mode], sizeof(SpeedMode), 4);
      }

    } else if (gui->isMainScreen(homeScreen)) {

      if (!board_link.is_connected()) {
        gui->transitionTo(connectScreen, TRANSITION_SLIDE_LEFT);
      } else {
          recv_msg_t recv_msg;
          while (board_link.get(recv_msg)) {
            if ((int) recv_msg.type == 11) {
              VescValues* values = (VescValues*) recv_msg.data;
              float battery = (values->batteryVoltage - 31) / 11.0f;
              float curr_speed = values->rpm * (16 / 36.0f) * 100 * 2 * 3.141592f * 0.0393701f * 0.000015782828283f * 60 / 14.0f;
              homeScreen->boardBattery.setBattery(battery);
              homeScreen->setSpeed(curr_speed);
              log(DEBUG, "TAC", values->tachometerAbs);
            }
            board_link.dispose(recv_msg);
          }

        if (controller.mainButton.releasedElapsedTime() < 250) {
          homeScreen->power.enable = true;
          homeScreen->power.setPower(controller.wheelValue());

          if (board_link.get_num_msgs() == 0) {
            float control_val = controller.wheelValue();
            board_link.send(10, (uint8_t*) &control_val, sizeof(float), 0);
          }
        } else {
          if (board_link.get_num_msgs() == 0) {
            float control_val = -1;
            board_link.send(10, (uint8_t*) &control_val, sizeof(float), 0);
          }
          homeScreen->power.enable = false;
        }
        homeScreen->setNumOutbound(board_link.get_num_msgs());
      }

      int ltaps = controller.leftButton.getTapEvent().taps;
      if (ltaps > 0) {
        int mode = speed_mode - ltaps;
        if (mode < 0)
          mode = 0;
        speed_mode = mode;
        homeScreen->setSpeedMode(mode);
        
      }

      int rtaps = controller.rightButton.getTapEvent().taps;
      if (rtaps > 0) {
        int mode = speed_mode + rtaps;
        if (mode >= max_modes)
          mode = max_modes - 1;
        speed_mode = mode;
        homeScreen->setSpeedMode(mode);
      }

      if (ltaps > 0 || rtaps > 0)
        board_link.send(9, (uint8_t*) &speed_modes[speed_mode], sizeof(SpeedMode), 4);
      //homeScreen->boardBattery.setBattery(controller.batteryLevel());
      //homeScreen->setSpeed(speed);
    }
        
    int rssi = board_link.get_rssi();
    if (rssi > 15)
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifiFull);
    else if (rssi > 5)
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi3);
    else if (rssi > 0)
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi2);
    else if (rssi > -100)
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifi1);
    else
      gui->notifications.wifi.showIcon(&gui->notifications.wifi.wifiNone);

    controller.consumeTaps();
  }

  if (debug.isReady())
      controller.printStats();

  gui->update();
  listener()->update();
}
