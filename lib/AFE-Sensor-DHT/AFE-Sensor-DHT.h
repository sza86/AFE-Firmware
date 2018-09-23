/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sensor_DHT_h
#define _AFE_Sensor_DHT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-Data-Access.h>
#include <DHT.h>
// #include <Streaming.h>

#define IDX_TYPE_TEMPERATURE 0
#define IDX_TYPE_HUMIDITY 1
#define IDX_TYPE_TEMPERATURE_AND_HUMIDITY 2

class AFESensorDHT {

private:
  DH configuration;
  float currentTemperature = -127;
  float currentHumidity = -1;
  boolean temperatureInBuffer = false;
  boolean humidityInBuffer = false;
  unsigned long temperatureCounterStartTime = 0;
  unsigned long humidityCounterStartTime = 0;
  boolean _initialized = false;

public:
  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorDHT();

  void begin();

  /* Method returns temperature */
  float getTemperature();

  /* Method returns humidity */
  float getHumidity();

  /* Method returns latest stored in memory temperature */
  float getLatestTemperature();

  /* Method returns latest stored in memory humidity */
  float getLatestHumidity();

  /* Method returns heat index */
  float getHeatIndex();

  /* It returns true if temperature has been read from the sensor */
  boolean temperatureSensorReady();

  /* It returns true if humidity has been read from the sensor */
  boolean humiditySensorReady();

  /* Method should be added to the main loop to check temperature / humidity in
   * defined time frame */
  void listener();

  /* Method returns sensor IDX for temperature, humidity, temperature and
   * humidity */
  unsigned long getDomoticzIDX(uint8_t type);

  /* Get HeatIndex publishing configuration item */
  boolean publishHeatIndex();
};

#endif
