#include <Arduino.h>
#include <SPI.h>
#include <PJON.h>
#include <AirSensor.cpp>
#include <LiquidCrystal.h>

#define WATER_PUMP_PIN 5
#define GROW_LIGHT_PIN 8
#define MCU_BUS_PIN 9
#define ESP_BUS_ID 44

AirSensor air_sensor;
PJON<SoftwareBitBang> MCUBus(45);   // device ID 45

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
    digitalWrite(WATER_PUMP_PIN, HIGH);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("water_pump=off")) == 0) {
    Serial.println(F("turn water pump off"));
    digitalWrite(WATER_PUMP_PIN, LOW);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light=on")) == 0) {
    Serial.println(F("turn grow light on"));
    digitalWrite(GROW_LIGHT_PIN, HIGH);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("grow_light=off")) == 0) {
    Serial.println(F("turn grow light off"));
    digitalWrite(GROW_LIGHT_PIN, LOW);
    MCUBus.reply("ok", 2);
    return;
  }

  if (strcmp_P(payload_str, (PGM_P)F("air_temp")) == 0) {
    // For some reason I couldn't get String to work
    // Also fuck arduino's sprintf doesn't support floats

    // air_temp=-999.99;

    char air_temp_str_buffer[6+1];   // 102.76 + null terminator
    dtostrf(air_sensor.getAirTemp(), 4, 2, air_temp_str_buffer);
    char reply_str[25];
    sprintf(reply_str,"air_temp=%s", air_temp_str_buffer);

    Serial.print(F("Replying with "));
    Serial.print(strlen(reply_str));
    Serial.print(F(" bytes: "));
    Serial.println(reply_str);
    MCUBus.reply(reply_str, strlen(reply_str));
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

//                RS, E,  D4,D5,D6,D7
LiquidCrystal lcd(12, 13, 4, 7, 3, 2);


void setup() {
  Serial.begin(74880);
  Serial.println(F("Ready to receive\n"));
  pinMode(GROW_LIGHT_PIN, OUTPUT);


  lcd.begin(16, 2);
  lcd.print("Start");
  lcd.setCursor(0, 1);
  // lcd.print("pHRaw");



  MCUBus.strategy.set_pin(MCU_BUS_PIN);
  MCUBus.begin();
  MCUBus.set_receiver(onBusPacket);

};





void loop() {
  air_sensor.loop();

  MCUBus.update();
  MCUBus.receive((uint32_t)5000);
};
