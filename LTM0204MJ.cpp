#include "LTM0204MJ.h"

static const uint8_t digits[] = {
    0b0111111,
    0b0000110,
    0b1011011,
    0b1001111,
    0b1100110,
    0b1101101,
    0b1111101,
    0b0000111,
    0b1111111,
    0b1101111,
};

LTM0204MJ::LTM0204MJ(PinName mosi, PinName miso, PinName sck, PinName stb, int spi_frequency)
    : m_display(mosi, miso, sck, stb, spi_frequency),
      m_dimming(PT6955::PulseWidth_1_16) {
    clear();
}

void LTM0204MJ::initialize() {
    m_display.initialize(PT6955::Display_5_Digits_11_Segments, true, m_dimming);
}

void LTM0204MJ::setBrightness(uint8_t brightness) {
    m_display.command4(true, (PT6955::PulseWidth)brightness);
}

void LTM0204MJ::clear() {
    memset(m_buffer, 0, sizeof(m_buffer));
}

void LTM0204MJ::setRecord(bool onoff) {
    m_buffer[4 * 2] = (m_buffer[4 * 2] & ~0b01) | onoff;
}

void LTM0204MJ::setColon(bool onoff) {
    m_buffer[2 * 2 + 1] = onoff ? 0b11 : 0;
}

void LTM0204MJ::setAM(bool onoff) {
    m_buffer[1 * 2 + 1] = (onoff << 1);
}

void LTM0204MJ::setTimer(bool onoff) {
    m_buffer[4 * 2] = (m_buffer[4 * 2] & ~0b10) | (onoff << 1);
}

void LTM0204MJ::setVCR(bool onoff) {
    m_buffer[0 * 2 + 1] = onoff;
}

void LTM0204MJ::setHiFi(bool onoff) {
    m_buffer[3 * 2] = (m_buffer[3 * 2] & ~0b10000000) | (onoff << 7);
}

void LTM0204MJ::setDigit(uint8_t position, uint8_t digit) {
    assert(position <= 3);
    assert(digit <= 9);
    m_buffer[position * 2] = digits[digit];
}

void LTM0204MJ::clearDigit(uint8_t position) {
    assert(position <= 3);
    m_buffer[position * 2] = 0;
}

uint8_t &LTM0204MJ::digit(uint8_t position) {
    assert(position <= 3);
    return m_buffer[position * 2];
}

void LTM0204MJ::update() {
    m_display.command3(0, m_buffer, sizeof(m_buffer));
}

