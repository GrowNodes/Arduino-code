#include <Arduino.h>
#include <MCUBus.hpp>
#include <AirSensor.hpp>
#include <constants.h>


AirSensor air_sensor;
PJON<SoftwareBitBang> MCUBus(THIS_BUS_ID);   // device ID 45

void payloadRouter(const char* payload_str) {

  if (strcmp_P(payload_str, (PGM_P)F("water_level")) == 0) {
    char reply_str[] = "water_level=1.69";
    Serial.print(F("Responding with: "));
    Serial.println(reply_str);
    MCUBus.reply(reply_str, sizeof(reply_str)-1);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("water_pump=on")) == 0) {
    Serial.println(F("turn water pump on"));
    digitalWrite(WATER_PUMP_PIN, LOW);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("water_pump=off")) == 0) {
    Serial.println(F("turn water pump off"));
    digitalWrite(WATER_PUMP_PIN, HIGH);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light=on")) == 0) {
    Serial.println(F("turn grow light on"));
    digitalWrite(GROW_LIGHT_PIN, LOW);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light=off")) == 0) {
    Serial.println(F("turn grow light off"));
    digitalWrite(GROW_LIGHT_PIN, HIGH);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("air_sensor")) == 0) {
    air_sensor.send();
    return;
  }
}

void onBusPacket(uint8_t *payload, uint16_t length, const PacketInfo &packet_info) {
  char payload_str[length+1];  // +1 for null terminator
  for(uint16_t i = 0; i < length; ++i) {
    payload_str[i] = (char)payload[i];
  }
  payload_str[length] = '\0'; // null terminate the string

  Serial.print(F("\nReceived "));
  Serial.print(length);
  Serial.print(F(" bytes: "));
  Serial.println(payload_str);

  payloadRouter(payload_str);
}


void setup() {
  Serial.begin(74880);
  Serial.println(F("Ready to receive\n"));
  pinMode(GROW_LIGHT_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);

  MCUBus.strategy.set_pin(MCU_BUS_PIN);
  MCUBus.begin();
  MCUBus.set_receiver(onBusPacket);

};





void loop() {
  air_sensor.loop();

  MCUBus.update();
  MCUBus.receive((uint32_t)5000);
};
