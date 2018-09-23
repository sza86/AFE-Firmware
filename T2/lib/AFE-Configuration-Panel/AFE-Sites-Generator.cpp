#include "AFE-Sites-Generator.h"

AFESitesGenerator::AFESitesGenerator() { language = Data.getLanguage(); }

const String AFESitesGenerator::generateHeader(uint8_t redirect) {

  FIRMWARE configuration;
  configuration = Data.getFirmwareConfiguration();

  String page = "<!doctype html>"
                "<html lang=\"en\">"
                "<head>"
                "<meta charset=\"utf-8\">";

  if (redirect > 0) {
    page += "<meta http-equiv=\"refresh\" content=\"";
    page += String(redirect);
    page += "; url=/\">";
  }

  page += "<title>AFE Firmware ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page +=
      "]</title>"
      "<style>body{margin:0}#c{display:table;width:100%}#l,#r{display:table-"
      "cell}#l{width:300px;background:#282828;padding:20px;color:#eee}#r{"
      "padding:20px}p,a,input,h1,h3,h4,span,label,button,li{font-family:sans-"
      "serif}a{text-decoration:none}.ltit,.ltag{margin:0}.ltag{font-weight:300;"
      "color:#b0cadb;margin-bottom:20px}.lst{list-style:none;margin:0;padding:"
      "0}.itm "
      "a{display:block;text-decoration:none;white-space:nowrap;padding:.2em "
      "1em;color:#777;font-size:95%}.itm a:hover,.itm "
      "a:focus{background-color:#eee;text-decoration:none;padding:.2em "
      "1.5em;color:#000}.ci{margin-bottom:2em}.ci "
      "h1{color:#aaa;border-bottom:1px solid "
      "#eee;font-size:110%;font-weight:500;letter-spacing:.1em}.ci "
      ".cd{color:#444;line-height:1.8em;font-size:80%;font-style:italic}.cm{"
      "color:#999;font-size:90%;margin:0 0 20px 0}.la{margin:0 "
      ".1em;padding:.3em "
      "1em;color:#fff;background:#999;font-size:80%}.lr{background:#ca3c3c}.lg{"
      "background:#2fb548}fieldset{margin:0;padding:.35em 0 "
      ".75em;border:0}.cf{margin-bottom:.5em}.cc{margin:1em 0 .5em 9.4em}.cf "
      "label{text-align:right;display:inline-block;vertical-align:middle;width:"
      "10em;margin:0 1em 0 0;font-size:.875em}.cc "
      "label{font-size:.875em}input,select{padding:.5em "
      ".6em;display:inline-block;border:1px solid "
      "#ccc;vertical-align:middle;box-sizing:border-box}.hint{display:inline-"
      "block;padding-left:.3em;color:#aaa;vertical-align:middle;font-size:80%}."
      "b{font-size:100%;padding:.5em 1em;border:1px solid #999;border:none "
      "rgba(0,0,0,0);text-decoration:none;color:white}.bs{background:#2fb548}."
      "be{background:#ca3c3c}.bw{background:#df7514}.bc{background:#42b8dd}.b:"
      "hover,.b:focus{filter:alpha(opacity=90);background-image:-webkit-linear-"
      "gradient(transparent,rgba(0,0,0,0.05) "
      "40%,rgba(0,0,0,0.10));background-image:linear-gradient(transparent,rgba("
      "0,0,0,0.05) 40%,rgba(0,0,0,0.10))}</style>"
      "</head>"
      "<body>"
      "<div id=\"c\">"
      "<div id=\"l\">"
      "<h3 class=\"ltit\">AFE FIRMWARE</h3>"
      "<h4 class=\"ltag\">";
  page += language == 0 ? "Włącznik" : "Switch";
  page += language == 0 ? " z czujnikiem temperatury i wilgotności"
                        : " with temperature and humidity sensor";
  page += "</h4><h4>MENU</h4>"
          "<ul class=\"lst\">";
  if (Device.getMode() != MODE_NORMAL) {
    Device.begin(); // Reading configuration data
    page += "<li class=\"itm\"><a href=\"\\?option=device\">";
    page += language == 0 ? "Urządzenie" : "Device";
    page += "</a></li> "
            "<li class=\"itm\"><a href=\"\\?option=network\">";
    page += language == 0 ? "Sieć WiFi" : "Network";
    page += "</a></li>";
    if (Device.configuration.mqttAPI) {
      page += "<li class=\"itm\"><a "
              "href=\"\\?option=mqtt\">MQTT "
              "Broker</a></li>";
    }
    if (Device.configuration.domoticzAPI) {
      page += "<li class=\"itm\"><a "
              "href=\"\\?option=domoticz\">";
      page += language == 0 ? "Serwer Domoticz" : "Domoticz Server";
      page += "</a></li>";
    }
    uint8_t itemPresent = 0;

    for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
      if (Device.configuration.isLED[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li class=\"itm\"><a href=\"\\?option=led\">LED";
      page += language == 0 ? "y" : "s";
      page += "</a></li>";
    }

    if (Device.configuration.isDHT) {
      page += "<li class=\"itm\"><a href=\"\\?option=DHT\">";
      page += language == 0 ? "Czujnik DHT" : "DHT sensor";
      page += "</a></li>";
    }

    if (Device.configuration.isRelay[0]) {
      page += "<li class=\"itm\"><a href=\"\\?option=relay0\">";
      page += language == 0 ? "Przekaźnik" : "Relay";
      page += "</a></li>";
      if (Device.configuration.isDHT) {
        page += "<li class=\"itm\"><a href=\"\\?option=thermostat\"> - ";
        page += language == 0 ? "Termostat" : "Thermostat";
        page += "</a></li>";
        page += "<li class=\"itm\"><a href=\"\\?option=humidistat\"> - ";
        page += language == 0 ? "Regulator wilgotności" : "Humidistat";
        page += "</a></li>";
      }
    }
    itemPresent = 0;
    for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
      if (Device.configuration.isSwitch[i]) {
        itemPresent++;
      } else {
        break;
      }
    }

    if (itemPresent > 0) {
      page += "<li  class=\"itm\"><a><i>";
      page += language == 0 ? "Przyciski / wyłączniki" : "Buttons / Switches";
      page += "</i></a></li>";

      for (uint8_t i = 0; i < itemPresent; i++) {
        page += "<li class=\"itm\"><a href=\"\\?option=switch";
        page += i;
        page += "\"> - ";
        page += language == 0 ? "Przycisk: " : "Switch: ";
        page += i + 1;
        page += "</a></li>";
      }
    }

    page +=
        "<br><br><li class=\"itm\"><a "
        "href=\"\\?option=language\">[PL] Język / "
        "[EN] Language</a></li><br><br><li class=\"itm\"><a href=\"\\update\">";
    page += language == 0 ? "Aktulizacja firmware" : "Firmware upgrade";
    page += "</a></li><li class=\"itm\"><a href=\"\\?option=reset\">";
    page += language == 0 ? "Przywracanie ustawień początkowych"
                          : "Reset to orginal state";
    page += "</a></li><br><br><li class=\"itm\"><a href=\"\\?option=exit\">";
    page += language == 0 ? "Zakończ konfigurację" : "Finish configuration";
  } else {
    page += "<li class=\"itm\"><a href=\"\\?option=help&cmd=1\">";
    page += language == 0 ? "Ustawienia" : "Settings";
    page += "</a></li><li class=\"itm\"><a href=\"\\?option=help&cmd=2\">";
    page += language == 0 ? "Ustawienia (tryb:" : "Settings (mode:";
    page += " Access Point)";
  }
  page += "</a></li></ul><br><br><h4>INFORMA";
  page += language == 0 ? "CJE" : "TION";
  page += "</h4><ul class=\"lst\"><li class=\"itm\"><a "
          "href=\"http://smart-house.adrian.czabanowski.com/afe-firmware-";
  page += language == 0 ? "pl" : "en";
  page += "/\" target=\"_blank\">Do";
  page += language == 0 ? "kumentacja" : "cumentation";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"http://smart-house.adrian.czabanowski.com/forum/"
          "firmware-do-przelacznika-sonoff/\" target=\"_blank\">";
  page += language == 0 ? "Pomoc" : "Help";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/"
          "LICENSE\" "
          "target=\"_blank\">Licenc";
  page += language == 0 ? "ja" : "e";
  page += "</a></li><li class=\"itm\"><a "
          "href=\"http://smart-house.adrian.czabanowski.com/afe-firmware-";
  page += language == 0 ? "pl" : "en";
  page += "/log\" target=\"_blank\">";
  page += language == 0 ? "Wersja" : "Version";
  page += " ";
  page += configuration.version;
  page += " [T";
  page += configuration.type;
  page += "]</a></li>"
          "</ul>";
  if (Device.getMode() != MODE_ACCESS_POINT) {
    page += "<br><br><h4>";
    page += language == 0 ? "WSPARCIE" : "DONATION";
    page += "</h4><p class=\"cm\">";
    page += language == 0
                ? "Oprogramowanie dostępne jest za darmo w ramach licencji "
                : "Software is available for free within terms of ";
    page += " <a "
            "href=\"https://github.com/tschaban/AFE-Firmware/blob/master/"
            "LICENSE\" "
            "target=\"_blank\"  style=\"color:#fff\">MIT</a>";
    page += language == 1 ? " licence" : "";
    page += "</p><p class=\"cm\">";
    page += language == 0 ? "Jeśli spełnia Twoje oczekiwania to rozważ wsparcie"
                          : "If the firmware meets your expectations then "
                            "consider donation to it's";
    page += " <a href=\"https://adrian.czabanowski.com\" "
            "target=\"_blank\" style=\"color:#fff\">aut";
    page += language == 0 ? "ora" : "hor";
    page += "</a>. ";
    page += language == 0 ? "Z góry dziękuję" : "Thank you";
    page += "</p>";
    page += "<a "
            "href=\"https://www.paypal.com/cgi-bin/"
            "webscr?cmd=_donations&business=VBPLM42PYCTM8&lc=PL&item_name="
            "Wsparcie%20projektu%20AFE%20Firmware&item_number=Firmware%20%5bvT";
    page += configuration.type;
    page += "%5d&currency_code=PLN&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%"
            "3aNonHosted\" target=\"_blank\"><img "
            "src=\"http://adrian.czabanowski.com/afe/donation/T";
    page += configuration.type;
    page += "/";
    page += configuration.version;
    page += "/";
    page += Data.getDeviceID();
    page += "/\" border=\"0\" alt=\"PayPal\" style=\"width:290px\"></a>";
  }
  page += "</div>"
          "<div id=\"r\">";

  return page;
}

