/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_FIRMWARE_Structure_h
#define _AFE_FIRMWARE_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

struct FIRMWARE {
  char version[7];
  uint8_t type;
  uint8_t autoUpgrade;
  char upgradeURL[120];
};

#endif
