#include "AStream.h"
#include <cstdlib>
#include <cstring>
#include <chrono>

// millis() 函数模拟
// 注意: 在实际的嵌入式系统中,你需要替换这个函数为真实的系统时间函数
unsigned long millis() {
    // 这里使用 C++11 的 chrono 库来模拟 millis() 函数
    using namespace std::chrono;
    static auto start = steady_clock::now();
    auto now = steady_clock::now();
    return duration_cast<milliseconds>(now - start).count();
}
// 读取一个字节,带超时
int Stream::timedRead() {
    unsigned long startMillis = millis();
    do {
        int c = read();
        if (c >= 0) return c;
    } while(millis() - startMillis < _timeout);
    return -1;     // -1 表示超时
}
// 预览一个字节,带超时
int Stream::timedPeek() {
    unsigned long startMillis = millis();
    do {
        int c = peek();
        if (c >= 0) return c;
    } while(millis() - startMillis < _timeout);
    return -1;     // -1 表示超时
}
// 查找目标字符串
bool Stream::find(char *target) {
    return find(target, strlen(target));
}
bool Stream::find(char *target, size_t length) {
    size_t index = 0;
    while (index < length) {
        int c = timedRead();
        if (c != target[index]) {
            index = 0;
            continue;
        }
        index++;
    }
    return index == length;
}
// 查找目标字符串,直到遇到终止符
bool Stream::findUntil(char *target, char *terminator) {
    size_t targetLen = strlen(target);
    size_t termLen = strlen(terminator);
    size_t index = 0;
    size_t termIndex = 0;

    while (true) {
        int c = timedRead();
        if (c < 0) return false;  // 超时
        if (c == target[index]) {
            index++;
            if (index == targetLen) return true;
        } else {
            index = 0;
        }
        if (c == terminator[termIndex]) {
            termIndex++;
            if (termIndex == termLen) return false;
        } else {
            termIndex = 0;
        }
    }
}
// 读取指定数量的字节
size_t Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0) break;
        *buffer++ = (char)c;
        count++;
    }
    return count;
}
// 读取字节直到遇到终止符
size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0 || c == terminator) break;
        *buffer++ = (char)c;
        count++;
    }
    return count;
}
// 读取字符串
String Stream::readString() {
    String ret;
    int c = timedRead();
    while (c >= 0) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}
// 读取字符串直到遇到终止符
String Stream::readStringUntil(char terminator) {
    String ret;
    int c = timedRead();
    while (c >= 0 && c != terminator) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}
// 查看下一个数字字符
int Stream::peekNextDigit(bool detectDecimal) {
    int c;
    while (true) {
        c = timedPeek();
        if (c < 0) return c;  // 超时
        if (c == '-') return c;
        if (c >= '0' && c <= '9') return c;
        if (detectDecimal && c == '.') return c;
        read();  // 丢弃非数字字符
    }
}
// 解析整数
long Stream::parseInt(char skipChar) {
    bool isNegative = false;
    long value = 0;
    int c;
    c = peekNextDigit(false);
    if (c < 0) return 0;  // 超时
    do {
        if (c == skipChar) {
            read();  // 跳过指定字符
            continue;
        }
        if (c == '-') {
            isNegative = true;
            read();
            continue;
        }
        if (c >= '0' && c <= '9') {
            value = value * 10 + c - '0';
            read();
        }
        c = timedPeek();
    } while (c >= '0' && c <= '9');
    return isNegative ? -value : value;
}
// 解析浮点数
float Stream::parseFloat(char skipChar) {
    bool isNegative = false;
    bool isFraction = false;
    long value = 0;
    float fraction = 1.0;
    int c;
    c = peekNextDigit(true);
    if (c < 0) return 0;  // 超时
    do {
        if (c == skipChar) {
            read();
            continue;
        }
        if (c == '-') {
            isNegative = true;
            read();
            continue;
        }
        if (c == '.') {
            isFraction = true;
            read();
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (isFraction) {
                fraction *= 0.1;
                value = value * 10 + (c - '0');
            } else {
                value = value * 10 + (c - '0');
            }
            read();
        }
        c = timedPeek();
    } while ((c >= '0' && c <= '9') || c == '.');
    float result = (float)value;
    if (isFraction) {
        result *= fraction;
    }
    return isNegative ? -result : result;
}


