#include <Arduino.h>
#include <SPI.h>
#include <PJON.h>
#include <dht.h>
// #define GROW_LIGHT_PIN 8
#define MCU_BUS_PIN 12
//

dht DHT;
#define DHT11_PIN 5



PJON<SoftwareBitBang> MCUBus(45);   // device ID 45
float air_temp_f = 0;
unsigned long tempf_last_read = 0;
//
//
//
void setup() {
  Serial.begin(74880);
  Serial.println("Ready to receive");
//   // pinMode(GROW_LIGHT_PIN, OUTPUT);
//
//
  MCUBus.strategy.set_pin(MCU_BUS_PIN);
  MCUBus.begin();
  MCUBus.set_receiver([](uint8_t *payload, uint16_t length, const PacketInfo &packet_info) {

    String payload_str;
     for(uint16_t i = 0; i < length; ++i)
        payload_str += (char)payload[i];

    Serial.print("Received ");
    Serial.print(length);
    Serial.print(" bytes: ");
    Serial.println(payload_str);

  });

};



void readTemp()
{
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
		break;
    case DHTLIB_ERROR_CHECKSUM:
		Serial.print("Checksum error,\t");
		break;
    case DHTLIB_ERROR_TIMEOUT:
		Serial.print("Time out error,\t");
		break;
    default:
		Serial.print("Unknown error,\t");
		break;
  }
  // DISPLAY DATA
  // Serial.print(DHT.humidity, 1);
  // Serial.print(",\t");
  // Serial.println(DHT.temperature, 1);
  air_temp_f = DHT.temperature;
}






void loop() {
  MCUBus.update();
  MCUBus.receive(1000);


  if (millis() - tempf_last_read >= 2000) {
    tempf_last_read = millis();
    readTemp();
    Serial.print("air_temp_f: ");
    Serial.println(air_temp_f);
    // air_temp_f = dht.readTemperature(true);
  }
};
