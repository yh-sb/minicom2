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
    
    /**
     * @brief DTR flow control
     */
    enum class dtr
    {
        DISABLE,
        ENABLE,
        HANDSHAKE
    };
    
    /**
     * @brief RTS flow control
     */
    enum class rts
    {
        DISABLE,
        ENABLE,
        HANDSHAKE,
        TOGGLE
    };

    virtual ~serial() {};
    virtual std::vector<char> read() = 0;
    virtual void write(const std::vector<char> &data) = 0;
    virtual void write(const char *data) = 0;
    virtual void write(char data) = 0;
};
