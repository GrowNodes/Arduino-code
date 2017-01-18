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

void payloadRouter(const char* payload_str) {

  if (strcmp_P(payload_str, (PGM_P)F("water_level")) == 0) {
    char reply_str[] = "water_level=1.69";
    Serial.print(F("Responding with: "));
    Serial.println(reply_str);
    MCUBus.reply(reply_str, sizeof(reply_str));
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light_on")) == 0) {
    Serial.println(F("turn grow light on"));
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light_off")) == 0) {
    Serial.println(F("turn grow light off"));
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("air_temp_f")) == 0) {
    // For some reason I couldn't get String to work
    // Also fuck arduino's sprintf doesn't support floats

    float tempf = isnan(air_temp_f) ? 0.0 : air_temp_f;
    char myConcatenation[20];

    sprintf(myConcatenation,"air_temp_f=%i.0", (int)round(tempf));
    Serial.print(F("Replying with: "));
    Serial.println(myConcatenation);

    MCUBus.reply(myConcatenation, 13);
    return;
  }
}

void onBusPacket(uint8_t *payload, uint16_t length, const PacketInfo &packet_info) {
  char payload_str[length+1];  // +1 for null terminator
  for(uint16_t i = 0; i < length; ++i) {
    payload_str[i] = (char)payload[i];
  }
  payload_str[length] = '\0'; // null terminate the string

  Serial.print(F("Received "));
  Serial.print(length);
  Serial.print(F(" bytes: "));
  Serial.println(payload_str);

  payloadRouter(payload_str);
}

void setup() {
  Serial.begin(74880);
  Serial.println(F("Ready to receive"));
//   // pinMode(GROW_LIGHT_PIN, OUTPUT);
//
//
  MCUBus.strategy.set_pin(MCU_BUS_PIN);
  MCUBus.begin();
  MCUBus.set_receiver(onBusPacket);

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
		Serial.print(F("DHT11 Checksum error,\t"));
		break;
    case DHTLIB_ERROR_TIMEOUT:
		Serial.print(F("DHT11 Time out error,\t"));
		break;
    default:
		Serial.print(F("DHT11 Unknown error,\t"));
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
    Serial.print(F("air_temp_f: "));
    Serial.println(air_temp_f);
    // air_temp_f = dht.readTemperature(true);
  }
};
