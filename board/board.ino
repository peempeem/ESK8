#include "Controller.h"
#include "util/FileSystem.h"
#include "util/pvar.h"
#include "util/vars.h"

ESK8FS filesys;
PVar<WiFiVar> wifiVar("/wifi.var", &filesys);

void setup() {
  Serial.begin(115200);
  delay(2000);
  filesys.init();
  filesys.logMap(filesys.map("/"));
  //filesys.clearFS();
  if (wifiVar.load()) {
    Serial.println(wifiVar.data.gotMac);
  } else {
    wifiVar.data.gotMac = true;
    wifiVar.write();
  }
  
}

void loop() {
  Serial.println("im ok!");
  delay(5000);
}
