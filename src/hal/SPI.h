//
// Created by book on 24-10-22.
//

#ifndef __SPI_H
#define __SPI_H
#include <cstdint>
#include <string>

// SPI modes
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

// Bit orders
#define MSBFIRST 0
#define LSBFIRST 1

class SPISettings {
public:
    SPISettings() : clock(0), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;
};

class SPIClass {
public:
    SPIClass();
    ~SPIClass();

    void begin();
    void end();
    void beginTransaction(SPISettings settings);
    void endTransaction();
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
    void usingInterrupt(int interruptNumber);
    void notUsingInterrupt(int interruptNumber);

private:
    int fd;
    std::string device;
    SPISettings currentSettings;
    void applySettings();
};

extern SPIClass SPI;


#endif //__SPI_H
