#include <main.h>

struct AirSensorData air_sensor_data;
byte selectedId;
byte lastSelectedId;

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

  // air_sensor_data.temperature = TODO;
  // air_sensor_data.humidity = TODO;
  air_sensor_data.last_read = millis();

  Serial.begin(74880);
}

void loop() {
  if (selectedId != lastSelectedId) {
    lastSelectedId = selectedId;
    Serial.print("selectedId: ");
    Serial.println(selectedId);
  }
  // if (millis() - air_sensor_data.last_read > 2000) {
  //   // air_sensor_data.temperature = TODO;
  //   // air_sensor_data.humidity = TODO;
  //   air_sensor_data.last_read = millis();
  // }
}
