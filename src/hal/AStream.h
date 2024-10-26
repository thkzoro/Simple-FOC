#ifndef __ASTREAM_H
#define __ASTREAM_H
#include "APrint.h"
class Stream : public Print {
public:
    Stream() : _timeout(1000) {}
    virtual ~Stream() {}
    // 纯虚函数 - 需要被子类实现
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    // 设置超时时间
    void setTimeout(unsigned long timeout) { _timeout = timeout; }
    unsigned long getTimeout() const { return _timeout; }
    // 查找函数
    bool find(char *target);
    bool find(char *target, size_t length);
    bool findUntil(char *target, char *terminator);
    // 读取函数
    size_t readBytes(char *buffer, size_t length);
    size_t readBytesUntil(char terminator, char *buffer, size_t length);
    String readString();
    String readStringUntil(char terminator);
    // 解析函数
    long parseInt(char skipChar = NO_SKIP_CHAR);
    float parseFloat(char skipChar = NO_SKIP_CHAR);
protected:
    unsigned long _timeout;    // 超时时间(毫秒)

    // 辅助函数
    int timedRead();
    int timedPeek();
    int peekNextDigit(bool detectDecimal);
    static const char NO_SKIP_CHAR = '\0';
private:
    // 禁用拷贝构造和赋值操作
    Stream(const Stream&);
    Stream& operator=(const Stream&);
};

#endif //__ASTREAM_H
