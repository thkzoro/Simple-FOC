#ifndef __ASERIAL_H
#define __ASERIAL_H
#include "wiringSerial.h"

#include <cstdint>
#include <string>
#include "AStream.h"

class ASerial : public Stream {
private:
    int fd;
    int baudRate;
    std::string device;

public:
    ASerial() : fd(-1), baudRate(115200), device("/dev/ttyAMA0") {}

    bool begin(unsigned long baud) {
        baudRate = baud;
        fd = serialOpen(device.c_str(), baudRate);
        return (fd >= 0);
    }

    void end() {
        if (fd >= 0) {
            serialClose(fd);
            fd = -1;
        }
    }

    bool available() {
        return (fd >= 0) ? serialDataAvail(fd) : 0;
    }

    int read() {
        return (fd >= 0) ? serialGetchar(fd) : -1;
    }

    size_t write(uint8_t c) {
        if (fd >= 0) {
            serialPutchar(fd, c);
            return 1;
        }
        return 0;
    }

    size_t write(const uint8_t *buffer, size_t size) {
        if (fd >= 0) {
            for (size_t i = 0; i < size; i++) {
                serialPutchar(fd, buffer[i]);
            }
            return size;
        }
        return 0;
    }

    void setDevice(const std::string& dev) {
        device = dev;
    }
};

extern ASerial Serial;

#endif //__ASERIAL_H
