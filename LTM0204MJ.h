#include "PT6955.h"

/*

|-| |---| |---|   |---| |---|
    |   | | | | . |   | |   |
    |---| |---|   |---| |---|    TIMER
    | / | | | | . | | | |   | A  VCR
    |---| |---|   |---| |---| M  Hi-Fi

            ^
            these two verticals do not work for me,
            possibly not wired

    1
  |---|
6 | 7 | 2
  |---|
5 |   | 3
  |---|
    4
*/

class LTM0204MJ {
    public:
    LTM0204MJ(PinName mosi, PinName miso, PinName sck, PinName stb, int spi_frequency = PT6955::DEFAULT_SPI_FREQ);

    void initialize();

    // set brightness, valid values are [0-7]
    void setBrightness(uint8_t brightness);

    // turn all leds off, must update to take effect
    void clear();

    void setRecord(bool onoff);
    void setColon(bool onoff);
    void setAM(bool onoff);
    void setTimer(bool onoff);
    void setVCR(bool onoff);
    void setHiFi(bool onoff);

    // set one of the four digits, position = [0-3], digit = [0-9]
    void setDigit(uint8_t position, uint8_t digit);
    // turn all segments off at digit position
    void clearDigit(uint8_t position);

    // get the raw buffer for a digit [0-3]
    uint8_t &digit(uint8_t position);

    // update the screen
    void update();

    private:
    PT6955 m_display;
    uint8_t m_buffer[10];

    PT6955::PulseWidth m_dimming;
};

