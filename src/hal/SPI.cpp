#include "SPI.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <stdexcept>

SPIClass SPI;

SPISettings::SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
        : clock(clock), bitOrder(bitOrder), dataMode(dataMode) {}

SPIClass::SPIClass() : fd(-1), device("/dev/spidev0.0"), currentSettings(0, MSBFIRST, SPI_MODE_0) {
}

SPIClass::~SPIClass() {
    end();
}

void SPIClass::begin() {
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("Could not open SPI device");
    }
}

void SPIClass::end() {
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}

void SPIClass::beginTransaction(SPISettings settings) {
    currentSettings = settings;
    applySettings();
}

void SPIClass::endTransaction() {
    // 在Linux SPI中,我们不需要显式地结束事务
}

uint8_t SPIClass::transfer(uint8_t data) {
    uint8_t rx;
    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)&data,
            .rx_buf = (unsigned long)&rx,
            .len = 1,
            .speed_hz = currentSettings.clock,
            .delay_usecs = 0,
            .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        throw std::runtime_error("Can't send SPI message");
    }

    return rx;
}

uint16_t SPIClass::transfer16(uint16_t data) {
    uint16_t rx;
    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)&data,
            .rx_buf = (unsigned long)&rx,
            .len = 2,
            .speed_hz = currentSettings.clock,
            .delay_usecs = 0,
            .bits_per_word = 16,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        throw std::runtime_error("Can't send SPI message");
    }

    // 如果需要，这里可以处理字节序（大端/小端）
    return rx;
}

void SPIClass::transfer(void *buf, size_t count) {
    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)buf,
            .rx_buf = (unsigned long)buf,
            .len = static_cast<__u32>(count),
            .speed_hz = currentSettings.clock,
            .delay_usecs = 0,
            .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        throw std::runtime_error("Can't send SPI message");
    }
}

void SPIClass::usingInterrupt(int interruptNumber) {
    // 在Linux中,这个函数可能不需要实现
}

void SPIClass::notUsingInterrupt(int interruptNumber) {
    // 在Linux中,这个函数可能不需要实现
}

void SPIClass::applySettings() {
    uint8_t mode = currentSettings.dataMode;
    uint8_t lsb = (currentSettings.bitOrder == LSBFIRST);
    uint32_t speed = currentSettings.clock;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        throw std::runtime_error("Can't set SPI mode");
    }

    if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb) < 0) {
        throw std::runtime_error("Can't set bit order");
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        throw std::runtime_error("Can't set max speed hz");
    }
}