String AFESitesGenerator::addDeviceConfiguration() {
  DEVICE configuration = Data.getDeviceConfiguration();
  uint8_t itemsNumber = 0;

  String body = "<fieldset><div class=\"cf\"><label>";
  body += language == 0 ? "Nazwa urządzenia" : "Device name";
  body += "*</label><input name=\"dn\" type=\"text\" maxlength=\"16\" "
          "value=\"";
  body += configuration.name;
  body += "\"><span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span></div></fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Nazwa urządzenia" : "Device name",
      language == 0
          ? "Nazwa jest wykorzystywana jako nazwa urządzenia w Twojej "
            "lokalnej sieci WiFi oraz jako nazwa hotspot'a urządzenia"
          : "Name is used for device identification within your LAN and "
            "a device's hotspot name used for configuration mode",
      body);

  body = "<fieldset>";
  for (uint8_t i = 0; i < sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isLED), itemsNumber, "hl",
      language == 0 ? "Ilość Led'ów" : "Number of LEDs");

  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }

  body += generateHardwareItemsList(
      sizeof(Device.configuration.isRelay), itemsNumber, "hr",
      language == 0 ? "Ilość przekaźników" : "Number of relay");

  itemsNumber = 0;
  for (uint8_t i = 0; i < sizeof(Device.configuration.isSwitch); i++) {
    if (Device.configuration.isSwitch[i]) {
      itemsNumber++;
    } else {
      break;
    }
  }
  body += generateHardwareItemsList(
      sizeof(Device.configuration.isSwitch), itemsNumber, "hs",
      language == 0 ? "Ilość przycisków" : "Number of switches");

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"ds\" type=\"checkbox\" value=\"1\"";
  body += configuration.isDHT ? " checked=\"checked\">" : ">";
  body += language == 0 ? "Czujnik" : " Sensor";
  body += " DHT";
  body += "</label>";
  body += "</div>";

  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Konfiguracja urządzenia" : "Hardware configuration",
      language == 0 ? "Wybierz podłączone elementy do urządzenia"
                    : "Select connected items to your device",
      body);

  body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"m\" type=\"checkbox\" value=\"1\"";
  body += configuration.mqttAPI ? " checked=\"checked\"" : "";
  body += ">MQTT API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"h\" type=\"checkbox\" value=\"1\"";
  body += configuration.httpAPI ? " checked=\"checked\"" : "";
  body += ">HTTP API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"d\" type=\"checkbox\" value=\"1\"";
  body += configuration.domoticzAPI ? " checked=\"checked\"" : "";
  body += ">Domoticz API ";
  body += language == 0 ? "włączone" : "enabled";
  body += "?";
  body += "</label>";
  body += "</div>";

  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Sterowanie urządzeniem" : "Device controlling mechanism",
      language == 0
          ? "Włączanie / Wyłączanie mechanizmów sterowania urządzeniem"
          : "Enable / Disable APIs",
      body);

  return page;
}

