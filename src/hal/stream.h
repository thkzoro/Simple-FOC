//
// Created by book on 24-10-22.
//

#ifndef SIMPLEFOC_LINUX_STREAM_H
#define SIMPLEFOC_LINUX_STREAM_H
#include <iostream>
#include "Astring.h"

class Stream : public std::iostream {
public:
    virtual ~Stream() = default;

    virtual size_t print(int number) {
        std::cout << number;
        return sizeof(number);
    }

    virtual size_t print(float number, int decimal_places = 2) {
        std::cout.precision(decimal_places);
        std::cout << std::fixed << number;
        return sizeof(number);
    }

    virtual size_t print(const char* message) {
        std::cout << message;
        return strlen(message);
    }

    virtual size_t print(const std::string& message) {
        std::cout << message;
        return message.size();
    }

    virtual size_t print(char message) {
        std::cout << message;
        return sizeof(message);
    }

    virtual size_t print(const __FlashStringHelper *message) {
        return 0;
    }

    virtual size_t println(int number) {
        print(number);
        std::cout << std::endl;
        return sizeof(number) + 1;
    }

    virtual size_t println(float number, int decimal_places = 2) {
        print(number, decimal_places);
        std::cout << std::endl;
        return sizeof(number) + 1;
    }

    virtual size_t println(const char* message) {
        print(message);
        std::cout << std::endl;
        return strlen(message) + 1;
    }

    virtual size_t println(const std::string& message) {
        print(message);
        std::cout << std::endl;
        return message.size() + 1;
    }

    virtual size_t println(char message) {
        print(message);
        std::cout << std::endl;
        return sizeof(message) + 1;
    }

    virtual size_t println(const __FlashStringHelper *message) {
        return 0;
    }

    bool available() {
        return true;
    }

    size_t read() {
        return 0;
    }

};

#endif //SIMPLEFOC_LINUX_STREAM_H
