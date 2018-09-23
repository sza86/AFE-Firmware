void initRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      Relay[i].begin(i);
      Relay[i].setRelayAfterRestoringPower();
    } else {
      break;
    }
  }
}

/* Method checks if any relay should be automatically turned off */
void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      if (Relay[i].autoTurnOff()) {
        Led.on();
        MQTTPublishRelayState(i);
        DomoticzPublishRelayState(i);
        Led.off();
      }
    } else {
      break;
    }
  }
}
