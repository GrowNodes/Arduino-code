#include <Arduino.h>
#include <SPI.h>
#include <PJON.h>
#include <dht.h>
// #define GROW_LIGHT_PIN 8
#define MCU_BUS_PIN 12
#define ESP_BUS_ID 44

dht DHT;
#define DHT11_PIN 5



PJON<SoftwareBitBang> MCUBus(45);   // device ID 45
float air_temp_f = 0;
unsigned long tempf_last_read = 0;




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
    for(uint16_t i = 0; i < length; ++i) {
      payload_str += (char)payload[i];
    }

    Serial.print("Received ");
    Serial.print(length);
    Serial.print(" bytes: ");
    Serial.println(payload_str);

    // if (payload_str == "water_level") {
    //   String reply_str = "water_level=1.69";
    //   Serial.print("Responding with: ");
    //   Serial.println(reply_str.c_str());
    //   MCUBus.reply(reply_str.c_str(), reply_str.length());
    //   // return;
    // }

    // if (payload_str == "grow_light_on") {
      // Serial.println("turn grow light on");
      // MCUBus.reply("ok", 2);
      // return;
    // }

    // if (payload_str == "grow_light_off") {
      // Serial.println("turn grow light off");
      // MCUBus.reply("ok", 2);
      // return;
    // }
    //
    if (payload_str == "air_temp_f") {
      // For some reason I couldn't get String to work
      // Also fuck arduino's sprintf doesn't support floats

      float tempf = isnan(air_temp_f) ? 0.0 : air_temp_f;
      char myConcatenation[20];

      sprintf(myConcatenation,"air_temp_f=%i.0", (int)round(tempf));
      Serial.print("Replying with: ");
      Serial.println(myConcatenation);

      MCUBus.reply(myConcatenation, 13);
      return;
    }
  });

};



void readTemp()
{
  // READ DATA
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
		break;
    case DHTLIB_ERROR_CHECKSUM:
		Serial.print("DHT11 Checksum error,\t");
		break;
    case DHTLIB_ERROR_TIMEOUT:
		Serial.print("DHT11 Time out error,\t");
		break;
    default:
		Serial.print("DHT11 Unknown error,\t");
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
  MCUBus.receive((uint32_t)5000);

  if (millis() - tempf_last_read >= 2000) {
    tempf_last_read = millis();
    readTemp();
    Serial.print("air_temp_f: ");
    Serial.println(air_temp_f);
    // air_temp_f = dht.readTemperature(true);
  }
};
