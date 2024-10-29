#ifndef __WIRE_H
#define __WIRE_H

#include <cstdint>
#include <functional>
#include <vector>
#include <array>
#include <optional>

class TwoWire {
public:
    TwoWire();
    ~TwoWire();

    // 禁止拷贝和移动
    TwoWire(const TwoWire&) = delete;
    TwoWire& operator=(const TwoWire&) = delete;
    TwoWire(TwoWire&&) = delete;
    TwoWire& operator=(TwoWire&&) = delete;

    void begin();
    void begin(uint8_t address);
    void end();
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stop = true);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stop = true);
    size_t write(uint8_t data);
    size_t write(const uint8_t* data, size_t quantity);
    int available();
    int read();
    void setClock(uint32_t clock);
    void onReceive(void (*function)(int));
    void onRequest(void (*function)(void));
    void setWireTimeout(uint32_t timeout = 25000, bool reset_on_timeout = false);
    bool getWireTimeoutFlag();
    void clearWireTimeoutFlag();

private:
    static constexpr size_t BUFFER_SIZE = 32;  // 固定缓冲区大小

    int m_fd;
    uint8_t m_address;
    std::array<uint8_t, BUFFER_SIZE> m_tx_buffer;
    size_t m_tx_buffer_index;
    std::array<uint8_t, BUFFER_SIZE> m_rx_buffer;
    size_t m_rx_buffer_index;
    size_t m_rx_buffer_length;
    std::function<void(int)> m_receive_handler;
    std::function<void()> m_request_handler;
    uint32_t m_timeout;
    bool m_reset_on_timeout;
    bool m_timeout_flag;
};

extern TwoWire Wire;

#endif //__WIRE_H
