// ASerialPort.cpp
#include "ASerial.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <system_error>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>

ASerialPort Serial;

ASerialPort::ASerialPort() : fd(-1), baudRate(9600),
                             dataBits(8), parity('N'), stopBits(1) {
}
ASerialPort::~ASerialPort() {
    end();
}
bool ASerialPort::begin(unsigned long baud) {
    // 打开串口设备
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return false;
    }
    baudRate = baud;
    return configurePort();
}
void ASerialPort::end() {
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}
int ASerialPort::available() {
    if (fd < 0) return 0;

    int bytes;
    ioctl(fd, FIONREAD, &bytes);
    return bytes;
}
int ASerialPort::read() {
    if (fd < 0) return -1;

    uint8_t byte;
    if (::read(fd, &byte, 1) == 1) {
        return byte;
    }
    return -1;
}
int ASerialPort::peek() {
    if (fd < 0) return -1;

    uint8_t byte;
    if (::read(fd, &byte, 1) == 1) {
        // 读取后将指针移回
        lseek(fd, -1, SEEK_CUR);
        return byte;
    }
    return -1;
}
void ASerialPort::flush() {
    if (fd >= 0) {
        tcflush(fd, TCIOFLUSH);
    }
}
size_t ASerialPort::write(uint8_t byte) {
    if (fd < 0) return 0;
    return ::write(fd, &byte, 1);
}
size_t ASerialPort::write(const char* str) {
    if (fd < 0 || str == nullptr) return 0;
    return ::write(fd, str, strlen(str));
}
size_t ASerialPort::write(const uint8_t* buffer, size_t size) {
    if (fd < 0 || buffer == nullptr) return 0;
    return ::write(fd, buffer, size);
}
bool ASerialPort::configurePort() {
    if (fd < 0) return false;
    struct termios options;
    tcgetattr(fd, &options);
    // 设置波特率
    speed_t baud;
    switch (baudRate) {
        case 9600:   baud = B9600;   break;
        case 19200:  baud = B19200;  break;
        case 38400:  baud = B38400;  break;
        case 57600:  baud = B57600;  break;
        case 115200: baud = B115200; break;
        default:     return false;
    }
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    // 设置数据位
    options.c_cflag &= ~CSIZE;
    switch (dataBits) {
        case 5: options.c_cflag |= CS5; break;
        case 6: options.c_cflag |= CS6; break;
        case 7: options.c_cflag |= CS7; break;
        case 8: options.c_cflag |= CS8; break;
        default: return false;
    }
    // 设置校验位
    switch (parity) {
        case 'N':
            options.c_cflag &= ~PARENB;
            break;
        case 'E':
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case 'O':
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;
        default:
            return false;
    }
    // 设置停止位
    if (stopBits == 1) {
        options.c_cflag &= ~CSTOPB;
    } else if (stopBits == 2) {
        options.c_cflag |= CSTOPB;
    } else {
        return false;
    }
    // 其他设置
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    // 应用设置
    tcsetattr(fd, TCSANOW, &options);
    return true;
}
