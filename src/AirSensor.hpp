#include <dht.h>
#define DHT_PIN A3

class AirSensor {
private:
  dht DHT;
  float air_temp;
  unsigned long tempf_last_read;


public:
  AirSensor();

  void loop();
  float getAirTemp();

private:
  void readTemp();
};
