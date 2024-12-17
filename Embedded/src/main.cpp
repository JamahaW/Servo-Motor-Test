#include "servomotor/drivers/MockDriver.hpp"
#include "servomotor/encoders/MockEncoder.hpp"
#include "servomotor/ServoMotor.hpp"
#include "servomotor/drivers/L293Driver.hpp"

#include <Arduino.h>


using servomotor::core::PIDSettings;
using servomotor::core::PID;
using servomotor::Position;
using servomotor::Speed;

auto settings = PIDSettings<Position, Speed>{
    .kp = 1.0,
    .ki = 1.0,
    .kd = 1.0,
    .input_range = {
        .min = -100,
        .max = 100
    },
    .output_range = {
        .min = -10,
        .max = 10
    },
    .integral_range = {
        .min = -5,
        .max = 5
    }
};

auto pid = PID<Position, Speed>(settings);

//auto driver = servomotor::drivers::MockDriver();
//auto encoder = servomotor::encoders::MockEncoder();

void setup() {
    Serial.begin(115200);
}


void loop() {

}
