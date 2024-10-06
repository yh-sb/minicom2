#pragma once

#include <string>
#include "serial.hpp"

struct cli_options
{
    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)
    cli_options(int argc, char *argv[]);
    
    std::string device;
    size_t baudrate;
    size_t databits;
    serial::parity parity;
    serial::stopbits stopbits;
    serial::flowctrl flowctrl;
};
