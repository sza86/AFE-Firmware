/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: http://smart-house.adrian.czabanowski.com/afe-firmware-pl/ */

/* Initializing MQTT */
void MQTTInit() {
  if (Device.getMode() != MODE_ACCESS_POINT && Device.configuration.mqttAPI) {
    MQTTConfiguration = Data.getMQTTConfiguration();
    Mqtt.begin();
  }
}

/* Method is launched after MQTT Message is received */
void MQTTMessagesListener(char *topic, byte *payload, unsigned int length) {

  char _mqttTopic[50];
  Led.on();
  // Serial << endl << "INFO: MQTT message recieved: " << topic << " \\ ";

  if (length >= 1) { // command arrived
                     /*
                         for (uint8_t i = 0; i < length; i++) {
                           Serial << (char)payload[i];
                         }
                     */

    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        sprintf(_mqttTopic, "%scmd", Relay[i].getMQTTTopic());

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'n') {
            Relay[i].on();
            Mqtt.publish(Relay[i].getMQTTTopic(), "state", "on");
          } else if ((char)payload[1] == 'f') {
            Relay[i].off();
            Mqtt.publish(Relay[i].getMQTTTopic(), "state", "off");
          } else if ((char)payload[1] == 'e') {
            MQTTPublishRelayState(i);
          } else if ((char)payload[1] == 'o') { // toggle
            Relay[i].get() == RELAY_ON ? Relay[i].off() : Relay[i].on();
            MQTTPublishRelayState(i);
          }
        }
      } else {
        break;
      }
    }

    for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
      if (Device.configuration.isPIR[i]) {
        sprintf(_mqttTopic, "%scmd", Pir[i].getMQTTTopic());

        if (strcmp(topic, _mqttTopic) == 0) {
          if ((char)payload[1] == 'e') { // get
            MQTTPublishPIRState(i);
          }
        }
      } else {
        break;
      }
    }

    sprintf(_mqttTopic, "%scmd", MQTTConfiguration.topic);
    /*
        Serial << endl
               << "DEBUG: "
               << "checking device level messages: " << _mqttTopic;
    */
    if (strcmp(topic, _mqttTopic) == 0) {
      if ((char)payload[2] == 'b') { // reboot
        //      Serial << endl << "INFO: Process: reboot";
        Device.reboot(MODE_NORMAL);
      } else if ((char)payload[2] == 'n') { // configurationMode
        //    Serial << endl << "INFO: Process: configuration Mode";
        Device.reboot(MODE_CONFIGURATION);
      }
    }
  }
  Led.off();
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishRelayState(uint8_t id) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish(Relay[id].getMQTTTopic(), "state",
                 Relay[id].get() == RELAY_ON ? "on" : "off");
  }
}

/* Metod publishes Relay state (used eg by HTTP API) */
void MQTTPublishPIRState(uint8_t id) {
  if (Device.configuration.mqttAPI) {
    Mqtt.publish(Pir[id].getMQTTTopic(), "state",
                 Pir[id].get() == PIR_OPEN ? "open" : "closed");
  }
}
