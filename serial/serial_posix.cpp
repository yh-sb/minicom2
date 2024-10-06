#include "serial_posix.hpp"
#include <stdexcept>

serial_posix::serial_posix(const std::string &port_name, size_t baudrate,
    size_t databits, parity parity, stopbits stopbits, flowctrl flowctrl)
{
    
}

std::vector<char> serial_posix::read()
{
    return {};
}

void serial_posix::write(const std::vector<char> &data)
{
    
}

void serial_posix::write(const char *data)
{
    
}

void serial_posix::write(char data)
{
    
}
