#include <dht.h>
#define DHT_PIN A3

class AirSensor {
private:
  dht DHT;
  float air_temp = 0;
  unsigned long tempf_last_read = 0;


public:
  AirSensor();

  void loop() {
    if (millis() - tempf_last_read >= 2000) {
      tempf_last_read = millis();
      readTemp();
      // Serial.print(F("air_temp: "));
      // Serial.println(air_temp);
    }
  }

  float getAirTemp() {
    return isnan(air_temp) ? 0.0 : air_temp;
  }

  void readTemp()
  {
    air_temp = 0.0; // Set to safe value
    // READ DATA
    int chk = DHT.read22(DHT_PIN);
    switch (chk)
    {
      case DHTLIB_OK:
  		break;
      case DHTLIB_ERROR_CHECKSUM:
  		Serial.println(F("DHT Checksum error"));
      return;
      case DHTLIB_ERROR_TIMEOUT:
  		Serial.println(F("DHT Time out error"));
  		return;
      default:
  		Serial.println(F("DHT Unknown error"));
  		return;
    }
    // DISPLAY DATA
    // Serial.print(DHT.humidity, 1);
    // Serial.print(",\t");
    // Serial.println(DHT.temperature, 1);
    air_temp = DHT.temperature *9/5 +32;
  }


};