String AFESitesGenerator::addNetworkConfiguration() {

  NETWORK configuration;
  configuration = Data.getNetworkConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa sieci WiFI" : "WiFi name";
  body += "*</label>";
  body += "<input name=\"s\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.ssid;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "*</label>";
  body += "<input type=\"password\" name=\"p\" maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>MAC</label>";
  body += "<input type=\"text\" readonly=\"readonly\" value=\"";
  body += WiFi.macAddress();
  body += "\">";
  body += "</div>";
  body += "</fieldset>";

  String page = addConfigurationBlock(
      language == 0 ? "Sieć WiFi" : "WiFi network",
      language == 0 ? "Urządzenie bez dostępu do sieci WiFi będzie "
                      "działać tylko w trybie sterowania ręcznego"
                    : "Device without access to WiFi network will only work in "
                      "manual mode",
      body);

  body = "<fieldset>";
  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"d\" type=\"checkbox\" value=\"1\"";
  body += (configuration.isDHCP ? " checked=\"checked\"" : "");
  body += "> ";
  body +=
      language == 0 ? "Konfiguracja przez DHCP?" : "Configuration over DHCP";
  body += "</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Adres IP" : "IP Addess";
  body += "</label>";
  body += "<input name=\"d1\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body += "\">.";
  body += "<input name=\"d2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body += "\">.";
  body += "<input name=\"d3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"d4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\">";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Bramka" : "Gateway";
  body += "</label>";
  body += "<input name=\"g1\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[0];
  body += "\">.";
  body += "<input name=\"g2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[1];
  body += "\">.";
  body += "<input name=\"g3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[2];
  body += "\">.";
  body += "<input name=\"g4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.gateway[3];
  body += "\">";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Maska sieci" : "Subnet";
  body += "</label>";
  body += "<input name=\"s1\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[0];
  body += "\">.";
  body += "<input name=\"s2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[1];
  body += "\">.";
  body += "<input name=\"s3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[2];
  body += "\">.";
  body += "<input name=\"s4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.subnet[3];
  body += "\">";
  body += "</div>";

  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Adres IP urządzenia" : "Device's IP address",
      language == 0 ? "Możesz skonfigurować adres IP urządzenia ręcznie lub "
                      "automatycznie za pośrednictwem DHCP"
                    : "You can set up device's IP address manually or "
                      "automatically by DHCP",
      body);

  body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Ilość prób łączenia się"
                        : "Number of connection attempts";
  body += "*</label>";
  body += "<input name=\"na\" type=\"number\" min=\"1\" max=\"255\" step=\"1\" "
          "value=\"";
  body += configuration.noConnectionAttempts;
  body += "\">";
  body += "<span class=\"hint\">1-255</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Czas między próbami łączenia się"
                        : "Time between connections' attempts";
  body += "*</label>";
  body += "<input type=\"number\" name=\"wc\" "
          "min=\"1\" max=\"255\" step=\"1\" value=\"";
  body += configuration.waitTimeConnections;
  body += "\">";
  body += "<span class=\"hint\">1-255 (";
  body += language == 0 ? "sekundy" : "seconds";
  body += ")</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Czas po jakim powtórzyć próby łączenia się"
                        : "Sleep time before next connection attempts";
  body += "*</label>";
  body += "<input type=\"number\" name=\"ws\" min=\"1\" max=\"255\" step=\"1\" "
          "value=\"";
  body += configuration.waitTimeSeries;
  body += "\">";
  body += "<span class=\"hint\">1-255 (";
  body += language == 0 ? "sekundy" : "seconds";
  body += ")</span>";
  body += "</div>";
  body += "</fieldset>";

  page += addConfigurationBlock(
      language == 0 ? "Konfiguracja nawiązywania połączeń z siecią WiFi"
                    : "Network connection's configuration",
      "", body);

  return page;
}

