/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-Data-Access.h"

AFEDataAccess::AFEDataAccess() {}

DEVICE AFEDataAccess::getDeviceConfiguration() {
  DEVICE configuration;
  Eeprom.read(9, 16).toCharArray(configuration.name,
                                 sizeof(configuration.name));
  configuration.isLED[0] = Eeprom.read(366);
  configuration.isLED[1] = Eeprom.read(418);
  configuration.isRelay[0] = Eeprom.read(369);
  configuration.isSwitch[0] = Eeprom.read(395);
  configuration.isSwitch[1] = Eeprom.read(402);
  configuration.httpAPI = Eeprom.read(25);
  configuration.mqttAPI = Eeprom.read(228);
  configuration.domoticzAPI = Eeprom.read(800);

  return configuration;
}

FIRMWARE AFEDataAccess::getFirmwareConfiguration() {
  FIRMWARE configuration;

  Eeprom.read(0, 7).toCharArray(configuration.version,
                                sizeof(configuration.version));
  Eeprom.read(28, 120).toCharArray(configuration.upgradeURL,
                                   sizeof(configuration.upgradeURL));
  configuration.type = Eeprom.readUInt8(7);
  configuration.autoUpgrade = Eeprom.readUInt8(27);
  return configuration;
}

NETWORK AFEDataAccess::getNetworkConfiguration() {
  NETWORK configuration;

  Eeprom.read(148, 32).toCharArray(configuration.ssid,
                                   sizeof(configuration.ssid));
  Eeprom.read(180, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));

  configuration.isDHCP = Eeprom.read(212);
  configuration.ip = Eeprom.readIP(213);
  configuration.gateway = Eeprom.readIP(217);
  configuration.subnet = Eeprom.readIP(221);
  configuration.noConnectionAttempts = Eeprom.readUInt8(225);
  configuration.waitTimeConnections = Eeprom.readUInt8(226);
  configuration.waitTimeSeries = Eeprom.readUInt8(227);
  return configuration;
}

