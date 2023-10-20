#include <mbed.h>

class PT6955 {
    public:
    static const int DEFAULT_SPI_FREQ = 500000;

    enum DisplayMode: uint8_t {
        Display_4_Digits_12_Segments = 0b00,
        Display_5_Digits_11_Segments = 0b01,
        Display_6_Digits_10_Segments = 0b10,
        Display_7_Digits_9_Segments  = 0b11,
    };

    enum PulseWidth: uint8_t {
        PulseWidth_1_16  = 0b000,
        PulseWidth_2_16  = 0b001,
        PulseWidth_4_16  = 0b010,
        PulseWidth_10_16 = 0b011,
        PulseWidth_11_16 = 0b100,
        PulseWidth_12_16 = 0b101,
        PulseWidth_13_16 = 0b110,
        PulseWidth_14_16 = 0b111,
    };

    enum DataMode: uint8_t {
        Normal = 0b0,
        Test   = 0b1,
    };

    enum AddressMode: uint8_t {
        Increment = 0b0,
        Fixed     = 0b1,
    };

    PT6955(PinName mosi, PinName miso, PinName sck, PinName stb, int spi_frequency = DEFAULT_SPI_FREQ);

    void initialize(DisplayMode mode, bool display_on = true, PulseWidth dimming = PulseWidth_1_16);

    // Display Mode Setting
    void command1(DisplayMode mode);
    // Data Setting
    void command2(DataMode dataMode, AddressMode addressMode);
    // Address Setting
    void command3(uint8_t address, uint8_t *data, size_t length);
    void clear();
    // Display Control
    void command4(bool display_on, PulseWidth dimming);

    private:
    static const size_t MAX_DATA_BYTES = 14;

    void write(uint8_t byte);

    SPI m_spi;
    DigitalOut m_stb;
};

