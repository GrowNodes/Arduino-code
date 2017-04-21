#include <Arduino.h>
#include <Wire.h>
#include <I2C_Anything.h>



#define I2C_BUS_ID 45
// #define WATER_PUMP_PIN 5
#define GROW_LIGHT_PIN 13
#define AIR_SENSOR_PIN D2
#define PELTIER_PIN 9


void send() {
  I2C_writeAnything(1234.12);
}


void receive(int howManyBytes) {
  byte selectedDevice = Wire.read(); // first byte of the transmission is the device selector

  switch (selectedDevice) {
    case PELTIER_PIN:
      analogWrite(PELTIER_PIN, Wire.read()); // pwm
      break;
    case GROW_LIGHT_PIN:
      bool newState = Wire.read();    // receive byte as boolean
      digitalWrite(GROW_LIGHT_PIN, newState);
      break;
  }

}


void setup(/* arguments */) {
  pinMode(PELTIER_PIN, OUTPUT);
  pinMode(GROW_LIGHT_PIN, OUTPUT);
  Wire.begin(I2C_BUS_ID);                // join i2c bus with address #8
  Wire.onReceive(receive); // master wrtier slave reader
  Wire.onRequest(send); // master reader slave writer

  Serial.begin(74880);
}

void loop() {
}
