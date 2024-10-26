#ifndef __ASERIAL_H
#define __ASERIAL_H
#include <string>
#include <termios.h>
#include <functional>
#include "AStream.h"

class ASerialPort : public Stream {
public:
    ASerialPort();
    virtual ~ASerialPort();
    // 初始化和关闭串口
    bool begin(unsigned long baud);
    void end();
    // 实现 Stream 的虚函数
    virtual int available() override;
    virtual int read() override;
    virtual int peek() override;
    virtual void flush() override;
    // 实现 Print 的虚函数
    virtual size_t write(uint8_t byte) override;

    // 额外的写入方法
    size_t write(const char* str);
    size_t write(const uint8_t* buffer, size_t size);
    // 设置串口参数
    bool setBaudRate(unsigned long baud);
    bool setDataBits(int bits);
    bool setParity(char parity);  // 'N', 'E', 'O'
    bool setStopBits(int bits);
    // 状态查询
    bool isOpen() const { return fd >= 0; }

private:
    int fd;              // 文件描述符
    int baudRate;        // 波特率
    int dataBits;        // 数据位
    char parity;         // 校验位
    int stopBits;        // 停止位

    bool configurePort();
    void clearError();
};

extern ASerialPort Serial;

#endif //__ASERIAL_H
