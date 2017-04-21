#include <main.h>
byte selectedId;
struct AirSensorData air_sensor_data;

void send() {
  switch (selectedId) {
    case AIR_SENSOR:
      // TODO send air_sensor_data over I2C
      break;
  }
}


void receive(int howManyBytes) {
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
  Wire.onReceive(receive); // master wrtier slave reader
  Wire.onRequest(send); // master reader slave writer

  // air_sensor_data.temperature = TODO;
  // air_sensor_data.humidity = TODO;
  air_sensor_data.last_read = millis();

  Serial.begin(74880);
}

void loop() {
  if (millis() - air_sensor_data.last_read > 2000) {
    // air_sensor_data.temperature = TODO;
    // air_sensor_data.humidity = TODO;
    air_sensor_data.last_read = millis();
  }
}
