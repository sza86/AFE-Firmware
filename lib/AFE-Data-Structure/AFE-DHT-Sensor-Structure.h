/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_DHT_Structure_h
#define _AFE_DHT_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-HUMIDITY-Structure.h>
#include <AFE-TEMPERATURE-Structure.h>

struct DH {
  uint8_t gpio;
  uint8_t type;
  TEMPERATURE temperature;
  HUMIDITY humidity;
  boolean sendOnlyChanges;
  boolean publishHeatIndex;
  unsigned long temperatureIdx;
  unsigned long humidityIdx;
  unsigned long temperatureAndHumidityIdx;
};

#endif
