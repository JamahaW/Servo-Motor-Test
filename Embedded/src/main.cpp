//#include "servomotor/ServoMotor.hpp"
#include "servomotor/drivers/L293Driver.hpp"
#include "servomotor/encoders/GA25Encoder.hpp"
#include "servomotor/core/PID.hpp"
#include "serialcmd/Protocol.hpp"
#include "serialcmd/Types.hpp"
#include <Arduino.h>


using servomotor::drivers::L293Driver;
using servomotor::Direction;
using servomotor::encoders::GA25Encoder;

const auto test_driver = L293Driver(6, 7, Direction::reverse);

static void test_encoder_interrupt();

const auto test_encoder = GA25Encoder(3, 9, test_encoder_interrupt, Direction::reverse);

void test_encoder_interrupt() { test_encoder.onEncoderMoved(); }

using servomotor::Speed;

Speed current_speed = 0;

using servomotor::core::PIDSettings;
using servomotor::Position;

servomotor::core::PIDSettings<Speed, int16_t> settings = {
    .kp = 0.0,
    .ki = 0.0,
    .kd = 0.0,
    .input_range={-200, 200},
    .output_range={-255, 255},
    .integral_range={-255, 255}
};


auto speed_pid = servomotor::core::PID<Speed, int16_t>(settings);

namespace cmd {

    using serialcmd::StreamSerializer;

    enum Result : serialcmd::u8 {
        ok = 0x00,
        fail = 0x01,
    };

    void setMotorSpeed(StreamSerializer &serializer) {
        serialcmd::i16 pwm_dir;


        serializer.read(pwm_dir);
        test_driver.setPower(pwm_dir);

        serializer.write(Result::ok);
    }

    void getEncoderPosition(StreamSerializer &serializer) {
        serialcmd::i32 position = test_encoder.getPosition();
        serializer.write(Result::ok);
        serializer.write(position);
    }

    void getEncoderSpeed(StreamSerializer &serializer) {
        serializer.write(Result::ok);
        serializer.write(current_speed);
    }

    void setSpeed(StreamSerializer &serializer) {
        serialcmd::f32 target_speed;
        serializer.read(target_speed);

        speed_pid.setTarget(target_speed);
        serializer.write(Result::ok);
    }

    void updateSpeedPID(StreamSerializer &serializer) {
        struct { float kp, ki, kd; } data;
        serializer.read(data);

        settings.kp = data.kp;
        settings.kd = data.kd;
        settings.ki = data.ki;

        serializer.write(Result::ok);
    }


    typedef void(*Cmd)(StreamSerializer &);

    Cmd commands[]{
        setMotorSpeed,
        getEncoderPosition,
        getEncoderSpeed,
        setSpeed,
        updateSpeedPID,
    };
}

serialcmd::Protocol<serialcmd::u8, serialcmd::u8> protocol(cmd::commands, 5, Serial);

void setup() {
    Serial.begin(115200);

    protocol.begin(0x01);

    delay(100);
}

void loop() {
    protocol.pull();

    static uint32_t t = 0;

    if (millis() > t) {
        t = millis() + 10;
        current_speed = test_encoder.getSpeed();
        int16_t u_power = speed_pid.calc(current_speed);
        test_driver.setPower(u_power);
    }
}