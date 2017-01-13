#include <Arduino.h>
#include <SPI.h>
#include <PJON.h>

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}



void set_grow_light(bool on) {
  if (on) {
    Serial.println("turn grow light on");
    return;
  }
  Serial.println("turn grow light off");
}

void payload_router(const char* payload) {
  switch (str2int(payload)) {
    case str2int("grow_light_on"):
      set_grow_light(true);
      break;

    case str2int("grow_light_off"):
      set_grow_light(false);
      break;
  }
}

void receiver_function(uint8_t *payload, uint16_t length, const PacketInfo &packet_info) {

  String payload_str;
   for(uint16_t i = 0; i < length; ++i)
      payload_str += (char)payload[i];

  Serial.print("Received ");
  Serial.print(length);
  Serial.print(" bytes: ");
  Serial.println(payload_str);

  payload_router(payload_str.c_str());
}

// <Strategy name> bus(selected device id)
PJON<SoftwareBitBang> bus(45);

void setup() {
  pinModeFast(12, OUTPUT);
  digitalWriteFast(12, LOW); // Initialize LED 12 to be off

  bus.strategy.set_pin(13);
  bus.begin();
  bus.set_receiver(receiver_function);

  Serial.begin(9600);
  Serial.println("Ready to receive");
};



void loop() {
  bus.receive(1000);
};
