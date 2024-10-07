#include <iostream>
#include <memory>
#include <thread>
#include <variant>
#include "cli_options.hpp"
#include "sync_queue.hpp"
#include "serial_boost.hpp"
#ifdef _WIN32
#include "terminal_win32.hpp"
#else
#include "terminal_posix.hpp"
#endif

using universal_event = std::variant<char, terminal::event>;

namespace
{
void terminal_thread(terminal &terminal,
    sync_queue<universal_event> &events_queue)
{
    while(true)
    {
        for(auto &&e : terminal.read())
        {
            events_queue << e;
        }
    }
}

void serial_thread(serial &serial, sync_queue<universal_event> &events_queue)
{
    while(true)
    {
        for(char e : serial.read())
        {
            events_queue << e;
        }
    }
}

void coordinator_thread(sync_queue<universal_event> &events_queue,
    terminal &terminal, serial &serial)
{
    while(true)
    {
        universal_event event = events_queue.pop();
        
        if(char *byte = std::get_if<char>(&event))
        {
            terminal.write(*byte);
        }
        else if(auto *terminal_event = std::get_if<terminal::event>(&event))
        {
            if(auto *key = std::get_if<terminal::events::key>(terminal_event);
                key && key->is_pressed)
            {
                switch(key->code)
                {
                    case 33: serial.write("\e[5~"); break;   // PAGE UP
                    case 34: serial.write("\e[6~"); break;   // PAGE DOWN
                    case 35: serial.write("\e[4~"); break;   // END
                    case 36: serial.write("\e[1~"); break;   // HOME
                    case 37: serial.write("\e[D"); break;    // LEFT
                    case 38: serial.write("\e[A"); break;    // UP
                    case 39: serial.write("\e[C"); break;    // RIGHT
                    case 40: serial.write("\e[B"); break;    // DOWN
                    case 45: serial.write("\e[2~"); break;   // INSERT
                    case 46: serial.write("\e[3~"); break;   // DEL
                    case 112: serial.write("\eOP"); break;   // F1
                    case 113: serial.write("\eOQ"); break;   // F2
                    case 114: serial.write("\eOR"); break;   // F3
                    case 115: serial.write("\eOS"); break;   // F4
                    case 116: serial.write("\e[16~"); break; // F5
                    case 117: serial.write("\e[17~"); break; // F6
                    case 118: serial.write("\e[18~"); break; // F7
                    case 119: serial.write("\e[19~"); break; // F8
                    case 120: serial.write("\e[20~"); break; // F9
                    case 121: serial.write("\e[21~"); break; // F10
                    case 122: serial.write("\e[23~"); break; // F11
                    case 123: serial.write("\e[24~"); break; // F12
                    default: serial.write(key->ascii_char);
                }
            }
        }
    }
}
} // namespace

int main(int argc, char *argv[])
{
    const cli_options opts(argc, argv);
    
    const std::unique_ptr<serial> serial = std::make_unique<serial_boost>(opts.device,
        opts.baudrate, opts.databits, opts.parity, opts.stopbits, opts.flowctrl);
    
#ifdef _WIN32
    const std::unique_ptr<terminal> terminal = std::make_unique<terminal_win32>();
#else
    const std::unique_ptr<terminal> terminal = std::make_unique<terminal_posix>();
#endif
    
    sync_queue<universal_event> events_queue;
    
    std::thread terminal_trd(terminal_thread, std::ref(*terminal), std::ref(events_queue));
    std::thread serial_trd(serial_thread, std::ref(*serial), std::ref(events_queue));
    std::thread coordinator_trd(coordinator_thread, std::ref(events_queue),
        std::ref(*terminal), std::ref(*serial));
    
    coordinator_trd.join();
    serial_trd.join();
    terminal_trd.join();
    
    return 0;
}
