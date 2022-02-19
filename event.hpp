#pragma once

#include "serial.hpp"
#include "terminal.hpp"

struct event
{
    enum class type
    {
        SERIAL,
        TERMINAL
    };
    
    event(char event)
    {
        type = type::SERIAL;
        serial = event;
    };
    event(const terminal::event &event)
    {
        type = type::TERMINAL;
        terminal = event;
    };
    
    type type;
    union
    {
        char serial;
        terminal::event terminal;
    };
};
