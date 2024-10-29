#include "Wire.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <system_error>

TwoWire Wire;

TwoWire::TwoWire()
        : m_fd(-1), m_address(0), m_tx_buffer_index(0), m_rx_buffer_index(0), m_rx_buffer_length(0),
          m_timeout(0), m_reset_on_timeout(false), m_timeout_flag(false) {}

TwoWire::~TwoWire() {
    end();
}

void TwoWire::begin() {
    // 打开I2C设备
    m_fd = open("/dev/i2c-0", O_RDWR);
    if (m_fd < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to open I2C device");
    }
}

void TwoWire::begin(uint8_t address) {
    begin();
    m_address = address;
}

void TwoWire::end() {
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, bool stop) {
    if (ioctl(m_fd, I2C_SLAVE, address) < 0) {
        throw std::runtime_error("Failed to set I2C slave address");
    }

    // 读取数据到接收缓冲区
    int bytes_read = ::read(m_fd, m_rx_buffer.data(), std::min(quantity, static_cast<uint8_t>(BUFFER_SIZE)));

    if (bytes_read < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to read from I2C device");
    }

    m_rx_buffer_index = 0;
    m_rx_buffer_length = bytes_read;

    return bytes_read;
}

void TwoWire::beginTransmission(uint8_t address) {
    m_address = address;
    m_tx_buffer_index = 0;
}

uint8_t TwoWire::endTransmission(bool stop) {
    if (ioctl(m_fd, I2C_SLAVE, m_address) < 0) {
        return 4; // 其他错误
    }

    if (::write(m_fd, m_tx_buffer.data(), m_tx_buffer_index) != m_tx_buffer_index) {
        return 4; // 其他错误
    }

    return 0; // 成功
}

size_t TwoWire::write(uint8_t data) {
    if (m_tx_buffer_index < BUFFER_SIZE) {
        m_tx_buffer[m_tx_buffer_index++] = data;
        return 1;
    }
    return 0;
}

size_t TwoWire::write(const uint8_t* data, size_t quantity) {
    size_t written = 0;
    while (written < quantity && m_tx_buffer_index < BUFFER_SIZE) {
        m_tx_buffer[m_tx_buffer_index++] = data[written++];
    }
    return written;
}

int TwoWire::available() {
    return m_rx_buffer_length - m_rx_buffer_index;
}

int TwoWire::read() {
    if (m_rx_buffer_index < m_rx_buffer_length) {
        return m_rx_buffer[m_rx_buffer_index++];
    }
    return -1;
}

void TwoWire::setClock(uint32_t clock) {
    // I2C时钟设置通常由Linux内核处理
    // 此函数可能不需要实现
}

void TwoWire::onReceive(void (*function)(int)) {
    m_receive_handler = function;
}

void TwoWire::onRequest(void (*function)(void)) {
    m_request_handler = function;
}

void TwoWire::setWireTimeout(uint32_t timeout, bool reset_on_timeout) {
    m_timeout = timeout;
    m_reset_on_timeout = reset_on_timeout;
}

bool TwoWire::getWireTimeoutFlag() {
    return m_timeout_flag;
}

void TwoWire::clearWireTimeoutFlag() {
    m_timeout_flag = false;
}

