#include <main.h>
#include <dht.h>


struct AirSensorData air_sensor_data;
byte selectedId;
byte lastSelectedId;
dht DHT;

void readAirSensor() {
  int chk = DHT.read22(AIR_SENSOR);
  air_sensor_data.last_read = millis();
  air_sensor_data.last_read_success = false;

  switch (chk) {
    case DHTLIB_OK:
    air_sensor_data.temperature = DHT.temperature * 1.8 + 32;
    air_sensor_data.humidity = DHT.humidity;
    air_sensor_data.last_read_success = true;

    Serial.print("DHT OK,\t");
    Serial.print("Temperature: ");
    Serial.print(air_sensor_data.temperature);
    Serial.print(" humidity: ");
    Serial.println(air_sensor_data.humidity);
    break;
    case DHTLIB_ERROR_CHECKSUM:
    Serial.print("DHT Checksum error,\t");
    break;
    case DHTLIB_ERROR_TIMEOUT:
    Serial.print("DHT Time out error,\t");
    break;
    default:
    Serial.print("DHT Unknown error,\t");
    break;
  }
}

void sendISR() {
  switch (selectedId) {
    case AIR_SENSOR:
      // TODO send air_sensor_data over I2C
      break;
  }
}

void receiveISR(int howManyBytes) {
  selectedId = Wire.read(); // first byte of the transmission is the selector

  switch (selectedId) {
    case PELTIER:
      analogWrite(PELTIER, Wire.read()); // pwm
      break;
    case GROW_LIGHT:
      digitalWrite(GROW_LIGHT, (bool)Wire.read());
      break;
  }

}


void setup(/* arguments */) {
  pinMode(PELTIER, OUTPUT);
  pinMode(GROW_LIGHT, OUTPUT);
  Wire.begin(I2C_BUS_ID);
  Wire.onReceive(receiveISR); // interrupt. master wrtier slave reader
  Wire.onRequest(sendISR); // interrupt. master reader slave writer

  readAirSensor();

  Serial.begin(74880);
}

void loop() {
  if (selectedId != lastSelectedId) {
    lastSelectedId = selectedId;
    Serial.print("selectedId: ");
    Serial.println(selectedId);
  }


  if (millis() - air_sensor_data.last_read > 2000) {
    readAirSensor();
  }
}
