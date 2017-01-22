#include <MCUBus.hpp>
#include <dht.h>
#include <constants.h>

class AirSensor {
private:
  dht DHT;
  float air_temp;
  float air_humidity;
  unsigned long air_sensor_last_read;


public:
  AirSensor();

  void loop();
  void send();

private:
  void readSensor();
  float getAirTemp();
  float getAirHumidity();
};
