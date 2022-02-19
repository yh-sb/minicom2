#include <iostream>
#include <memory>
#include <thread>

#include "event.hpp"
#include "sync_queue.hpp"

#ifdef _WIN32
#include "serial_win32.hpp"
#include "terminal_win32.hpp"
#else
#include "serial_posix.hpp"
#include "terminal_posix.hpp"
#endif

void termianal_thread(std::shared_ptr<terminal> terminal,
    sync_queue<event> &events_queue)
{
    while(1)
    {
        std::vector<terminal::event> terminal_events = terminal->read();
        
        for(terminal::event e : terminal_events)
        {
            events_queue << event(e);
        }
    }
}

void serial_thread(std::shared_ptr<serial> serial, sync_queue<event> &events_queue)
{
    while(1)
    {
        std::vector<char> serial_events = serial->read();
        
        for(char e : serial_events)
        {
            events_queue << event(e);
        }
    }
}

void coordinator_thread(sync_queue<event> &events_queue,
    std::shared_ptr<terminal> terminal, std::shared_ptr<serial> serial)
{
    while(1)
    {
        event event = events_queue.pop();
        
        switch(event.type)
        {
            case event::type::TERMINAL:
                if(event.terminal.type == terminal::event::type::KEY &&
                    event.terminal.key.is_pressed)
                {
                    switch(event.terminal.key.key_code)
                    {
                        case 33: serial->write("\e[5~"); break;   // PAGE UP
                        case 34: serial->write("\e[6~"); break;   // PAGE DOWN
                        case 35: serial->write("\e[4~"); break;   // END
                        case 36: serial->write("\e[1~"); break;   // HOME
                        case 37: serial->write("\e[D"); break;    // LEFT
                        case 38: serial->write("\e[A"); break;    // UP
                        case 39: serial->write("\e[C"); break;    // RIGHT
                        case 40: serial->write("\e[B"); break;    // DOWN
                        case 45: serial->write("\e[2~"); break;   // INSERT
                        case 46: serial->write("\e[3~"); break;   // DEL
                        case 112: serial->write("\eOP"); break;   // F1
                        case 113: serial->write("\eOQ"); break;   // F2
                        case 114: serial->write("\eOR"); break;   // F3
                        case 115: serial->write("\eOS"); break;   // F4
                        case 116: serial->write("\e[16~"); break; // F5
                        case 117: serial->write("\e[17~"); break; // F6
                        case 118: serial->write("\e[18~"); break; // F7
                        case 119: serial->write("\e[19~"); break; // F8
                        case 120: serial->write("\e[20~"); break; // F9
                        case 121: serial->write("\e[21~"); break; // F10
                        case 122: serial->write("\e[23~"); break; // F11
                        case 123: serial->write("\e[24~"); break; // F12
                        default: serial->write(event.terminal.key.ascii_char);
                    }
                }
                break;
            
            case event::type::SERIAL:
                terminal->write(event.serial);
                break;
        }
    }
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    std::shared_ptr<terminal> terminal = std::make_shared<terminal_win32>();
    std::shared_ptr<serial> serial = std::make_shared<serial_win32>("COM4",
        115200, 8, serial::parity::NONE, serial::stopbits::ONE,
        serial::dtr::DISABLE, serial::rts::DISABLE);
#else
    std::shared_ptr<terminal> terminal = std::make_shared<terminal_posix>();
    std::shared_ptr<serial> serial = std::make_shared<serial>(serial_posix("/dev/ttyUSB1"));
#endif
    
    sync_queue<event> events_queue;
    
    std::thread terminal_trd(termianal_thread, terminal, std::ref(events_queue));
    std::thread serial_trd(serial_thread, serial, std::ref(events_queue));
    std::thread coordinator_trd(coordinator_thread, std::ref(events_queue),
        terminal, serial);
    
    coordinator_trd.join();
    serial_trd.join();
    terminal_trd.join();
    
    return 0;
}
