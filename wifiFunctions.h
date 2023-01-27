#ifndef wifiFunctions 
#define wifiFunctions
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "variables.h"

#ifdef ARDUINO_ARCH_MEGAAVR
#endif

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

void wifiConnect(const char* ssid = __ssid__, const char* password = __password__);

#endif
