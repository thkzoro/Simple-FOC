#ifndef __PRINT_H
#define __PRINT_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <stdint.h>
#include "Astring.h"

class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    size_t print(const char* str) {
        return write((const uint8_t*)str, strlen(str));
    }

    size_t print(char c) {
        return write((uint8_t)c);
    }

    size_t print(unsigned char num, int base = DEC) {
        return print((unsigned long)num, base);
    }

    size_t print(int num, int base = DEC) {
        return print((long)num, base);
    }

    size_t print(unsigned int num, int base = DEC) {
        return print((unsigned long)num, base);
    }

    size_t print(long num, int base = DEC) {
        if (base == 0) {
            return write(num);
        } else {
            return print(std::to_string(num).c_str());
        }
    }

    size_t print(unsigned long num, int base = DEC) {
        if (base == 0) {
            return write(num);
        } else {
            return print(std::to_string(num).c_str());
        }
    }

    size_t print(double num, int digits = 2) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(digits) << num;
        return print(oss.str().c_str());
    }

    size_t print(const __FlashStringHelper *ifsh) {
        return print(ifsh->c_str());
    }

    size_t println(const char* str) {
        size_t n = print(str);
        n += println();
        return n;
    }

    size_t println(char c) {
        size_t n = print(c);
        n += println();
        return n;
    }

    size_t println(unsigned char num, int base = DEC) {
        size_t n = print(num, base);
        n += println();
        return n;
    }

    size_t println(int num, int base = DEC) {
        size_t n = print(num, base);
        n += println();
        return n;
    }

    size_t println(unsigned int num, int base = DEC) {
        size_t n = print(num, base);
        n += println();
        return n;
    }

    size_t println(long num, int base = DEC) {
        size_t n = print(num, base);
        n += println();
        return n;
    }

    size_t println(unsigned long num, int base = DEC) {
        size_t n = print(num, base);
        n += println();
        return n;
    }

    size_t println(double num, int digits = 2) {
        size_t n = print(num, digits);
        n += println();
        return n;
    }

    size_t println(const __FlashStringHelper *ifsh) {
        return println(ifsh->c_str());
    }

    size_t println(void) {
        return write((uint8_t)'\n');
    }

protected:
    static const int DEC = 10;
    static const int HEX = 16;
    static const int OCT = 8;
    static const int BIN = 2;
};


#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
#define PSTR(str) (str)


// 模拟 StringSumHelper
typedef String StringSumHelper;

#endif //__PRINT_H
