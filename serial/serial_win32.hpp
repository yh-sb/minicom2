#pragma once

#include "serial.hpp"
#include <string_view>
#include <windows.h>

class serial_win32 : public serial
{
public:
    serial_win32(std::string_view port_name, size_t baudrate, size_t databits = 8,
        parity parity = parity::NONE, stopbits stopbits = stopbits::ONE,
        dtr dtr = dtr::ENABLE, rts rts = rts::ENABLE);
    ~serial_win32();
    
    std::vector<char> read() override;
    void write(const std::vector<char> &data) override;
    void write(const char *data) override;
    void write(char data) override;
    
private:
    HANDLE port_handle;
};
