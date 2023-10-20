#include "PT6955.h"

#define RBYTE(b) ((b * 0x0202020202ULL & 0x010884422010ULL) % 1023)

PT6955::PT6955(PinName mosi, PinName miso, PinName sck, PinName stb, int spi_frequency)
    : m_spi(mosi, miso, sck), m_stb(stb) {
    m_stb = 1;
    m_spi.format(8, 3);
    m_spi.frequency(spi_frequency);
}

void PT6955::initialize(DisplayMode mode, bool display_on, PulseWidth dimming) {
    ThisThread::sleep_for(200ms);
    command2(DataMode::Normal, AddressMode::Increment);
    clear();
    command1(mode);
    command4(display_on, dimming);
}

void PT6955::command1(DisplayMode mode) {
    m_stb = 0;
    write(0b00000000 | (mode & 0b11));
    m_stb = 1;
}

void PT6955::command2(DataMode dataMode, AddressMode addressMode) {
    m_stb = 0;
    write(
        0b01000000
        | ((dataMode & 0b1) << 3)
        | ((addressMode & 0b1) << 2));
    m_stb = 1;
}

void PT6955::command3(uint8_t address, uint8_t *data, size_t length) {
    m_stb = 0;
    write(0b11000000 | (address & 0b1111));
    for (size_t i = 0; i < length; i++) {
        write(data[i]);
    }
    m_stb = 1;
}

void PT6955::clear() {
    m_stb = 0;
    write(0b11000000);
    for (size_t i = 0; i < MAX_DATA_BYTES; i++) {
        write(0);
    }
    m_stb = 1;
}

void PT6955::command4(bool display_on, PulseWidth dimming) {
    m_stb = 0;
    write(0b10000000
        | (display_on ? 0b1000 : 0)
        | (dimming & 0b111));
    m_stb = 1;
}

void PT6955::write(uint8_t byte) {
    m_spi.write((uint8_t)RBYTE(byte));
}

