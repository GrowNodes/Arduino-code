#include <main.h>
#include <dht.h>


struct AirSensorData air_sensor_data;
byte selectedId;
byte payload;

dht DHT;


// is alignment an issue?
void struct_to_byte_2(byte *a) {
    for (unsigned int i = 0; i < sizeof(air_sensor_data); i++) {
        a[i] = *((byte *)(&air_sensor_data) + i);
        //cout << "in array: " << i << ":" << a[i] << endl;
    }
}

// // less robust? dependent on same architecture?
// void struct_to_byte(char *a)
// {
//     memcpy(a, &air_sensor_data, sizeof(air_sensor_data));
// }
// void byte_to_struct(char *a)
// {
//     struct AirSensorData temp;
//     memcpy(&temp, a, sizeof(temp));
//     cout << "temperature:" << temp.temperature << endl;
//     cout << "humidity:" << temp.humidity << endl;
//     cout << "last_read:" << temp.last_read << endl;
//     cout << "last_read_success:" << temp.last_read_success << endl;
// }





void readAirSensor() {
  air_sensor_data.last_read = 1234;
  air_sensor_data.last_read_success = true;
  air_sensor_data.temperature = 74.4;
  air_sensor_data.humidity = 50.0;
  return;

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
      // send air_sensor_data over I2C
      const int struct_size = 13;
      byte buf[struct_size];
      struct_to_byte_2(buf);
      Wire.write(buf, struct_size);
      break;
  }
}

void receiveISR(int howManyBytes) {
  selectedId = Wire.read(); // first byte of the transmission is the selector
  payload = Wire.read();

  switch (selectedId) {
    case PELTIER_FAN:
      digitalWrite(PELTIER_FAN, payload);
      break;
    case PELTIER:
      analogWrite(PELTIER, payload); // pwm
      break;
    case GROW_LIGHT:
      digitalWrite(GROW_LIGHT, (bool)payload);
      break;
    case WATER_PUMP:
      digitalWrite(WATER_PUMP, (bool)payload);
      break;
  }

}


void setup(/* arguments */) {
  pinMode(GROW_LIGHT, OUTPUT);
  pinMode(PELTIER, OUTPUT);
  pinMode(PELTIER_FAN, OUTPUT);
  pinMode(WATER_PUMP, OUTPUT);

  Wire.begin(HWC_I2C_ID);
  Wire.onReceive(receiveISR); // interrupt. master wrtier slave reader
  Wire.onRequest(sendISR); // interrupt. master reader slave writer

  readAirSensor();

  Serial.begin(74880);
  Serial.println("ready");
}

void loop() {
  if (selectedId && payload) {
    Serial.print("selectedId: ");
    Serial.print(selectedId);
    Serial.print(", payload: ");
    Serial.println(payload);

    selectedId = 0;
    payload = 0;
  }


  if (millis() - air_sensor_data.last_read > 2000) {
    readAirSensor();
  }
}
