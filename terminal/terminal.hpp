#pragma once

#include <variant>
#include <vector>
#include <cstdint>

class terminal
{
public:
    struct events
    {
        struct key
        {
            bool is_pressed;
            uint16_t code;
            uint16_t scan_code;
            wchar_t unicode_char;
            char ascii_char;
            uint16_t repeat_count;
        };
        
        struct mouse
        {
            size_t x;
            size_t y;
            int button_state;
            int control_key_state;
            int event_flags;
        };
        
        struct window_resize
        {
            size_t x;
            size_t y;
        };
        
        struct focus
        {
            bool is_focused;
        };
    };
    
    using event = std::variant<events::key, events::mouse, events::window_resize, events::focus>;
    
    virtual ~terminal() = default;
    virtual std::vector<event> read() = 0;
    virtual void write(char byte) = 0;
};
