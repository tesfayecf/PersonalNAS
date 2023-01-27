#include "functions.h"
#include "variables.h"
#include <Dictionary.h>
#include <SPI.h>
#include "SdFat.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

char currentDirectory[100] = "/users/";
char baseDir[8] = "/users/";

String getExtension(String fileName) {
  return fileName.substring(fileName.indexOf('.') + 1);
}

String getUserMainDir() {
  return String(baseDir);
}

String rewindDirectory(String Sdir) {
  String midDir = Sdir.substring(0, Sdir.lastIndexOf('/')); // delete last '/'
  int last = midDir.lastIndexOf('/') + 1; // get position of new last '/'
  String dirRet = midDir.substring(0, last); // delete from new last '/'
  //char dirChar[100] = dirRet.c_str();
  return dirRet;
}

String listDir(SdExFat* NAS, String name) {
  int i = 1;
  String type;
  String response;
  ExFatFile root;
  
  String prov = String(currentDirectory) + name + "/";
  char provC[100];
  prov.toCharArray(provC, prov.length()+1);
  
  if (name == __main_folder__) {
    String provMain = getUserMainDir(/*algo per identificar l'usuari*/);
    provMain.toCharArray(currentDirectory, provMain.length()+1);
    
  } else if (name == __rewind_dir__) {
    if (strcmp(currentDirectory, baseDir) != 0) {
      String provRew = rewindDirectory(String(currentDirectory));
      provRew.toCharArray(currentDirectory, provRew.length()+1);
    } else {
      strcpy(currentDirectory, baseDir);
    }
    
  } else if  (NAS->exists(provC)) {
    strcpy(currentDirectory, provC);
  
  } else {
    response = __wrong_file__;
    return response;
  }
  
  root.open(currentDirectory);
  
  response += "{";
  ExFatFile entry;
  char entryName[100];
  Dictionary &data = *(new Dictionary());
  while (entry.openNext(&root)) {
    if (!entry) {
      Serial.println("EOF");
      break;
    } 
    entry.getName(entryName,100);

    /*
     * getAccesDateTime()
     * getCreateDateTime()
     */
    
    data("name", entryName);
    if (entry.isDir()) {
      data("type", "folder");
      data("extension", "folder");
      data("size", "NaN");
    } else {
      data("type", "file");
      data("extension", getExtension(String(entryName)));
      uint64_t fileSize = entry.fileSize();
      data("size", String(fileSize));
    }
    //Serial.println(data.json());
    if (i != 1) response += " ,";
    response += "\"" + String(i) + "\"" + ": [" + data.json() + "]";

    i++;
    entry.close();
    while ( data.count() ) data.remove(data(0));
  }
  response += "}";
  //Serial.print("listDir: ");
  //Serial.println(response);
  Serial.println(currentDirectory);
  return response;
}

String readFile(SdExFat* NAS, String fileName) {
  char dirC[100];
  String dir = currentDirectory  + fileName;
  dir.toCharArray(dirC, dir.length()+1);
  String text;
  ExFatFile file;
  if (NAS->exists(currentDirectory  + fileName)) {
    file.open(dirC);
    while (file.available()) {
      text += char(file.read());
    }
    file.close();
    return text;
  } else {
    return "FDNE";
  }
}

//String getCurrentDirectory() {}
