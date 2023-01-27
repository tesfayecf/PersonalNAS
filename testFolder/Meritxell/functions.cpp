#include "functions.h"
#include <Dictionary.h>
#include <SD.h>
#include <SPI.h>

String getExtension(String fileName) {
  return fileName.substring(fileName.indexOf('.'));
}


String listDir(File root) {
  Dictionary &dirList = *(new Dictionary());
  int i = 0; 
  String type;
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    Dictionary &data = *(new Dictionary());
    data("name",entry.name());
    if (entry.isDirectory()) {
      data("type","folder");
      data("extension","folder");
      data("size","NaN");
    } else {
      data("type","file");
      data("extension",getExtension(entry.name()));
      data("size",String(entry.size()));
    }
    dirList(String(i),data.json());
    i++;
    entry.close();
    while ( data.count() ) data.remove(data(0)); 
  }
  dirList.remove(dirList(0));
  return dirList.json();
}
