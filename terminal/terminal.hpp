#pragma once

#include <vector>
#include <stdint.h>

class terminal
{
public:
    struct event
    {
        enum class type
        {
            KEY,
            MOUSE,
            WINDOW_RESIZE,
            //MENU, // What is it?
            FOCUS
        };
        
        struct key
        {
            bool is_pressed;
            uint16_t key_code;
            uint16_t key_scan_code;
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
        
        event(bool is_pressed, uint16_t key_code, uint16_t key_scan_code,
            wchar_t unicode_char, char ascii_char, uint16_t repeat_count)
        {
            type = type::KEY;
            key.is_pressed = is_pressed;
            key.key_code = key_code;
            key.key_scan_code = key_scan_code;
            key.unicode_char = unicode_char;
            key.ascii_char = ascii_char;
            key.repeat_count = repeat_count;
        }
        
        type type;
        union
        {
            event::key key;
            event::mouse mouse;
            event::window_resize window_resize;
            // event::menu menu;
            event::focus focus;
        };
    };
    
    virtual ~terminal() {};
    virtual std::vector<event> read() = 0;
    virtual void write(char byte) = 0;
};
