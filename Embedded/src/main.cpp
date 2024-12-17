#include "servomotor/drivers/MockDriver.hpp"
#include "servomotor/encoders/MockEncoder.hpp"
#include "servomotor/ServoMotor.hpp"

#include <Arduino.h>


auto driver = servomotor::drivers::MockDriver();

auto encoder = servomotor::encoders::MockEncoder();

void setup() {
    Serial.begin(115200);
}


void loop() {
    encoder.current_position += 80;
    delay(10);

//    static servomotor::core::Chronometer chronometer;

    Serial.println(encoder.getSpeed());
}
