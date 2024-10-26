#ifndef __ASTRING_H
#define __ASTRING_H

#include <string>
#include <cstdint>

class String {
public:
    // 构造函数
    String();
    String(const char *cstr);
    String(const String &str);
    String(char c);
    String(unsigned char c);
    String(int num, unsigned char base = 10);
    String(unsigned int num, unsigned char base = 10);
    String(long num, unsigned char base = 10);
    String(unsigned long num, unsigned char base = 10);
    String(float num, unsigned char decimalPlaces = 2);
    String(double num, unsigned char decimalPlaces = 2);

    // 析构函数
    ~String() = default;

    // 运算符重载
    String& operator = (const String &rhs);
    String& operator += (const String &rhs);
    friend String operator + (const String &lhs, const String &rhs);
    char operator [] (unsigned int index) const;
    char& operator [] (unsigned int index);

    // 其他方法
    unsigned int length() const;
    const char* c_str() const;
    void reserve(unsigned int size);
    void setCharAt(unsigned int index, char c);
    int toInt() const;
    float toFloat() const;
    double toDouble() const;

private:
    std::string m_buffer;

    // 私有辅助函数
    static std::string numberToString(long num, unsigned char base);

public:
    static const unsigned char DEC = 10;
    static const unsigned char HEX = 16;
    static const unsigned char OCT = 8;
    static const unsigned char BIN = 2;
};



class __FlashStringHelper {
private:
    const char* _str;

public:
    __FlashStringHelper(const char* str) : _str(str) {}

    const char* c_str() const { return _str; }
};

#define PSTR(str) (str)
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

class StringSumHelper : public String {
public:
    StringSumHelper(const String &s) : String(s) {}
    StringSumHelper(const char *p) : String(p) {}
    StringSumHelper(char c) : String(c) {}
    StringSumHelper(unsigned char num) : String(num) {}
    StringSumHelper(int num) : String(num) {}
    StringSumHelper(unsigned int num) : String(num) {}
    StringSumHelper(long num) : String(num) {}
    StringSumHelper(unsigned long num) : String(num) {}
    StringSumHelper(float num) : String(num) {}
    StringSumHelper(double num) : String(num) {}
};
#endif //__ASTRING_H
