/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Data_Access_h
#define _AFE_Data_Access_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Structures.h>
#include <AFE-EEPROM.h>
// #include <Streaming.h>

class AFEDataAccess {
private:
  AFEEEPROM Eeprom;

public:
  AFEDataAccess();

  /* Methods read configuration from EEPROM */
  DEVICE getDeviceConfiguration();
  FIRMWARE getFirmwareConfiguration();
  NETWORK getNetworkConfiguration();
  MQTT getMQTTConfiguration();
  DOMOTICZ getDomoticzConfiguration();
  LED getLEDConfiguration(uint8_t id);
  RELAY getRelayConfiguration(uint8_t id);
  SWITCH getSwitchConfiguration(uint8_t id);
  PIR getPIRConfiguration(uint8_t id);

  /* Methods save configuration to EEPROM */
  void saveConfiguration(DEVICE configuration);
  void saveConfiguration(FIRMWARE configuration);
  void saveConfiguration(NETWORK configuration);
  void saveConfiguration(MQTT configuration);
  void saveConfiguration(DOMOTICZ configuration);
  void saveConfiguration(uint8_t id, LED configuration);
  void saveConfiguration(uint8_t id, RELAY configuration);
  void saveConfiguration(uint8_t id, SWITCH configuration);
  void saveConfiguration(uint8_t id, PIR configuration);

  /* Methods ave firmware version from/to EEPROM */
  void saveVersion(String version);

  /* Methods read and save relay state from/to EEPROM */
  boolean getRelayState(uint8_t id);
  void saveRelayState(uint8_t id, boolean state);

  /* Methods read and save device mode from/to EEPROM */
  uint8_t getDeviceMode();
  void saveDeviceMode(uint8_t mode);

  /* Methods read and save firmware Language from/to EEPROM */
  uint8_t getLanguage();
  void saveLanguage(uint8_t language);

  /* Methods read and save ID of system led */
  uint8_t getSystemLedID();
  void saveSystemLedID(uint8_t id);

  /* Methods saves and reads device ID */
  const String getDeviceID();
  void saveDeviceID(String id);

  /* Methods turns on / off APIs */
  void saveAPI(uint8_t apiID, boolean state);
};
#endif
