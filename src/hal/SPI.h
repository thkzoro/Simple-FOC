//
// Created by book on 24-10-22.
//

#ifndef __SPI_H
#define __SPI_H
#include <wiringPiSPI.h>
#include <stdint.h>
#include "wiringShift.h"

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

struct SPISettings {
    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;

    // 默认构造函数
    SPISettings() : clock(0), bitOrder(0), dataMode(0) {}

    // 参数化构造函数
    SPISettings(uint32_t clk, uint8_t order, uint8_t mode)
            : clock(clk), bitOrder(order), dataMode(mode) {}
};


class SPIClass {
private:
    int channel;
    int speed;

public:
    SPIClass(int channel = 0) : channel(channel), speed(1000000) {}

    void begin() {
        wiringPiSetup();
        wiringPiSPISetup(channel, speed);
    }

    void beginTransaction(SPISettings settings) {
        // WiringPi 不直接支持动态改变 SPI 设置，所以这里我们只存储速度
        speed = settings.clock;
    }

    void endTransaction() {
        // 在 WiringPi 中不需要特别的结束操作
    }

    uint8_t transfer(uint8_t data) {
        uint8_t rx_data = data;
        wiringPiSPIDataRW(channel, &rx_data, 1);
        return rx_data;
    }

    uint16_t transfer16(uint16_t data) {
        uint8_t tx_data[2] = {(uint8_t)(data >> 8), (uint8_t)data};
        uint8_t rx_data[2];
        memcpy(rx_data, tx_data, 2);
        wiringPiSPIDataRW(channel, rx_data, 2);
        return (rx_data[0] << 8) | rx_data[1];
    }

    void end() {
        // WiringPi 没有直接的方法来结束 SPI，这里可以留空
    }
};

extern SPIClass SPI;

#endif //__SPI_H
