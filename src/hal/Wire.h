#ifndef __WIRE_H
#define __WIRE_H

#include <vector>
#include <functional>
#include <cstdint>
#include <cstddef>

class TwoWire {
public:
    TwoWire();
    ~TwoWire();
    // 基本I2C操作
    void begin();
    void end();
    void beginTransmission(int address);
    uint8_t endTransmission(bool sendStop = true);
    uint8_t requestFrom(int address, int quantity, bool sendStop = true);

    // 数据读写
    size_t write(uint8_t data);
    size_t write(const uint8_t* data, size_t quantity);
    int available();
    int read();

    // 时钟和超时设置
    void setClock(uint32_t frequency);
    void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false);
    bool getWireTimeoutFlag();
    void clearWireTimeoutFlag();

    // 回调函数设置
    void onReceive(std::function<void(int)> handler);
    void onRequest(std::function<void()> handler);
private:
    int fd;                     // I2C设备文件描述符
    int currentAddress;         // 当前I2C设备地址
    std::vector<uint8_t> txBuffer;  // 发送缓冲区
    std::vector<uint8_t> rxBuffer;  // 接收缓冲区
    size_t rxIndex;            // 接收缓冲区当前读取位置

    unsigned long timeout;      // 超时时间设置
    bool timeoutFlag;          // 超时标志

    std::function<void(int)> receiveHandler;    // 接收回调函数
    std::function<void()> requestHandler;       // 请求回调函数
};

extern TwoWire Wire;

#endif //__WIRE_H
