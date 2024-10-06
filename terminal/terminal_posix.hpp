#pragma once

#include "terminal.hpp"
#include <string_view>

class terminal_posix : public terminal
{
public:
    terminal_posix() = default;
    ~terminal_posix() override = default;
    
    std::vector<event> read() override;
    void write(char byte) override;
    
private:
};
