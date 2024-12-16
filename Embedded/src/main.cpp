#include <Arduino.h>
#include "servomotor/ServoMotor.hpp"
#include "servomotor/MockServo.hpp"


servomotor::Config<int32_t, int8_t> config = {
    .speed_regulator_settings {1.0, 0.1, 0.5, 127},
    .position_regulator_settings {1.0, 1.0, 1.0, 1000000},
    .max_position_error = 5
};

servomotor::MockServoMotor m(config);

void setup() {
}

void loop() {
}
