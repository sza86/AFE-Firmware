/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_PIR_Structure_h
#define _AFE_PIR_Structure_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Relay states */
#define PIR_OPEN 1
#define PIR_CLOSE 0

struct PIR {
  uint8_t gpio;
  char name[16];
  boolean state;
  char mqttTopic[49];
  uint8_t ledId;
  uint8_t relayId;
  uint16_t howLongKeepRelayOn;
  boolean invertRelayState;
  unsigned long idx;
  boolean NOorNC;
};

#endif
