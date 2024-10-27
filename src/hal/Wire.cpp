#include "Wire.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <cstdio>

TwoWire Wire;

TwoWire::TwoWire() : fd(-1), currentAddress(0), rxIndex(0), timeout(0), timeoutFlag(false) {
}
TwoWire::~TwoWire() {
    end();
}
void TwoWire::begin() {
    // 打开I2C设备
    fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0) {
        // 处理错误
        perror("Failed to open I2C device");
    }
}
void TwoWire::end() {
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }
}
void TwoWire::beginTransmission(int address) {
    currentAddress = address;
    txBuffer.clear();
}
uint8_t TwoWire::endTransmission(bool sendStop) {
    if (fd < 0) return 4;  // 错误：总线未初始化
    if (ioctl(fd, I2C_SLAVE, currentAddress) < 0) {
        return 2;  // 错误：地址设置失败
    }
    if (::write(fd, txBuffer.data(), txBuffer.size()) != static_cast<ssize_t>(txBuffer.size())) {
        return 4;  // 错误：数据写入失败
    }
    return 0;  // 成功
}
uint8_t TwoWire::requestFrom(int address, int quantity, bool sendStop) {
    if (fd < 0) return 0;
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        return 0;
    }
    rxBuffer.resize(quantity);
    ssize_t count = ::read(fd, rxBuffer.data(), quantity);

    if (count < 0) {
        rxBuffer.clear();
        return 0;
    }
    rxIndex = 0;
    return count;
}
size_t TwoWire::write(uint8_t data) {
    txBuffer.push_back(data);
    return 1;
}
size_t TwoWire::write(const uint8_t* data, size_t quantity) {
    txBuffer.insert(txBuffer.end(), data, data + quantity);
    return quantity;
}
int TwoWire::available() {
    return rxBuffer.size() - rxIndex;
}
int TwoWire::read() {
    if (rxIndex >= rxBuffer.size()) {
        return -1;
    }
    return rxBuffer[rxIndex++];
}
void TwoWire::setClock(uint32_t frequency) {
    // Linux I2C驱动通常会自动处理时钟频率
    // 如果需要可以通过ioctl实现具体控制
}
void TwoWire::setWireTimeout(uint32_t timeoutValue, bool reset_with_timeout) {
    timeout = timeoutValue;
}
bool TwoWire::getWireTimeoutFlag() {
    return timeoutFlag;
}
void TwoWire::clearWireTimeoutFlag() {
    timeoutFlag = false;
}
void TwoWire::onReceive(std::function<void(int)> handler) {
    receiveHandler = handler;
}
void TwoWire::onRequest(std::function<void()> handler) {
    requestHandler = handler;
}
