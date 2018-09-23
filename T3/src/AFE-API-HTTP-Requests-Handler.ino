/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

/* Method listens for HTTP Requests */
void mainHTTPRequestsHandler() {
  if (Device.configuration.httpAPI) {
    if (WebServer.httpAPIlistener()) {
      Led.on();
      processHTTPAPIRequest(WebServer.getHTTPCommand());
      Led.off();
    }
  }
}

/* Method creates JSON respons after processing HTTP API request, and pushes it.
 * The second one method converts float to charString before pushing response */
void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status,
                              const char *value = "") {
  String respond;
  respond = "{";
  if (strlen(request.device) > 0) {
    respond += "\"device\":\"" + String(request.device) + "\",";
  }
  if (strlen(request.name) > 0) {
    respond += "\"name\":\"" + String(request.name) + "\",";
  }
  if (strlen(request.command) > 0) {
    respond += "\"command\":\"" + String(request.command) + "\",";
  }

  if (!strlen(value) == 0) {
    respond += "\"value\":\"";
    respond += value;
    respond += "\",";
  }

  respond += "\"status\":\"";
  respond += status ? "success" : "error";
  respond += "\"}";
  WebServer.sendJSON(respond);
}
void sendHTTPAPIRequestStatus(HTTPCOMMAND request, boolean status, float value,
                              uint8_t width = 2, uint8_t precision = 2) {
  char valueString[10];
  dtostrf(value, width, precision, valueString);
  sendHTTPAPIRequestStatus(request, status, valueString);
}

/* Method converts Relay value to string and invokes sendHTTPAPIRequestStatus
 * method which creates JSON respons and pushes it */
void sendHTTPAPIRelayRequestStatus(HTTPCOMMAND request, boolean status,
                                   byte value) {
  sendHTTPAPIRequestStatus(request, status, value == RELAY_ON ? "on" : "off");
}

void sendHTTPAPIPirRequestStatus(HTTPCOMMAND request, boolean status,
                                 byte value) {
  sendHTTPAPIRequestStatus(request, status,
                           value == PIR_OPEN ? "open" : "closed");
}

/* Method processes HTTP API request */
void processHTTPAPIRequest(HTTPCOMMAND request) {
  /* Checking of request is about a relay */
  if (strcmp(request.device, "relay") == 0) {
    boolean noRelay = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
      if (Device.configuration.isRelay[i]) {
        if (strcmp(request.name, Relay[i].getName()) == 0) {
          noRelay = false;
          if (strcmp(request.command, "on") == 0) {
            Relay[i].on();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == RELAY_ON,
                                          Relay[i].get());
          } else if (strcmp(request.command, "off") == 0) { // Off
            Relay[i].off();
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            }
            sendHTTPAPIRelayRequestStatus(request, Relay[i].get() == RELAY_OFF,
                                          Relay[i].get());
          } else if (strcmp(request.command, "toggle") == 0) { // toggle
            uint8_t state = Relay[i].get();
            Relay[i].toggle();
            sendHTTPAPIRelayRequestStatus(request, state != Relay[i].get(),
                                          Relay[i].get());
            MQTTPublishRelayState(i); // MQTT Listener library
            if (strcmp(request.source, "domoticz") != 0) {
              DomoticzPublishRelayState(i);
            };
          } else if (strcmp(request.command, "get") == 0) {
            sendHTTPAPIRelayRequestStatus(request, true, Relay[i].get());
            /* Command not implemented.Info */
          } else {
            sendHTTPAPIRequestStatus(request, false);
          }
        }
      } else {
        break;
      }
    }
    if (noRelay) {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.device, "pir") == 0) {
    boolean noPir = true;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isPIR); i++) {
      if (Device.configuration.isPIR[i]) {
        if (strcmp(request.name, Pir[i].getName()) == 0) {
          noPir = false;
          if (strcmp(request.command, "get") == 0) { // get
            sendHTTPAPIPirRequestStatus(request, true, Pir[i].get());
            /* Command not implemented.Info */
          } else {
            sendHTTPAPIRequestStatus(request, false);
          }
        }
      } else {
        break;
      }
    }
    if (noPir) {
      sendHTTPAPIRequestStatus(request, false);
    }
  } else if (strcmp(request.command, "reboot") == 0) { // reboot
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(Device.getMode());
  } else if (strcmp(request.command, "configurationMode") ==
             0) { // configurationMode
    sendHTTPAPIRequestStatus(request, true);
    Device.reboot(MODE_CONFIGURATION);
    /* No such device or commend not implemented */
  } else {
    sendHTTPAPIRequestStatus(request, false);
  }
}
