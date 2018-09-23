/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Common_Configuration_h
#define _AFE_Common_Configuration_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Parameter indicates that HTTP request requires data save */
#define SERVER_CMD_SAVE 1
#define SERVER_CMD_NONE 0

#endif
