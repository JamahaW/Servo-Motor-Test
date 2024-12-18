
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
    MemIO_print(stream.output);

    for (uint8_t i = 0; i < 10; i++) {
        stream.write(i);
    }

    MemIO_print(stream.output);

}

void loop() {

}