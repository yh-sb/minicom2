#pragma once

#include "serial.hpp"
#include <boost/asio.hpp>

class serial_boost : public serial
{
public:
    explicit serial_boost(const std::string &port_name, size_t baudrate = 115200,
        size_t databits = 8, parity parity = parity::NONE,
        stopbits stopbits = stopbits::ONE, flowctrl flowctrl = flowctrl::NONE);
    ~serial_boost() override = default;
    
    std::vector<char> read() override;
    void write(const std::vector<char> &data) override;
    void write(const char *data) override;
    void write(char data) override;
    
private:
    boost::asio::io_context io;
    boost::asio::serial_port serial;
};