MQTT AFEDataAccess::getMQTTConfiguration() {
  MQTT configuration;

  Eeprom.read(229, 32).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.ip = Eeprom.readIP(261);
  configuration.port = Eeprom.read(265, 5).toInt();
  Eeprom.read(270, 32).toCharArray(configuration.user,
                                   sizeof(configuration.user));
  Eeprom.read(302, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  Eeprom.read(334, 32).toCharArray(configuration.topic,
                                   sizeof(configuration.topic));

  return configuration;
}

DOMOTICZ AFEDataAccess::getDomoticzConfiguration() {
  DOMOTICZ configuration;

  configuration.protocol = Eeprom.readUInt8(801);
  Eeprom.read(802, 40).toCharArray(configuration.host,
                                   sizeof(configuration.host));
  configuration.port = Eeprom.read(842, 5).toInt();
  Eeprom.read(847, 32).toCharArray(configuration.user, 32);
  Eeprom.read(879, 32).toCharArray(configuration.password,
                                   sizeof(configuration.password));
  return configuration;
}

LED AFEDataAccess::getLEDConfiguration(uint8_t id) {
  LED configuration;
  uint8_t nextLED = 52;
  configuration.gpio = Eeprom.readUInt8(367 + id * nextLED);
  configuration.changeToOppositeValue = Eeprom.read(368 + id * nextLED);

  return configuration;
}

RELAY AFEDataAccess::getRelayConfiguration(uint8_t id) {
  RELAY configuration;
  MQTT configurationMQTT;
  uint8_t nextRelay = 26;
  configuration.gpio = Eeprom.readUInt8(370 + id * nextRelay);
  configuration.timeToOff = Eeprom.read(372 + id * nextRelay, 5).toFloat();
  configuration.statePowerOn = Eeprom.readUInt8(377 + id * nextRelay);

  Eeprom.read(378 + id * nextRelay, 16)
      .toCharArray(configuration.name, sizeof(configuration.name));

  configuration.stateMQTTConnected = Eeprom.readUInt8(394 + id * nextRelay);

  Eeprom.read(334, 32).toCharArray(configurationMQTT.topic,
                                   sizeof(configurationMQTT.topic));

  sprintf(configuration.mqttTopic, "%s%s/", configurationMQTT.topic,
          configuration.name);

  configuration.ledID = Eeprom.readUInt8(421 + id);

  configuration.idx = Eeprom.read(930 + id, 6).toInt();

  return configuration;
}

SWITCH AFEDataAccess::getSwitchConfiguration(uint8_t id) {
  SWITCH configuration;
  uint8_t nextSwitch = 7;
  configuration.gpio = Eeprom.readUInt8(396 + id * nextSwitch);
  configuration.type = Eeprom.readUInt8(397 + id * nextSwitch);
  configuration.sensitiveness = Eeprom.read(398 + id * nextSwitch, 3).toInt();
  configuration.functionality = Eeprom.readUInt8(401 + id * nextSwitch);
  configuration.relayID = Eeprom.readUInt8(416 + id);
  return configuration;
}

void AFEDataAccess::saveConfiguration(DEVICE configuration) {
  Eeprom.write(9, 16, configuration.name);
  Eeprom.write(369, configuration.isRelay[0]);
  Eeprom.write(395, configuration.isSwitch[0]);
  Eeprom.write(402, configuration.isSwitch[1]);
  Eeprom.write(366, configuration.isLED[0]);
  Eeprom.write(418, configuration.isLED[1]);
  saveAPI(API_MQTT, configuration.mqttAPI);
  saveAPI(API_HTTP, configuration.httpAPI);
  saveAPI(API_DOMOTICZ, configuration.domoticzAPI);
}

void AFEDataAccess::saveConfiguration(FIRMWARE configuration) {
  Eeprom.write(0, 7, configuration.version);
  Eeprom.writeUInt8(7, configuration.type);
  Eeprom.writeUInt8(27, configuration.autoUpgrade);
  Eeprom.write(28, 120, configuration.upgradeURL);
}

void AFEDataAccess::saveConfiguration(NETWORK configuration) {
  Eeprom.write(148, 32, configuration.ssid);
  Eeprom.write(180, 32, configuration.password);
  Eeprom.write(212, configuration.isDHCP);
  Eeprom.writeIP(213, configuration.ip);
  Eeprom.writeIP(217, configuration.gateway);
  Eeprom.writeIP(221, configuration.subnet);
  Eeprom.writeUInt8(225, configuration.noConnectionAttempts);
  Eeprom.writeUInt8(226, configuration.waitTimeConnections);
  Eeprom.writeUInt8(227, configuration.waitTimeSeries);
}

void AFEDataAccess::saveConfiguration(MQTT configuration) {
  Eeprom.write(229, 32, configuration.host);
  Eeprom.writeIP(261, configuration.ip);
  Eeprom.write(265, 5, (long)configuration.port);
  Eeprom.write(270, 32, configuration.user);
  Eeprom.write(302, 32, configuration.password);
  Eeprom.write(334, 32, configuration.topic);
}

void AFEDataAccess::saveConfiguration(DOMOTICZ configuration) {
  Eeprom.writeUInt8(801, configuration.protocol);
  Eeprom.write(802, 40, configuration.host);
  Eeprom.write(842, 5, (long)configuration.port);
  Eeprom.write(847, 32, configuration.user);
  Eeprom.write(879, 32, configuration.password);
}

void AFEDataAccess::saveConfiguration(uint8_t id, RELAY configuration) {
  uint8_t nextRelay = 26;

  Eeprom.writeUInt8(370 + id * nextRelay, configuration.gpio);
  Eeprom.write(372 + id * nextRelay, 5, configuration.timeToOff);
  Eeprom.writeUInt8(377 + id * nextRelay, configuration.statePowerOn);
  /* @TODO For MQTT only */
  Eeprom.write(378 + id * nextRelay, 16, configuration.name);
  Eeprom.writeUInt8(394 + id * nextRelay, configuration.stateMQTTConnected);
  Eeprom.writeUInt8(421 + id, configuration.ledID);
  Eeprom.write(930 + id, 6, (long)configuration.idx);
}

void AFEDataAccess::saveConfiguration(uint8_t id, LED configuration) {
  uint8_t nextLED = 52;
  Eeprom.writeUInt8(367 + id * nextLED, configuration.gpio);
  Eeprom.write(368 + id * nextLED, configuration.changeToOppositeValue);
}

void AFEDataAccess::saveConfiguration(uint8_t id, SWITCH configuration) {
  uint8_t nextSwitch = 7;
  Eeprom.writeUInt8(396 + id * nextSwitch, configuration.gpio);
  Eeprom.writeUInt8(397 + id * nextSwitch, configuration.type);
  Eeprom.write(398 + id * nextSwitch, 3, (long)configuration.sensitiveness);
  Eeprom.writeUInt8(401 + id * nextSwitch, configuration.functionality);
  Eeprom.writeUInt8(416 + id, configuration.relayID);
}

void AFEDataAccess::saveVersion(String version) { Eeprom.write(0, 7, version); }

boolean AFEDataAccess::getRelayState(uint8_t id) {
  uint8_t nextRelay = 26;
  return Eeprom.read(371 + id * nextRelay);
}

void AFEDataAccess::saveRelayState(uint8_t id, boolean state) {
  uint8_t nextRelay = 26;
  Eeprom.write(371 + id * nextRelay, state);
}

uint8_t AFEDataAccess::getDeviceMode() { return Eeprom.readUInt8(26); }

void AFEDataAccess::saveDeviceMode(uint8_t mode) {
  Eeprom.writeUInt8(26, mode);
}

uint8_t AFEDataAccess::getLanguage() { return Eeprom.readUInt8(8); }

void AFEDataAccess::saveLanguage(uint8_t language) {
  Eeprom.writeUInt8(8, language);
}

uint8_t AFEDataAccess::getSystemLedID() { return Eeprom.readUInt8(415); }

void AFEDataAccess::saveSystemLedID(uint8_t id) { Eeprom.writeUInt8(415, id); }

const String AFEDataAccess::getDeviceID() { return Eeprom.read(1000, 8); }

void AFEDataAccess::saveDeviceID(String id) { Eeprom.write(1000, 8, id); }

void AFEDataAccess::saveAPI(uint8_t apiID, boolean state) {
  if (apiID == API_HTTP) {
    Eeprom.write(25, state);
  } else if (apiID == API_MQTT) {
    Eeprom.write(228, state);
  } else if (apiID == API_DOMOTICZ) {
    Eeprom.write(800, state);
    if (state) {
      Eeprom.write(25, true);
    }
  }
}
