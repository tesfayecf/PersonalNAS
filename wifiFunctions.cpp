#include "wifiFunctions.h"
#include <ESP8266WiFi.h>
#include "variables.h"

void wifiConnect(const char* ssid, const char* password) {

  IPAddress local_IP(192, 168, 1, 101);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
    //eventToLog(WIFI_CONNECT_ERROR,local_IP.toString(),"STA Failed to configure");
  }
  
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(500);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  int r=0;
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    r++;
    if (r==150) break;
  }
  if (r==150) {
    //eventToLog(WIFI_CONNECT_ERROR,local_IP.toString(),"Unable to connect to wifi");
  }
}
