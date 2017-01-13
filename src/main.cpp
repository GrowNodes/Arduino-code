#include <Arduino.h>
#include <SPI.h>
#include <PJON.h>
PJON<SoftwareBitBang> MCUBus(45);


constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}



void set_grow_light(bool on) {
  if (on) {
    Serial.println("turn grow light on");
    MCUBus.reply("ok", 2);
    return;
  }
  Serial.println("turn grow light off");
  MCUBus.reply("ok", 2);
}

void payload_router(const char* payload) {
  switch (str2int(payload)) {
    case str2int("grow_light_on"): {
      set_grow_light(true);
      break;
    }

    case str2int("grow_light_off"): {
      set_grow_light(false);
      break;
    }

    case str2int("air_temp_f"): {
      String reply_str = "air_temp_f=";
      reply_str += (float)random(6500, 9900)*0.01;
      MCUBus.reply(reply_str.c_str(), reply_str.length());
      break;
    }

    case str2int("water_level"): {
      String reply_str = "water_level=";
      reply_str += (float)random(100, 300)*0.01;
      MCUBus.reply(reply_str.c_str(), reply_str.length());
      break;
    }
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

void setup() {

  MCUBus.strategy.set_pin(13);
  MCUBus.begin();
  MCUBus.set_receiver(receiver_function);

  Serial.begin(9600);
  Serial.println("Ready to receive");
};



void loop() {
  MCUBus.update();
  MCUBus.receive(1000);
};
