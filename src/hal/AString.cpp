#include "AString.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

String::String() : m_buffer() {}

String::String(const char *cstr) : m_buffer(cstr ? cstr : "") {}

String::String(const String &str) : m_buffer(str.m_buffer) {}

String::String(char c) : m_buffer(1, c) {}

String::String(unsigned char c) : m_buffer(1, static_cast<char>(c)) {}

String::String(int num, unsigned char base) : m_buffer(numberToString(num, base)) {}

String::String(unsigned int num, unsigned char base) : m_buffer(numberToString(num, base)) {}

String::String(long num, unsigned char base) : m_buffer(numberToString(num, base)) {}

String::String(unsigned long num, unsigned char base) : m_buffer(numberToString(num, base)) {}

String::String(float num, unsigned char decimalPlaces) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalPlaces) << num;
    m_buffer = oss.str();
}

String::String(double num, unsigned char decimalPlaces) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalPlaces) << num;
    m_buffer = oss.str();
}

String& String::operator = (const String &rhs) {
    if (this != &rhs) {
        m_buffer = rhs.m_buffer;
    }
    return *this;
}

String& String::operator += (const String &rhs) {
    m_buffer += rhs.m_buffer;
    return *this;
}

String operator + (const String &lhs, const String &rhs) {
    String result = lhs;
    result += rhs;
    return result;
}

char String::operator [] (unsigned int index) const {
    return m_buffer[index];
}

char& String::operator [] (unsigned int index) {
    return m_buffer[index];
}

unsigned int String::length() const {
    return m_buffer.length();
}

const char* String::c_str() const {
    return m_buffer.c_str();
}

void String::reserve(unsigned int size) {
    m_buffer.reserve(size);
}

void String::setCharAt(unsigned int index, char c) {
    if (index < m_buffer.length()) {
        m_buffer[index] = c;
    }
}

int String::toInt() const {
    return std::stoi(m_buffer);
}

float String::toFloat() const {
    return std::stof(m_buffer);
}

double String::toDouble() const {
    return std::stod(m_buffer);
}

std::string String::numberToString(long num, unsigned char base) {
    std::string result;
    bool isNegative = num < 0;
    num = std::abs(num);

    do {
        char digit = num % base;
        result += digit < 10 ? '0' + digit : 'A' + digit - 10;
        num /= base;
    } while (num > 0);

    if (isNegative) {
        result += '-';
    }

    std::reverse(result.begin(), result.end());
    return result;
}