String AFESitesGenerator::addMQTTBrokerConfiguration() {

  MQTT configuration;
  configuration = Data.getMQTTConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Hostname</label>";
  body += "<input name=\"h\" type=\"text\" maxlength=\"32\" value=\"";
  body += configuration.host;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Adres IP" : "IP address";
  body += "</label>";
  body += "<input name=\"m1\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[0];
  body += "\">.";
  body += "<input name=\"m2\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[1];
  body += "\">.";
  body += "<input name=\"m3\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[2];
  body += "\">.";
  body += "<input name=\"m4\" type=\"number\" max=\"255\" min=\"0\" step=\"1\" "
          "style=\"width:70px\" value=\"";
  body += configuration.ip[3];
  body += "\">";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Port*</label>";
  body += "<input name=\"p\" type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\">";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Użytkownik" : "User";
  body += "</label>";
  body += "<input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "</label>";
  body += "<input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Temat wiadomości" : "MQTT Topic";
  body += "*</label>";
  body += "<input name=\"t\" type=\"text\""
          "maxlength=\"32\" value=\"";
  body += configuration.topic;
  body += "\">";
  body += "<span class=\"hint\">Format <strong>/abc/def/</strong>. Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "</fieldset>";

  return addConfigurationBlock(
      "MQTT Broker",
      language == 0 ? "Wprowadź adres hosta np. localhost lub adres IP"
                    : "Enter MQTT Broker hostname or its IP address",
      body);
}

String AFESitesGenerator::addDomoticzServerConfiguration() {

  DOMOTICZ configuration;
  configuration = Data.getDomoticzConfiguration();

  String body = "<fieldset>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Protokół" : "Protocol";
  body += "* </label>";
  body += "<select name=\"t\">";
  body += "<option value=\"0\"";
  body += configuration.protocol == 0 ? " selected=\"selected\"" : "";
  body += ">http://</option>";
  body += "<option value=\"1\"";
  body += configuration.protocol == 1 ? " selected=\"selected\"" : "";
  body += ">https://</option>";
  body += "</select>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>Hostname/IP*</label>";
  body += "<input name=\"h\" type=\"text\" maxlength=\"40\" value=\"";
  body += configuration.host;
  body += "\">";
  body += "<span class=\"hint\">Max 40 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>Port*</label>";
  body += "<input name=\"p\" type=\"number\""
          " min=\"0\" max=\"65535\" step=\"1\" value=\"";
  body += configuration.port;
  body += "\">";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Użytkownik" : "User";
  body += "</label>";
  body += "<input name=\"u\" type=\"text\"  maxlength=\"32\" value=\"";
  body += configuration.user;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Hasło" : "Password";
  body += "</label>";
  body += "<input name=\"s\" type=\"password\"  maxlength=\"32\" "
          "value=\"";
  body += configuration.password;
  body += "\">";
  body += "<span class=\"hint\">Max 32 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  body += "</fieldset>";

  return addConfigurationBlock(
      language == 0 ? "Serwer Domoticz" : "Domoticz Server",
      language == 0 ? "Wprowadź adres hosta np. localhost lub adres IP"
                    : "Enter Domoticz hostname or its IP address",
      body);
}

String AFESitesGenerator::addLEDConfiguration(uint8_t id) {
  LED configuration;
  configuration = Data.getLEDConfiguration(id);

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "<label style=\"width: 300px;\">";
  body += "<input name=\"o";
  body += id;
  body += "\" type=\"checkbox\" value=\"1\"";
  body += configuration.changeToOppositeValue ? " checked=\"checked\"" : "";
  body += ">";
  body += language == 0 ? "Zmień świecenie diody LED na odwrotne"
                        : "Change LED ligtning to opposite";
  body += "</label>";
  body += "</div>";

  body += "</fieldset>";

  return addConfigurationBlock("LED #" + String(id + 1), "", body);
}

String AFESitesGenerator::addSystemLEDConfiguration() {
  uint8_t configuration = Data.getSystemLedID();
  AFEDevice Device;

  String body = "<fieldset>";

  body += "<div class=\"cf\">";
  body += "<label>LED*</label>";

  body += "<select name=\"i\">";

  body += "<option value=\"0\"";
  body += configuration == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select>";
  body += "</div>";

  body += "</fieldset>";

  return addConfigurationBlock(
      language == 0 ? "LED systemowy" : "System LED",
      language == 0 ? "Wybierz diodę LED, która będzie sygnalizowała stan "
                      "urządzenia oraz występujące zdarzenia"
                    : "Select LED which will be informing about device status "
                      "and its events ",
      body);
}

