//
// Created by book on 24-10-22.
//

#ifndef SIMPLEFOC_LINUX_WIRE_H
#define SIMPLEFOC_LINUX_WIRE_H

#include <cstdint>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

class TwoWire {
private:
    int fd;
    uint8_t address;

public:
    TwoWire() : fd(-1), address(0) {}

    void begin() {
        wiringPiSetup();
    }

    void beginTransmission(uint8_t addr) {
        address = addr;
        if (fd != -1) {
            close(fd);
        }
        fd = wiringPiI2CSetup(address);
    }

    uint8_t endTransmission(bool stop = true) {
        return (fd != -1) ? 0 : 4;
    }

    uint8_t requestFrom(uint8_t addr, uint8_t quantity) {
        // 简化实现，实际使用时可能需要更复杂的逻辑
        return quantity;
    }

    size_t write(uint8_t data) {
        if (fd != -1) {
            return (wiringPiI2CWrite(fd, data) != -1) ? 1 : 0;
        }
        return 0;
    }

    int available() {
        // 简化实现
        return 1;
    }

    int read() {
        if (fd != -1) {
            return wiringPiI2CRead(fd);
        }
        return -1;
    }
};

extern TwoWire Wire;

#endif //SIMPLEFOC_LINUX_WIRE_H
