/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_DS18B20_Structure_h
#define _AFE_DS18B20_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define UNIT_CELCIUS 0
#define UNIT_FAHRENHEIT 1

struct DS18B20 {
  uint8_t gpio;
  float correction;
  uint16_t interval;
  uint8_t unit;
  boolean sendOnlyChanges;
  unsigned long idx;
};

#endif
