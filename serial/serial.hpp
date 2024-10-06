#pragma once

#include <vector>

class serial
{
public:
    enum class parity
    {
        NONE,
        ODD,
        EVEN,
        MARK,
        SPACE
    };
    
    enum class stopbits
    {
        ONE,
        ONE_AND_HALF,
        TWO
    };
    
    enum class flowctrl
    {
        NONE, /// No flow control
        HARDWARE, /// Hardware flow control (RTS/CTS)
        SOFTWARE /// Software flow control (XON/XOFF)
    };

    virtual ~serial() = default;
    virtual std::vector<char> read() = 0;
    virtual void write(char data) = 0;
    virtual void write(const char *data) = 0;
    virtual void write(const std::vector<char> &data) = 0;
};
