#pragma once

#include "terminal.hpp"
#include <windows.h>

class terminal_win32 : public terminal
{
public:
    terminal_win32();
    ~terminal_win32();
    
    std::vector<event> read() override;
    void write(char byte) override;
    /*void write(const std::string &data, const std::string &color,
        short x = 0, short y = 0);*/
    
private:
    HANDLE stdin_handle;
    HANDLE stdout_handle;
};
