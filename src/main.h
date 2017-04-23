#include <Arduino.h>
#include <Wire.h>

// start environment defines/types
// TODO refactor to git submodule for ESP
#define I2C_BUS_ID 45

// Pin assignments
#define GROW_LIGHT 13
#define PELTIER 9
#define WATER_PUMP 5
#define AIR_SENSOR 2

struct AirSensorData {
  float temperature;
  float humidity;
  unsigned long last_read;
  bool last_read_success;
};
// end environment defines/types
