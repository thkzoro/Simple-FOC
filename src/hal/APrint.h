#ifndef __PRINT_H
#define __PRINT_H

#include <cstddef>
#include <cstdint>
#include "AString.h"

class Print {
public:
    virtual ~Print() {}

    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const char *str);
    virtual size_t write(const uint8_t *buffer, size_t size);

    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const __FlashStringHelper *str);

    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);
    size_t println(const __FlashStringHelper *str);

protected:
    static const int DEC = 10;
    static const int HEX = 16;
    static const int OCT = 8;
    static const int BIN = 2;

private:
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
};

#endif //__PRINT_H
