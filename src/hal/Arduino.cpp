#include "Arduino.h"
#include <chrono>

ASerial Serial;
int digitalPinToInterrupt(int pin) {
    // 在 wiringPi 中，GPIO 编号与数字引脚一致
    return pin;
}

void attachInterrupt(int pin, void (*ISR)(void), int mode) {
    // 使用 wiringPi 的中断函数
    wiringPiISR(pin, mode, ISR);
}

unsigned long pulseIn(int pin, int value, unsigned long timeout) {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    while (digitalRead(pin) != value) {
        if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() > timeout) {
            return 0;
        }
        end = std::chrono::high_resolution_clock::now();
    }

    start = std::chrono::high_resolution_clock::now();
    while (digitalRead(pin) == value) {
        if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() > timeout) {
            return 0;
        }
        end = std::chrono::high_resolution_clock::now();
    }

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void noInterrupts() {

}

void interrupts() {

}