String AFESitesGenerator::addRelayConfiguration(uint8_t id) {

  RELAY configuration = Data.getRelayConfiguration(id);

  DEVICE device;
  device = Data.getDeviceConfiguration();

  String body = "<fieldset>";

  char filed[13];
  sprintf(filed, "g%d", id);

  body += generateConfigParameter_GPIO(filed, configuration.gpio);

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa" : "Name";
  body += "*</label>";
  body += "<input name=\"n" + String(id) +
          "\" type=\"text\" maxlength=\"16\" value=\"";
  body += configuration.name;
  body += "\">";
  body += "<span class=\"hint\">Max 16 ";
  body += language == 0 ? "znaków" : "chars";
  body += "</span>";
  body += "</div>";
  if (device.domoticzAPI) {
    body += "<div class=\"cf\">";
    body += "<label>IDX ";
    body += language == 0 ? "w" : "in";
    body += " Domoticz*</label>";
    body += "<input name=\"x" + String(id) +
            "\" type=\"number\" step=\"1\" min=\"0\" max=\"999999\"  value=\"";
    body += configuration.idx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
  }
  body += "<p class=\"cm\">";
  body += language == 0 ? "Wartości domyślne" : "Default values";
  body += "</p>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Po przywróceniu zasilania"
                        : "When power is restored set it to";
  body += "</label>";
  body += "<select name=\"pr" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.statePowerOn == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Brak akcji" : "No action";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.statePowerOn == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Wyłączony" : "Off";
  body += "</option>";
  body += "<option value=\"2\"";
  body += (configuration.statePowerOn == 2 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Włączony" : "On";
  body += "</option>";
  body += "<option value=\"3\"";
  body += (configuration.statePowerOn == 3 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Ostatnia zapamiętana wartość" : "Last known state";
  body += "</option>";
  body += "<option value=\"4\"";
  body += (configuration.statePowerOn == 4 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Przeciwna do ostatniej zapamiętanej"
                        : "Opposite to the last known state";
  body += "</option>";
  body += "</select>";
  body += "</div>";

  if (device.mqttAPI) {
    body += "<div class=\"cf\">";
    body += "<label>";
    body += language == 0 ? "Po podłączeniu do brokera MQTT"
                          : "After establishing connection to MQTT Broker";
    body += "</label>";
    body += "<select  name=\"mc" + String(id) + "\">";
    body += "<option value=\"0\"";
    body +=
        (configuration.stateMQTTConnected == 0 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Brak akcji" : "No action";
    body += "</option>";
    body += "<option value=\"1\"";
    body +=
        (configuration.stateMQTTConnected == 1 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Wyłączony" : "Off";
    body += "</option>";
    body += "<option value=\"2\"";
    body +=
        (configuration.stateMQTTConnected == 2 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Włączony" : "On";
    body += "</option>";
    body += "<option value=\"3\"";
    body +=
        (configuration.stateMQTTConnected == 3 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Ostatnia zapamiętana wartość" : "Last known state";
    body += "</option>";
    body += "<option value=\"4\"";
    body +=
        (configuration.stateMQTTConnected == 4 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0 ? "Przeciwna do ostatniej zapamiętanej"
                          : "Opposite to the last known state";
    body += "</option>";
    body += "<option value=\"5\"";
    body +=
        (configuration.stateMQTTConnected == 5 ? " selected=\"selected\"" : "");
    body += ">";
    body += language == 0
                ? "Wartość z systemu sterowania przekaźnikiem (przez MQTT)"
                : "Get state from a relay's controlling system (over MQTT)";
    body += "</option>";
    body += "</select>";
    body += "</div>";
  }
  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Automatyczne wyłączenie przekaźnika"
                        : "Automatic switching off of the relay";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wyłącz po" : "Switch off after";
  body += "*</label>";
  body += "<input name=\"ot" + String(id) +
          "\" type=\"number\" step=\"0.01\" min=\"0\" max=\"86400\"  value=\"";
  body += configuration.timeToOff;
  body += "\">";
  body += "<span class=\"hint\">0.01 - 86400";
  body += language == 0 ? "sek (24h). Brak akcji jeśli jest 0"
                        : "sec (24h). No action if it's set to 0";
  body += "</span>";
  body += "</div>";

  if (device.isDHT) {

    body += "<br><p class=\"cm\">";
    body += language == 0 ? "Zabezpieczenie termiczne" : "Thermal protection";
    body += "</p>";

    body += "<div class=\"cf\">";
    body += "<label>";
    body += language == 0 ? "Wyłącz powyżej" : "Switch off above";
    body += "*</label>";
    body += "<input name=\"tp" + String(id) +
            "\" type=\"number\" step=\"1\" min=\"-67\" max=\"259\"  value=\"";
    body += configuration.thermalProtection;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres" : "Range";
    body += ": -55C : +125C (-67F : +259F). ";
    body += language == 0 ? "Brak akcji jeśli jest 0"
                          : "No action if it's set to 0";
    body += "</span></div>";
  }

  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Wybierz LED sygnalizujący stan przekaźnika"
                        : "Select LED informing about relay state";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>LED</label>";

  body += "<select  name=\"l" + String(id) + "\">";

  body += "<option value=\"0\"";
  body += configuration.ledID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isLED); i++) {
    if (Device.configuration.isLED[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.ledID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select>";
  body += "</div>";

  body += "</fieldset>";

  char title[23];
  language == 0 ? sprintf(title, "Przekaźnik #%d", id + 1)
                : sprintf(title, "Relay #%d", id + 1);
  return addConfigurationBlock(title, "", body);
}

String AFESitesGenerator::addThermostatConfiguration() {
  RELAY configuration = Data.getRelayConfiguration(0);
  String body = generateRegulatorController(configuration.thermostat, true);
  return addConfigurationBlock(
      language == 0 ? "Termostat" : "Thermostat",
      language == 0
          ? "Termostat kontroluje przekaźnik w "
            "zależności od wartości temperatury"
          : "Thermostat controlls the relay depending on temperature value",
      body);
}

String AFESitesGenerator::addHumidistatConfiguration() {
  RELAY configuration = Data.getRelayConfiguration(0);
  String body = generateRegulatorController(configuration.humidistat, false);
  return addConfigurationBlock(
      language == 0 ? "Regulator wilgotności" : "Humidistat",
      language == 0
          ? "Regulator wilgotności kontroluje przekaźnik w "
            "zależności od wartości wilgotności"
          : "Humidistat controlls the relay depending on humidity value",
      body);
}

String AFESitesGenerator::addSwitchConfiguration(uint8_t id) {

  SWITCH configuration;
  configuration = Data.getSwitchConfiguration(id);

  String body = "<fieldset>";
  char filed[13];
  sprintf(filed, "g%d", id);
  body += "<div class=\"cf\">";
  body += generateConfigParameter_GPIO(filed, configuration.gpio);
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Funkcja" : "Functionality";
  body += "</label>";
  body += "<select name=\"f" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.functionality == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Przycisk systemowy" : "System button";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.functionality == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Tylko sterowanie przekaźnikiem"
                        : "Controlling only the relay";
  body += "</option>";

  body += "</select>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Przekaźnik sterowany tym przyciskiem"
                        : "Select relay controlled by this switch";
  body += "</label>";

  body += "<select  name=\"r" + String(id) + "\">";

  body += "<option value=\"0\"";
  body += configuration.relayID == 0 ? " selected=\"selected\"" : "";
  body += language == 0 ? ">Brak" : ">None";
  body += "</option>";

  for (uint8_t i = 1; i <= sizeof(Device.configuration.isRelay); i++) {
    if (Device.configuration.isRelay[i - 1]) {
      body += "<option value=\"";
      body += i;
      body += "\"";
      body += configuration.relayID == i ? " selected=\"selected\"" : "";
      body += ">";
      body += i;
      body += "</option>";
    } else {
      break;
    }
  }

  body += "</select>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Typ" : "Type";
  body += "*</label>";
  body += "<select name=\"t" + String(id) + "\">";
  body += "<option value=\"0\"";
  body += (configuration.type == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Monostabilny" : "Monostable";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Bistabilny" : "Bistable";
  body += "</option>";
  body += "</select>";
  body += "</div>";
  body += "<br><p class=\"cm\">";
  body += language == 0
              ? "Czułość należy ustawić metodą prób, aż uzyska się "
                "pożądane działanie przycisku podczas jego wciskania"
              : "Sensitiveness should be adjusted if switch didn't behave "
                "as expected while pressing it";

  body += "</p><div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Czułość" : "Sensitiveness";
  body += "*</label>";
  body += "<input name=\"s" + String(id) +
          "\" type=\"number\" max=\"999\" min=\"0\" step=\"1\" "
          "value=\"";
  body += configuration.sensitiveness;
  body += "\">";
  body += "<span class=\"hint\">0 - 999 (milise";
  body += language == 0 ? "kund" : "conds";
  body += ")</span>";
  body += "</div>";
  body += "</fieldset>";

  char title[23];
  language == 0 ? sprintf(title, "Przycisk / Włącznik #%d", id + 1)
                : sprintf(title, "Switch / Button #%d", id + 1);

  return addConfigurationBlock(title, "", body);
}

String AFESitesGenerator::addDHTConfiguration() {

  DH configuration = Data.getDHTConfiguration();
  DEVICE device = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += generateConfigParameter_GPIO("g", configuration.gpio);

  body += "<div class=\"cf\">";
  body += "<label>Typ";
  body += language == 1 ? "e" : "";
  body += "</label>";
  body += "<select name=\"t\">";
  body += "<option value=\"1\"";
  body += (configuration.type == 1 ? " selected=\"selected\"" : "");
  body += ">DH11</option>";
  body += "<option value=\"2\"";
  body += (configuration.type == 2 ? " selected=\"selected\"" : "");
  body += ">DH21</option>";
  body += "<option value=\"3\"";
  body += (configuration.type == 3 ? " selected=\"selected\"" : "");
  body += ">DH22</option>";
  body += "</select>";
  body += "</div>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"o\" type=\"checkbox\" value=\"1\"";
  body += configuration.sendOnlyChanges ? " checked=\"checked\"" : "";
  body +=
      language == 0
          ? ">Wysyłać dane tylko, gdy wartość temperatury lub wilgotności "
            "zmieni się"
          : ">Send data only if value of temperature or humidity has changed";
  body += "</label>";
  body += "</div>";

  if (device.mqttAPI) {
    body += "<div class=\"cc\">";
    body += "<label>";
    body += "<input name=\"p\" type=\"checkbox\" value=\"1\"";
    body += configuration.publishHeatIndex ? " checked=\"checked\"" : "";
    body += language == 0 ? ">Wysyłać temperaturę odczuwalną"
                          : ">Publish felt air temperature";
    body += "?</label>";
    body += "</div>";
  }

  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Czujnik temperatury" : "Temperature sensor";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label>";
  body +=
      "<input name=\"i\" min=\"10\" max=\"86400\" step=\"1\" type=\"number\" "
      "value=\"";
  body += configuration.temperature.interval;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 10 do 86400sek"
                        : "seconds. Range: 10 to 86400sec";
  body += " (24h)</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Korekta wartości o" : "Temperature correction";
  body += "</label>";
  body += "<input name=\"c\" type=\"number\" min=\"-9.99\" max=\"9.99\" "
          "step=\"0.01\" "
          "value=\"";
  body += configuration.temperature.correction;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "stopni. Zakres" : "degrees. Range";
  body += ": -9.99 - +9.99</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Jednostka" : "Unit";
  body += "</label>";
  body += "<select  name=\"u\">";
  body += "<option value=\"0\"";
  body +=
      (configuration.temperature.unit == 0 ? " selected=\"selected\">" : ">");
  body += language == 0 ? "Celsjusz" : "Celsius";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.temperature.unit == 1 ? " selected=\"selected\"" : "");
  body += ">Fahrenheit</option>";
  body += "</select>";
  body += "</div>";

  body += "<br><p class=\"cm\">";
  body += language == 0 ? "Czujnik wilgotności" : "Humidity sensor";
  body += "</p>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Odczyty co" : "Read every";
  body += "</label>";
  body +=
      "<input name=\"j\" min=\"10\" max=\"86400\" step=\"1\" type=\"number\" "
      "value=\"";
  body += configuration.humidity.interval;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "sekund. Zakres: 10 do 86400sek"
                        : "seconds. Range: 10 to 86400sec";
  body += " (24h)</span>";
  body += "</div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Korekta wartości o" : "Humidity correction";
  body += "</label>";
  body += "<input name=\"d\" type=\"number\" min=\"-99.9\" max=\"99.9\" "
          "step=\"0.1\" "
          "value=\"";
  body += configuration.humidity.correction;
  body += "\">";
  body += "<span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  body += ": -99.9 - +99.9</span>";
  body += "</div>";

  if (device.domoticzAPI) {

    body += "<br><p class=\"cm\">";
    body +=
        language == 0 ? "Konfiguracja dla Domoticz" : "Domoticz configuration";
    body += "</p>";
    body += "<p class=\"cm\">";
    body += language == 0
                ? "Jeśli IDX jest 0 to wartośc nie będzie wysyłana do "
                : "If IDX is set to 0 then a value won't be sent to ";
    body += "Domoticz</p>";

    body += "<div class=\"cf\"><label> ";
    body +=
        language == 0 ? "IDX czujnika temperatury" : "Temperature sensor IDX";
    body += " </label>";
    body += "<input name=\"xt\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika wilgotności" : "Humidity sensor IDX";
    body += "</label><input name=\"xh\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.humidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";

    body += "<div class=\"cf\"><label>";
    body += language == 0 ? "IDX czujnika temperatury i wilgotności"
                          : "Temperature and humidity sensor IDX";
    body += "</label><input name=\"xth\" type=\"number\" step=\"1\" min=\"0\" "
            "max=\"999999\"  value=\"";
    body += configuration.temperatureAndHumidityIdx;
    body += "\">";
    body += "<span class=\"hint\">";
    body += language == 0 ? "Zakres: " : "Range: ";
    body += "0 - 999999</span>";
    body += "</div>";
  }

  body += "</fieldset>";

  return addConfigurationBlock(language == 0
                                   ? "Czujnik temperatury i wilgotności DHT"
                                   : "DHT temperature and humidity sensor",
                               "", body);
}

String AFESitesGenerator::addUpgradeSection() {
  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wybierz" : "Select";
  body += " firmware</label>";
  body += "<input class=\"bs\" name=\"update\" type=\"file\" accept=\".bin\">";
  body += "</div>";
  body += "<p class=\"cm\">";
  body +=
      language == 0
          ? "Po zakończeniu aktualizacji urządzenie zostanie "
            "automatycznie zresetowane<br><br><strong>Uwaga:</strong> po "
            "aktualizacji nie odłączaj urządzenia przez conajmniej 1min.<br>"
            "Urządzenie formatuje pamięc i wgrywa domyślne ustawienia. "
            "<br><strong>Ten proces nie może zostac przerwany</strong>."
          : "Device will be automatically rebooted after "
            "upgrade<br><br><strong>Warning</strong>: after upgrade do not "
            "plug "
            "off the device from power source for around a minute.<br>Device's "
            "memory "
            "will be formatted and default settings will be "
            "uploaded.<br><strong>This process cannot be interrupted</strong>";
  body += "</p>";
  body += "<button type=\"submit\" class=\"b be\">";
  body += language == 0 ? "Aktualizuj" : "Upgrade";
  body += "</button>";
  body += "</fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Aktualizacja firmware" : "Firmware upgrade",
      language == 0 ? "<strong>UWAGA</strong>: nie odłączaj "
                      "urządzenia od źródła zasilania podczas aktualizacji"
                    : "<strong>Warning</strong>: don't plug off the device "
                      "from a power source while upgrading",
      body);
}

String AFESitesGenerator::addPostUpgradeSection(boolean status) {

  String body = "<fieldset><ul>";
  if (status) {
    body += "<li style=\"color:red\">";
    body += language == 0 ? "Aktualizacja nie powiodła się" : "Upgrade failed";
  } else {
    body += "<li class=\"cm\">";
    body += language == 0 ? "Aktualizacja zakończona pomyślnie"
                          : "Upgrade finished succesfully";
  }
  body += "</li><li class=\"cm\">";
  body += language == 0
              ? "Po 10 "
                "sekundach przełącznik zostanie przeładowany. Proszę czekać"
              : "After 10s device will be rebooted. Please wait";
  body += "....</li>";
  body += "</fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Aktualizacja firmware" : "Firmware upgrade", "", body);
}

String AFESitesGenerator::addResetSection(uint8_t command) {
  String body = "<fieldset>";
  String subtitle;
  if (command == 0) {
    body += "<a href=\"\\?option=reset&cmd=1\" class=\"b be\">";
    body += language == 0 ? "Przywróć ustawienia początkowe"
                          : "Restore default settings";
    body += "</a>";
    subtitle = "<strong>";
    subtitle += language == 0 ? "Uwaga" : "Warning";
    subtitle += "</strong>: ";
    subtitle +=
        language == 0
            ? "przywrócenie ustawień "
              "początkowych usuwa wszystkie ustawienia "
              "urządzenia, włącznie z konfiguracją sieci WiFi"
            : "restoring to default settings will remove all configuration's "
              "information, incl. WiFi configuration";
  } else {
    subtitle += "";
    body += "<p class=\"cm\">";
    body += language == 0 ? "Trwa przywracanie ustawień początkowych"
                          : "Restoring configuration is in progress";
    body += "</p><p class=\"cm\">";
    body += language == 0 ? "Po 20 sekundach połącz się z siecią WiFi o "
                            "nazwie: <strong>AFE-Device</strong>, a następnie "
                            "połącz się z "
                            "panelem konfiguracyjnym pod adresem"
                          : "After 20 seconds connect to WiFi network called "
                            "<strong>AFE-Device</strong> and open "
                            "configuration panel";
    body += ": </p>";
    body += "<a href=\"http://192.168.5.1\">http://192.168.5.1</a>";
  }
  body += "</fieldset>";
  return addConfigurationBlock(language == 0
                                   ? "Przywracanie ustawień początkowych"
                                   : "Restoring to default settings",
                               subtitle, body);
}

String AFESitesGenerator::addExitSection() {
  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<p class=\"cm\">";
  body += language == 0 ? "Trwa restart urządzenia" : "Reboot in progress";
  body += ".....</p>";
  body += "<p class=\"cm\">";
  body += language == 0 ? "Strona zostanie przeładowana automatycznie... czekaj"
                        : "Site will be automatically reloaded... wait";
  body += "</p>";
  body += "</div>";
  body += "</fieldset>";
  return addConfigurationBlock(
      language == 0 ? "Restart urządzenia" : "Device reboot", "", body);
}

String AFESitesGenerator::addHelpSection() {
  DEVICE configuration;
  configuration = Data.getDeviceConfiguration();

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Nazwa urządzenia" : "Device name";
  body += "</label>";
  body += "<span>";
  body += configuration.name;
  body += "</span>";
  body += "</div>";
  body += "</fieldset>";
  return addConfigurationBlock(language == 0
                                   ? "Urządzenie jest w trybie operacyjnym"
                                   : "Device is in working mode",
                               "", body);
}

String AFESitesGenerator::addLanguageConfiguration() {

  String body = "<fieldset>";
  body += "<div class=\"cf\">";
  body += "<label>Language / Język";
  body += ": </label>";
  body += "<select name=\"l\">";
  body += "<option value=\"0\"";
  body += language == 0 ? " selected=\"selected\"" : "";
  body += ">Polski</option>";
  body += "<option value=\"1\"";
  body += language == 1 ? " selected=\"selected\"" : "";
  body += ">English</option>";
  body += "</select>";
  body += "</div>";
  body += "</fieldset>";
  String page = addConfigurationBlock("Language / Język",
                                      "Choose language / "
                                      "Wybierz język",
                                      body);
  return page;
}

const char *AFESitesGenerator::generateFooter() {
  return "</div></div></body></html>";
}

String AFESitesGenerator::addConfigurationBlock(const String title,
                                                const String description,
                                                const String body) {
  String page = "<div class=\"ci\">";
  page += "<h1>" + title + "</h1>";
  page += "<p class=\"cd\">" + description + "</p>";
  page += body;
  page += "</div>";

  return page;
}

const String AFESitesGenerator::generateConfigParameter_GPIO(const char *field,
                                                             uint8_t selected) {
  String page = "<div class=\"cf\"><label>GPIO</label><select name=\"";
  page += field;
  page += "\">";
  for (uint8_t i = 0; i <= 16; i++) {
    page += "<option value=\"" + String(i) + "\"" +
            (selected == i ? "selected=\"selected\"" : "") + ">" + String(i) +
            "</option>";
  }
  page += "</select></div>";
  return page;
}

const String AFESitesGenerator::generateHardwareItemsList(
    uint8_t noOfItems, uint8_t noOffConnected, const char *field,
    const char *label) {

  String body = "<div class=\"cf\"><label>";
  body += label;
  body += "</label>";
  body += "<select name=\"";
  body += field;
  body += "\">";
  body += "<option value=\"0\"";
  body += (noOffConnected == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "Brak" : "None";
  body += "</option>";

  for (uint8_t i = 1; i <= noOfItems; i++) {
    body += "<option value=\"";
    body += i;
    body += "\"";
    body += noOffConnected == i ? " selected=\"selected\"" : "";
    body += ">";
    body += i;
    body += "</option>";
  }

  body += "</select>";
  body += "</div>";

  return body;
}

const String
AFESitesGenerator::generateRegulatorController(REGULATOR configuration,
                                               boolean isThermostat) {

  String body = "<fieldset>";

  body += "<div class=\"cc\">";
  body += "<label>";
  body += "<input name=\"te\" type=\"checkbox\" value=\"1\"";
  body += configuration.enabled ? " checked=\"checked\">" : ">";

  body += language == 0 ? " włączony" : "enabled";

  body += "?</label>";
  body += "</div>";

  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Włącz jeśli " : "Switch on if ";
  body += "temp.";
  body += language == 0 ? " jest" : " is";
  body += "</label>";

  body += "<select name=\"ta\"><option value=\"0\"";
  body += (configuration.turnOnAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOnAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<span> ";
  body += language == 0 ? "od" : "from";
  body += " </span>";
  body += "<input name=\"tn\" type=\"number\" value=\"";
  body += configuration.turnOn;
  if (isThermostat) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (isThermostat) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }
  body += "</span></div>";
  body += "<div class=\"cf\">";
  body += "<label>";
  body += language == 0 ? "Wyłącz jeśli " : "Switch off if ";
  body += "temp.";
  body += language == 0 ? " jest" : " is";
  body += "</label>";
  body += "<select name=\"tb\"><option value=\"0\"";
  body += (configuration.turnOffAbove == 0 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "mniejsza" : "below";
  body += "</option>";
  body += "<option value=\"1\"";
  body += (configuration.turnOffAbove == 1 ? " selected=\"selected\"" : "");
  body += ">";
  body += language == 0 ? "większa" : "above";
  body += "</option>";
  body += "</select>";
  body += "<span> ";
  body += language == 0 ? "od" : "from";
  body += " </span>";
  body += "<input name=\"tf\" type=\"number\" value=\"";
  body += configuration.turnOff;
  if (isThermostat) {
    body += "\" min=\"-55\" max=\"125\"";
  } else {
    body += "\" min=\"0\" max=\"100\"";
  }
  body += "step=\"any\"><span class=\"hint\">";
  body += language == 0 ? "Zakres" : "Range";
  if (isThermostat) {
    body += ": -55&deg;C : +125&deg;C (-67&deg;F : +260&deg;F)";
  } else {
    body += ": 0% : 100%";
  }
  body += "</div></fieldset>";

  return body;
}
