
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <SdFat.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "wifiFunctions.h"
#include "functions.h"
#include "base64.h"
#include "variables.h"

AsyncWebServer server(80);
SdExFat NAS;
#define PIN_CS D8
#define SD_SPI_SPEED 30

//https://www.youtube.com/watch?v=TmV6JAUtrvw&t=365s
//https://github.com/G6EJD/G6EJD-ESP-File-Server
int USER_NUM;

void setup() {
  Serial.begin(9600);
  Serial.println();

  Serial.print("Initialization of LittleFS...");
  if(!LittleFS.begin()){
    Serial.println("failed!");
    return;
  }
  Serial.println("done.");

  Serial.print("Initialization of SD card...");
  if (!NAS.begin(PIN_CS, SD_SCK_MHZ(SD_SPI_SPEED))) {
    Serial.println("failed!");
    return;
  }
  Serial.println("done.");
 
  wifiConnect();
  Serial.println("Connected to wifi");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response;
    if (request->authenticate(__adim_usr__, __admin_pswd__)) {
      USER_NUM = 0;
    } else if (request->authenticate(__user1_usr__, __user1_pswd__)) {
      USER_NUM = 1;
    } else if (request->authenticate(__user2_usr__, __user2_pswd__)) {
      USER_NUM = 2;
    } else if (request->authenticate(__user3_usr__, __user3_pswd__)) {
      USER_NUM = 3;
    } else if (request->authenticate(__user4_usr__, __user4_pswd__)) {
      USER_NUM = 4;
    } else {
      return request->requestAuthentication();
    }
//    response = request->beginResponse(200, "text/html", webpage);
      response = request->beginResponse(LittleFS, "/main.html", "text/html");
      response->addHeader("Access-Control-Allow-Origin", "*");
      request->send(response);
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request) {
    //AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
    //response->addHeader("Content-Encoding", "gzip");
    //request->send_P(200, "image/x-icon", favicon_ico, favicon_ico_len);
    request->send(LittleFS, "/icon.ico", "image/x-icon");
  });

 server.on("/dirList", HTTP_GET, [](AsyncWebServerRequest * request) {
   AsyncWebServerResponse *response;
   if (request->hasParam("folder")) {
     AsyncWebParameter* p = request->getParam("folder");
     char b64_response[20000];
     char char_listOfDirs[20000];
     String listOfDirs = listDir(&NAS, p->value().c_str());
     listOfDirs.toCharArray(char_listOfDirs, 25000);
     b64_encode( b64_response, char_listOfDirs, listOfDirs.length()); // add hmac sha256 encryption
     response = request->beginResponse(200, "application/json", char_listOfDirs);
     Serial.printf("GET[%s]: %s --> %s\n", p->name().c_str(), p->value().c_str(), char_listOfDirs);
   } else {
     response = request->beginResponse(404, "text", "Parameter not available");
   }
   response->addHeader("Access-Control-Allow-Origin", "*");
   request->send(response);
 });

  
server.on("/file", HTTP_GET, [](AsyncWebServerRequest * request) {
  String file;
  AsyncWebServerResponse *response;
  if (request->hasParam("fileName")) {
    AsyncWebParameter* p = request->getParam("fileName");
    if (p->value().c_str() != "") {
      file = readFile(&NAS, p->value());
      response = request->beginResponse(200, "application/json", file);
      Serial.println(file);
      //response = request->beginResponse(NAS, "/users/Martí/File43.txt", "text/plain");
      //print(file);
      //Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
    } else {
      response = request->beginResponse(404, "text", "Parameter not aviable");
    }
  } else {
    response = request->beginResponse(404, "text", "Parameter not aviable");
  }
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
});
  
  /*
  return file --> Respond with content coming from a Stream
  */

  /*
String getContentType(String filename){
  if(filename.endsWith(F(".htm")))          return F("text/html");
  else if(filename.endsWith(F(".html")))    return F("text/html");
  else if(filename.endsWith(F(".css")))     return F("text/css");
  else if(filename.endsWith(F(".js")))      return F("application/javascript");
  else if(filename.endsWith(F(".json")))    return F("application/json");
  else if(filename.endsWith(F(".png")))     return F("image/png");
  else if(filename.endsWith(F(".gif")))     return F("image/gif");
  else if(filename.endsWith(F(".jpg")))     return F("image/jpeg");
  else if(filename.endsWith(F(".jpeg")))    return F("image/jpeg");
  else if(filename.endsWith(F(".ico")))     return F("image/x-icon");
  else if(filename.endsWith(F(".xml")))     return F("text/xml");
  else if(filename.endsWith(F(".pdf")))     return F("application/x-pdf");
  else if(filename.endsWith(F(".zip")))     return F("application/x-zip");
  else if(filename.endsWith(F(".gz")))      return F("application/x-gzip");
  return F("text/plain");
}

*/
  
  server.begin();
  Serial.println("Server Started");
}


void loop() {

}
