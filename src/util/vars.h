#ifndef VARS_H
#define VARS_H

#include <cstdint>

struct WiFiVar {
  bool gotMac = false;
  uint8_t mac[6];
};
 

#endif