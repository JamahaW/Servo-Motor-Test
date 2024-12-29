#include "servomotor/drivers/L293Driver.hpp"
#include "servomotor/encoders/GA25Encoder.hpp"
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
        serialcmd::i8 speed = test_encoder.getSpeed();
        serializer.write(Result::ok);
        serializer.write(speed);
    }

    typedef void(*Cmd)(StreamSerializer &);

    Cmd commands[]{
        setMotorSpeed,
        getEncoderPosition,
        getEncoderSpeed,
    };
}

serialcmd::Protocol<serialcmd::u8, serialcmd::u8> protocol(
    cmd::commands,
    3,
    Serial
);

void setup() {
    Serial.begin(115200);

    protocol.begin(0x01);
}

void loop() {
    protocol.pull();
}