/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <AFE-Device.h>
#include <AFE-EEPROM.h>
#include <ESP8266WiFi.h>
//#include <Streaming.h>

class AFESitesGenerator {

private:
  AFEEEPROM Eeprom;
  AFEDataAccess Data;
  AFEDevice Device;
  uint8_t language;

  /* Method generates GPIO selecton list */
  const String generateConfigParameter_GPIO(const char *field,
                                            uint8_t selected);
  /* These three methods generates checkboxes for Switch, Relay and LED */
  const String generateSwitchItem(uint8_t id, boolean checked);
  const String generateRelayItem(boolean checked);
  const String generateLEDItem(boolean checked);
  const String generateTwoValueController(REGULATOR configuration,
                                          boolean thermostat);

  /* Method addes configuration block to the site */
  String addConfigurationBlock(const String title, const String description,
                               const String body);

  const String generateHardwareItemsList(uint8_t noOfItems,
                                         uint8_t noOffConnected,
                                         const char *field, const char *label);

  const String generateTwoValueController(REGULATOR configuration);

public:
  /* Constructor*/
  AFESitesGenerator();

  /* Method generates site header with menu. When redirect param is diff than 0
    then it will redirect page to main page after redirect param time (in sec)
   */
  const String generateHeader(uint8_t redirect = 0);

  /* Method generates site footer */
  const char *generateFooter();

  /* All following methods generates configuration sections */
  String addLanguageConfiguration();
  String addDeviceConfiguration();
  String addNetworkConfiguration();
  String addMQTTBrokerConfiguration();
  String addDomoticzServerConfiguration();
  String addLEDConfiguration(uint8_t id);
  String addSystemLEDConfiguration();
  String addRelayConfiguration(uint8_t id);
  String addSwitchConfiguration(uint8_t id);
  String addDS18B20Configuration();
  String addThermostatConfiguration();

  /* These methods generates firmware upgrade sections */
  String addUpgradeSection();
  String addPostUpgradeSection(boolean status);

  /* Method generate restore to defaults section. Command = 0 is pre-reset site,
   * 1 is a post reset site */
  String addResetSection(uint8_t command);

  /* Method addes info that device is being reset */
  String addExitSection();

  /* Method generates section shown when device is in norma mode */
  String addHelpSection();
};

#endif
