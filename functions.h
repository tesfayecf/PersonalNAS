#ifndef functions 
#define functions

#include "variables.h"
#include <SdFat.h>

#include <SPI.h>

#ifdef ARDUINO_ARCH_MEGAAVR
#endif

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

String getExtension(String fileName);

String getUserMainDir();

String rewindDirectory(String dir);

String listDir(SdExFat* NAS, String name);

String readFile(SdExFat* NAS, String fileName);

#endif
