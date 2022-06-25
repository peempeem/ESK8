#include "Controller.h"
#include "util/FileSystem.h"
#include "hardware/connection.h"


MessageLink* msglink;

void setup() {
  Serial.begin(115200);
  filesys.init();
  //filesys.logMap(filesys.map("/"));
  //filesys.clearFS();

  msglink = new MessageLink();
  msglink->init(false);
  
}

void loop() {
  
  if (!msglink->is_paired() && !msglink->is_pairing())
      msglink->pair_slave();
  else if (msglink->is_paired())
    log(DEBUG, "loop", "PAIRED!!");
  
}
