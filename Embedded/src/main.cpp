#include "servomotor/drivers/MockDriver.hpp"
#include "servomotor/encoders/MockEncoder.hpp"

#include <Arduino.h>


auto driver = servomotor::drivers::MockDriver();

auto encoder = servomotor::encoders::MockEncoder();

void setup() {
    Serial.begin(115200);
}


void loop() {
    encoder.current_position += 10L;
    delay(1);

    Serial.println(encoder.getSpeed());
}
