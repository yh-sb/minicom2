#pragma once

#include "serial.hpp"
#include <string_view>

class serial_posix : public serial
{
public:
    serial_posix(std::string_view port_name);
    ~serial_posix();
    
    std::vector<char> read() override;
    void write(const std::vector<char> &data) override;
    void write(const char *data) override;
    void write(char data) override;
    
private:
    //HANDLE comport_handle;
};
