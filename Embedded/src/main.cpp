
#include "serialcmd/test/MockStream.hpp"
#include "serialcmd/StreamBinarySerializer.hpp"

#include <Arduino.h>


uint8_t input_buffer[8], output_buffer[8];

using serialcmd::core::MemIO;

serialcmd::test::MockStream stream(
    MemIO(input_buffer, sizeof(input_buffer)),
    MemIO(output_buffer, sizeof(output_buffer))
);

serialcmd::StreamBinarySerializer serializer(stream);

void MemIO_print(const MemIO &mem) {
    for (uint8_t *i = const_cast<uint8_t *>(mem.begin); i < mem.end; i++) {
        Serial.print((size_t) i);
        Serial.write(':');
        Serial.write('\t');
        Serial.println(*i, HEX);
    }

    Serial.print(F("Available:\t"));
    Serial.println(mem.getAvailableSize());
    Serial.print(F("Cursor:\t"));
    Serial.println((size_t) mem.cursor);
}


void setup() {
    Serial.begin(115200);

    struct Foo {
        uint32_t a;
        uint16_t b;
        uint8_t c;
    } foo{
        .a = 0xA1A2A3A4,
        .b = 0xB1B2,
        .c = 0x69
    };

    serializer.write(foo);

    MemIO_print(stream.output);
}

void loop() {}