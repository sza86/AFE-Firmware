/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "AFE-API-MQTT.h"

AFEMQTT::AFEMQTT() {}

void AFEMQTT::begin() {
  NetworkConfiguration = Data.getNetworkConfiguration();
  sprintf(deviceName, "%s", Device.configuration.name);
  Broker.setClient(esp);
  if (strlen(MQTTConfiguration.host) > 0) {
    Broker.setServer(MQTTConfiguration.host, MQTTConfiguration.port);
  } else if (MQTTConfiguration.ip[0] > 0) {
    Broker.setServer(MQTTConfiguration.ip, MQTTConfiguration.port);
  } else {
    isConfigured = false;
  }

  Broker.setCallback(MQTTMessagesListener);
  sprintf(mqttTopicForSubscription, "%s#", MQTTConfiguration.topic);
  Data = {};
}

void AFEMQTT::disconnect() {
  if (Broker.connected()) {
    Broker.disconnect();
  }
}

void AFEMQTT::listener() {
  if (Broker.connected()) {
    Broker.loop();
  } else {
    connect();
  }
}

void AFEMQTT::connect() {

  if (isConfigured) {
    if (sleepMode) {
      if (millis() - sleepStartTime >=
          NetworkConfiguration.waitTimeSeries * 1000) {
        sleepMode = false;
      }
    } else {

      if (ledStartTime == 0) {
        ledStartTime = millis();
      }

      if (delayStartTime == 0) {
        delayStartTime = millis();

        /* LWT Topic */
        char mqttLWTMessage[38];
        sprintf(mqttLWTMessage, "%sstate", MQTTConfiguration.topic);

        if (Broker.connect(deviceName, MQTTConfiguration.user,
                           MQTTConfiguration.password, mqttLWTMessage, 2, false,
                           "disconnected")) {

          /*
                    Serial << endl << "INFO: Connected";
                    Serial << endl
                           << "INFO: Subscribing to : " <<
             mqttTopicForSubscription;
          */
          Broker.subscribe((char *)mqttTopicForSubscription);

          //        Serial << endl << "INFO: Subsribed";

          /* Publishing message that device has been connected */
          publish(MQTTConfiguration.topic, "state", "connected");

          /* Setting Relay state after connection to MQTT */
          for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
            if (Device.configuration.isRelay[i]) {
              if (!Relay[i].setRelayAfterRestoringMQTTConnection()) {
                // Requesting state from MQTT Broker / service
                publish(Relay[i].getMQTTTopic(), "get", "defaultState");
              } else {
                // Updating relay state after setting default value after MQTT
                // connected
                publish(Relay[i].getMQTTTopic(), "state",
                        Relay[i].get() == RELAY_ON ? "on" : "off");
              }
              // Publishing relay state to Domoticz
              DomoticzPublishRelayState(i);
            }
          }
          delayStartTime = 0;
          ledStartTime = 0;
          Led.off();
          connections = 0;

          return;
        }
      }

      if (millis() > ledStartTime + 500) {
        Led.toggle();
        ledStartTime = 0;
      }

      if (millis() >

          delayStartTime + (NetworkConfiguration.waitTimeConnections * 1000)) {
        connections++;
        /*
                Serial << endl
                       << "INFO: MQTT Connection attempt: " << connections + 1
                       << " from " << NetworkConfiguration.noConnectionAttempts
                       << ", connection status: " << Broker.state()
                       << ", connection time: " << millis() - delayStartTime <<
           "ms";

        */
        delayStartTime = 0;
      }

      if (connections >= NetworkConfiguration.noConnectionAttempts) {
        sleepMode = true;
        sleepStartTime = millis();

        delayStartTime = 0;
        ledStartTime = 0;
        Led.off();
        connections = 0;
        /*
                Serial << endl
                       << "WARN: Not able to connect to MQTT.Going to sleep mode
           for "
                       << NetworkConfiguration.waitTimeSeries << "sec.";
        */
      }
    }
  }
}

void AFEMQTT::setReconnectionParams(
    uint8_t no_connection_attempts,
    uint8_t duration_between_connection_attempts,
    uint8_t duration_between_next_connection_attempts_series) {
  NetworkConfiguration.noConnectionAttempts = no_connection_attempts;
  NetworkConfiguration.waitTimeConnections =
      duration_between_connection_attempts;
  NetworkConfiguration.waitTimeSeries =
      duration_between_next_connection_attempts_series;
}

void AFEMQTT::publish(const char *type, const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", MQTTConfiguration.topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publish(const char *type, float value, uint8_t width,
                      uint8_t precision) {
  char message[10];
  dtostrf(value, width, precision, message);
  publish(type, message);
}

void AFEMQTT::publish(const char *topic, const char *type,
                      const char *message) {
  char _mqttTopic[50];
  sprintf(_mqttTopic, "%s%s", topic, type);
  publishToMQTTBroker(_mqttTopic, message);
}

void AFEMQTT::publishToMQTTBroker(const char *topic, const char *message) {
  if (Broker.state() == MQTT_CONNECTED) {
    //  Serial << endl << "INFO: MQTT publising:  " << topic << "  \\ " <<
    //  message;
    Broker.publish(topic, message);
  }
}
