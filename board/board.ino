#include "Controller.h"
#include "util/FileSystem.h"
#include "util/pvar.h"
//#include "hardware/connection.h"
#include "hardware/wifi/MACLinkListener.h"
#include <VescUart.h>

MACLink controller_link;

//MessageLink* msglink;

VescUart vesc;

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

void setup() {
  controller.init();
  filesys.init();
  pinMode(27, INPUT_PULLUP);
  ledcAttachPin(26, 0);
  ledcAttachPin(25, 1);
  ledcAttachPin(33, 2);
  ledcSetup(0, 1000, 8);
  //filesys.logMap(filesys.map("/"));
  //filesys.clearFS();

  listener()->init("Board");

  if (wifi_pvar.load()) {
    if (wifi_pvar.data.paired) {
      controller_link = MACLink(wifi_pvar.data.mac, 1500);
      listener()->add(&controller_link);
    }
  } else {
    listener()->start_pairing();
    wifi_pvar.data.paired = false;
  }
  
  //listener()->add(&controller_link);

  Serial2.begin(115200, SERIAL_8N1, 13, 15);
  vesc.setSerialPort(&Serial2);
  //vesc.setDebugPort(&Serial);
}

Rate main_rate(100);
Rate send(5);
Rate debug(0.5);
Rate blink1(1);
Rate blink2(1);
Rate blink3(1);

float control_val = 0;
float speed = 0;

struct SpeedMode {
  float max_accel = 0.001f;
  float max_break = 0.002f;
  float max_speed = 0.2f;
  float max_reverse = -0.05f;
  float max_coast = 0.0005f;
} speed_mode;

void update_speed(float control_val) {
  if (control_val >= 0) {
    float accel;
    control_val = (control_val - 0.5f) * 2;
    if (control_val > 0.1f) {
      if (speed > 0)
        accel = speed_mode.max_accel;
      else
        accel = speed_mode.max_break;
      control_val -= 0.1f;
    } else if (control_val < -0.1f) {
      if (speed > 0)
        accel = speed_mode.max_break;
      else
        accel = speed_mode.max_accel;
      control_val += 0.1f;
    } else
      accel = 0;
    
    if (!((speed > speed_mode.max_speed && control_val > 0) || (speed < speed_mode.max_reverse && control_val < 0))) {
      control_val /= 0.9f;
      speed += accel * control_val;
    }
  } else {
    if (speed > 0) {
      speed -= speed_mode.max_coast;
      if (speed < 0)
        speed = 0;
    } else if (speed < 0) {
      speed += speed_mode.max_coast;
      if (speed > 0)
        speed = 0;
    }
  }
}

void loop() {
  if (main_rate.isReady()) {
    if (debug.isReady())
      controller.printStats();

    if (listener()->get_pairing_mac(wifi_pvar.data.mac)) {
      wifi_pvar.data.paired = true;
      controller_link = MACLink(wifi_pvar.data.mac, 1500);
      listener()->add(&controller_link);
      listener()->stop_pairing();
    }

    if (controller_link.is_connected()) {
      recv_msg_t recv_msg;
      while (controller_link.get(recv_msg)) {
        //Serial.println("?");
        if ((int) recv_msg.type == 10)
          control_val = *((float*) recv_msg.data);
        else if ((int) recv_msg.type == 9)
          speed_mode = *((SpeedMode*) recv_msg.data);
        controller_link.dispose(recv_msg);
      }

      if (send.isReady()) {
        VescValues values = {
          .batteryVoltage = vesc.data.inpVoltage,
          .rpm = vesc.data.rpm,
          .tachometerAbs = vesc.data.tachometerAbs,
          .tempMosfet = vesc.data.tempMosfet
        };
        controller_link.send(11, (uint8_t*) &values, sizeof(values), 0);
      }

      blink1.enabled = vesc.getVescValues();

      //vesc.nunchuck.valueY = (int) (speed * 255 / 1000.f);
      
      //vesc.setNunchuckValues();
      //Serial.println(vesc.nunchuck.valueY);
      //vesc.printVescValues();

      blink2.enabled = true;
    } else {
      control_val = -1;
      blink2.enabled = false;
    }

    update_speed(control_val);

    vesc.setDuty(speed, 55);
    vesc.setDuty(speed, 56);
  }

  ledcWrite(0, 128 + (int) (-128 * blink1.getStageCos()));
  ledcWrite(1, 128 + (int) (-128 * blink2.getStageCos()));
  ledcWrite(2, 128 + (int) (-128 * blink3.getStageCos()));

  controller.update(true);
  listener()->update();
}
