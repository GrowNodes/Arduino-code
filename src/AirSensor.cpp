#include <AirSensor.hpp>
AirSensor::AirSensor():
DHT()
{
  air_temp = 0;
  tempf_last_read = 0;
}

void AirSensor::loop() {
  if (millis() - tempf_last_read >= 2000) {
    tempf_last_read = millis();
    readTemp();
    // Serial.print(F("air_temp: "));
    // Serial.println(air_temp);
  }
}

void AirSensor::send() {
  char air_temp_str_buffer[6+1];   // 102.76 + null terminator
  dtostrf(getAirTemp(), 4, 2, air_temp_str_buffer);
  char reply_str[25];
  sprintf(reply_str,"air_temp=%s", air_temp_str_buffer);

  Serial.print(F("Replying with "));
  Serial.print(strlen(reply_str));
  Serial.print(F(" bytes: "));
  Serial.println(reply_str);
  MCUBus.reply(reply_str, strlen(reply_str));
}

float AirSensor::getAirTemp() {
  return isnan(air_temp) ? 0.0 : air_temp;
}

void AirSensor::readTemp() {
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

  air_temp = DHT.temperature *9/5 +32;
}
