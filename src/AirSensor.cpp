#include <AirSensor.hpp>
AirSensor::AirSensor():
DHT()
{
  air_temp = 0;
  air_sensor_last_read = 0;
}

void AirSensor::loop() {
  if (millis() - air_sensor_last_read >= 3000) {
    air_sensor_last_read = millis();
    readSensor();
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
/////////////////////////////////
  char air_humidity_str_buffer[6+1];   // 102.76 + null terminator
  dtostrf(getAirHumidity(), 4, 2, air_humidity_str_buffer);
  char reply_str2[25];
  sprintf(reply_str2,"air_humidity=%s", air_humidity_str_buffer);

  Serial.print(F("Replying with "));
  Serial.print(strlen(reply_str2));
  Serial.print(F(" bytes: "));
  Serial.println(reply_str2);
  MCUBus.reply(reply_str2, strlen(reply_str2));


}

float AirSensor::getAirTemp() {
  return isnan(air_temp) ? 0.0 : air_temp;
}

float AirSensor::getAirHumidity() {
  return isnan(air_humidity) ? 0.0 : air_humidity;
}

void AirSensor::readSensor() {
  // READ DATA
  int chk = DHT.read22(AIR_SENSOR_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
    air_temp = DHT.temperature *9/5 +32;
    air_humidity = DHT.humidity;
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

}
