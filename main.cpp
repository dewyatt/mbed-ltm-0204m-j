#include <mbed.h>
#include "LTM0204MJ.h"

// spinning animation on all digits
void example1(LTM0204MJ &display, bool &done) {
    for (int i = 0; i < 6; i++) {
        display.digit(0) = 1 << i;
        display.digit(1) = 1 << i;
        display.digit(2) = 1 << i;
        display.digit(3) = 1 << i;
        display.update();
        ThisThread::sleep_for(200ms);
    }
    if (done) {
        return;
    }
}

// count from [0, 9999]
void example2(LTM0204MJ &display, bool &done) {
    uint8_t digits[4];
    for (int i = 0; i <= 9999; i++) {
        // extract the number to its individual digits
        memset(digits, 0, sizeof(digits));
        uint8_t numdigits = 0;
        int num = i;
        while (num > 9) {
            digits[numdigits++] = num % 10;
            num /= 10;
        }
        digits[numdigits++] = num;
        // display the digits
        for (int n = numdigits - 1; n >= 0; n--) {
            display.setDigit(numdigits - n - 1, digits[n]);
        }
        // update the screen
        display.update();
        wait_us(1000 * 10);
        if (done) {
            return;
        }
    }
}

// outline animation around digits
void example3(LTM0204MJ &display, bool &done) {
    for (int i = 0; i < 4; i++) {
        display.clear();
        display.digit(i) = 1;
        display.update();
        ThisThread::sleep_for(100ms);
    }
    for (int i = 0; i < 2; i++) {
        display.clear();
        display.digit(3) = 1 << (1 + i);
        display.update();
        ThisThread::sleep_for(100ms);
    }
    for (int i = 0; i < 4; i++) {
        display.clear();
        display.digit(3 - i) = 1 << 3;
        display.update();
        ThisThread::sleep_for(100ms);
    }
    for (int i = 0; i < 2; i++) {
        display.clear();
        display.digit(0) = 1 << (4 + i);
        display.update();
        ThisThread::sleep_for(100ms);
    }
}

// bouncing horizontal 3 line animation on digits
void example4(LTM0204MJ &display, bool &done) {
    for (int i = 0; i < 4; i++) {
        display.clear();
        display.digit(i) = 1 | (1 << 6) | (1 << 3);
        display.update();
        ThisThread::sleep_for(70ms);
    }
    for (int i = 3; i >= 0; i--) {
        display.clear();
        display.digit(i) = 1 | (1 << 6) | (1 << 3);
        display.update();
        ThisThread::sleep_for(70ms);
    }
}

// spinner on top and bottom half of digits
void example5(LTM0204MJ &display, bool &done) {
    static const uint8_t bitsupper[] = {0, 1, 6, 5};
    static const uint8_t bitslower[] = {6, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        display.clear();
        display.digit(0) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(1) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(2) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(3) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.update();
        ThisThread::sleep_for(200ms);
    }
}

// offset spinning animation on top and bottom half of digits
void example6(LTM0204MJ &display, bool &done) {
    static const uint8_t bitsupper[] = {0, 1, 6, 5};
    static const uint8_t bitslower[] = {3, 4, 6, 2};
    for (int i = 0; i < 4; i++) {
        display.clear();
        display.digit(0) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(1) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(2) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.digit(3) = (1 << bitsupper[i]) | (1 << bitslower[i]);
        display.update();
        ThisThread::sleep_for(200ms);
    }
}

// flash icons at different rates
void example7(LTM0204MJ &display, bool &done) {
    Thread t1;
    Mutex mutex;

    t1.start([&]() {
        bool toggle = true;
        while (!done) {
            {
                mutex.lock();
                display.setRecord(toggle);
                display.setTimer(!toggle);
                display.update();
                mutex.unlock();
            }
            ThisThread::sleep_for(200ms);
            toggle = !toggle;
        }
    });
    bool toggle = true;
    while (!done) {
        {
            mutex.lock();
            display.setColon(toggle);
            display.update();
            mutex.unlock();
        }
        ThisThread::sleep_for(1s);
        toggle = !toggle;
    }
    t1.join();
}

static const std::function<void(LTM0204MJ&, bool&)> examples[] = {
    example1,
    example2,
    example3,
    example4,
    example5,
    example6,
    example7
};

int main() {
    LTM0204MJ display(p11, p12, p13, p8);
    display.initialize();

    while (true) {
        for (auto &example: examples) {
            display.clear();

            bool done = false;
            Thread thread;
            thread.start([&] {
                while (!done) {
                    display.clear();
                    example(display, done);
                }
            });
            ThisThread::sleep_for(2s);
            done = true;
            thread.join();
        }
    }
}

