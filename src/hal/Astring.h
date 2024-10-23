//
// Created by book on 24-10-22.
//

#ifndef __ASTRING_H
#define __ASTRING_H

class String : public std::string {
public:
    String() : std::string() {}
    String(const char* str) : std::string(str) {}
    String(const String& str) : std::string(str) {}
    explicit String(int num) : std::string(std::to_string(num)) {}
    explicit String(unsigned int num) : std::string(std::to_string(num)) {}
    explicit String(long num) : std::string(std::to_string(num)) {}
    explicit String(unsigned long num) : std::string(std::to_string(num)) {}
    explicit String(float num, unsigned int decimals = 2) : std::string(to_string_with_precision(num, decimals)) {}
    explicit String(double num, unsigned int decimals = 2) : std::string(to_string_with_precision(num, decimals)) {}
private:
    template<typename T>
    static std::string to_string_with_precision(const T a_value, const int n = 2) {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
    }
};

class __FlashStringHelper :public std::string {
public:
    __FlashStringHelper(const char* str) : std::string(str) {}
};

#endif //__ASTRING_H
