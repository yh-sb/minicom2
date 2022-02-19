#include "terminal_win32.hpp"
#include <iostream>
#include <stdexcept>

#include <fileapi.h>
#include <winbase.h>
#include <windows.h>

BOOL WINAPI ctrl_handler(DWORD CtrlType)
{
    switch(CtrlType)
    {
        case CTRL_C_EVENT:
            std::cout << "CTRL_C_EVENT" << std::endl;
            break;
        
        case CTRL_BREAK_EVENT:
            std::cout << "CTRL_BREAK_EVENT" << std::endl;
            break;
        
        case CTRL_CLOSE_EVENT:
            std::cout << "CTRL_CLOSE_EVENT" << std::endl;
            break;
        
        case CTRL_LOGOFF_EVENT:
            std::cout << "CTRL_LOGOFF_EVENT" << std::endl;
            break;
        
        case CTRL_SHUTDOWN_EVENT:
            std::cout << "CTRL_SHUTDOWN_EVENT" << std::endl;
            break;
    }
    
    return true;
}

terminal_win32::terminal_win32()
{
    if(!SetConsoleCtrlHandler(ctrl_handler, true))
    {
        throw std::runtime_error("Failed to set Ctrl handler");
    }
    
    stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
    if(stdin_handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to get stdin handle");
    }
    
    DWORD stdin_mode;
    if(!GetConsoleMode(stdin_handle, &stdin_mode))
    {
        throw std::runtime_error("Failed to get stdin console mode");
    }
    // Process Ctrl+C, Backspace, Carriage return and Line feed as normal input
    stdin_mode &= ~(ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    if(!SetConsoleMode(stdin_handle, stdin_mode))
    {
        throw std::runtime_error("Failed to set stdin console mode");
    }
    
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(stdout_handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to get stdout handle");
    }
    
    DWORD stdout_mode;
    if(!GetConsoleMode(stdout_handle, &stdout_mode))
    {
        throw std::runtime_error("Failed to get stdout console mode");
    }
    
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x4 // Fix for GCC 8.1.0
    #endif
    stdout_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(stdout_handle, stdout_mode))
    {
        throw std::runtime_error("Failed to set stdout console mode");
    }
    
    CONSOLE_CURSOR_INFO cursor_info { .dwSize = 100, .bVisible = true};
    SetConsoleCursorInfo(stdout_handle, &cursor_info);
}

terminal_win32::~terminal_win32()
{
    //CloseHandle(stdin_handle);
    //CloseHandle(stdout_handle);
}

std::vector<terminal::event> terminal_win32::read()
{
    INPUT_RECORD first_event;
    DWORD events_count = 0;
    // Read first event
    ReadConsoleInput(stdin_handle, &first_event, 1, &events_count);
    
    // Read remaining events
    GetNumberOfConsoleInputEvents(stdin_handle, &events_count);
    INPUT_RECORD events[events_count + 1] {first_event};
    if(events_count)
    {
        ReadConsoleInput(stdin_handle, &events[1], events_count, &events_count);
    }
    
    std::vector<event> terminal_events;
    for(unsigned int i = 0; i < (events_count + 1); i++)
    {
        switch(events[i].EventType)
        {
            case KEY_EVENT:
            {
                KEY_EVENT_RECORD key_event = events[i].Event.KeyEvent;
                terminal_events.push_back(event(key_event.bKeyDown,
                    key_event.wVirtualKeyCode, key_event.wVirtualScanCode,
                    key_event.uChar.UnicodeChar, key_event.uChar.AsciiChar,
                    key_event.wRepeatCount));
            }
            break;
            
            case WINDOW_BUFFER_SIZE_EVENT:
                std::cout << "new window size: x=" << events[i].Event.WindowBufferSizeEvent.dwSize.X << \
                    " y=" << events[i].Event.WindowBufferSizeEvent.dwSize.Y << std::endl;
                break;
            
            case MOUSE_EVENT:
                std::cout << "mouse event" << std::endl;
                break;
            
            case MENU_EVENT:
                std::cout << "menu event" << std::endl;
                break;
            
            case FOCUS_EVENT:
                std::cout << "focus event" << std::endl;
                break;
        }
    }
    
    return terminal_events;
}

void terminal_win32::write(char byte)
{
    DWORD bytes_written;
    if(!WriteConsoleA(stdout_handle, &byte, sizeof(byte), &bytes_written, nullptr))
    {
        throw std::runtime_error(std::string("Failed to write to the console. GetLastError()=", GetLastError()));
    }
}

/*void terminal_win32::write(const std::string &data, const std::string &color,
    short x, short y)
{
    // Demo code
    std::cout << "\e[31mThis text has a red foreground using SGR.31.\n";
    std::cout << "\e[34;46mThis text shows the foreground and background change at the same time.\n";
    std::cout << "\e[mThis text has returned to default colors using SGR.0 implicitly." << std::endl;
    
    COORD text_position {x, y};
    if(!SetConsoleCursorPosition(stdout_handle, text_position))
    {
        throw std::runtime_error("Failed to set console cursor position");
    }
    std::cout << data << std::endl;
}*/
