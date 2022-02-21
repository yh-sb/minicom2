#include "serial_win32.hpp"
#include <iostream>
#include <stdexcept>

#include <fileapi.h>
#include <winbase.h>

serial_win32::serial_win32(const std::string &port_name, size_t baudrate,
    size_t databits, parity parity, stopbits stopbits, flowctrl flowctrl)
{
    std::string full_port_name = std::string("\\\\.\\") + port_name;
    
    port_handle = CreateFileA(full_port_name.c_str(),
        GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, nullptr);
    if(port_handle == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error(std::string("Failed to open serial port ", port_name.data()));
    }
    
    DCB comport_settings {.DCBlength = sizeof(DCB)};
    if(!GetCommState(port_handle, &comport_settings))
    {
        CloseHandle(port_handle);
        throw std::runtime_error("Failed to get serial port settings");
    }
    
    comport_settings.BaudRate = baudrate;
    comport_settings.fBinary = true;       // Binary mode; no EOF check
    comport_settings.fParity = true;       // Enable parity checking
    comport_settings.fOutxCtsFlow = false; // No CTS output flow control
    comport_settings.fOutxDsrFlow = false; // No DSR output flow control
    
    /* DTR_CONTROL_DISABLE   0x0
       DTR_CONTROL_ENABLE    0x1
       DTR_CONTROL_HANDSHAKE 0x2
    */
    comport_settings.fDtrControl = static_cast<int>(flowctrl);
    
    comport_settings.fDsrSensitivity = false;  // DSR sensitivity
    comport_settings.fTXContinueOnXoff = true; // XOFF continues Tx
    comport_settings.fOutX = false;            // No XON/XOFF out flow control
    comport_settings.fInX = false;             // No XON/XOFF in flow control
    comport_settings.fErrorChar = false;       // Disable error replacement
    comport_settings.fNull = false;            // Disable null stripping
    
    /* RTS_CONTROL_DISABLE   0x0
       RTS_CONTROL_ENABLE    0x1
       RTS_CONTROL_HANDSHAKE 0x2
       RTS_CONTROL_TOGGLE    0x3
    */
    comport_settings.fRtsControl = static_cast<int>(flowctrl);
    
    comport_settings.fAbortOnError = false; // Do not abort reads/writes on error
    
    comport_settings.ByteSize = databits;   // Number of bits in byte, 4-8
    
    /* NOPARITY    0
       ODDPARITY   1
       EVENPARITY  2
       MARKPARITY  3
       SPACEPARITY 4
    */
    comport_settings.Parity = static_cast<int>(parity);
    
    /* ONESTOPBIT   0
       ONE5STOPBITS 1
       TWOSTOPBITS  2
    */
    comport_settings.StopBits = static_cast<int>(stopbits);
    
    if(!SetCommState(port_handle, &comport_settings))
    {
        CloseHandle(port_handle);
        throw std::runtime_error("Failed to set serial port settings");
    }
    
    COMMTIMEOUTS timeouts
    {
        .ReadIntervalTimeout = MAXDWORD,
        .ReadTotalTimeoutMultiplier = 0,
        .ReadTotalTimeoutConstant = 0,
        .WriteTotalTimeoutMultiplier = 0,
        .WriteTotalTimeoutConstant = 0
    };
    if(!SetCommTimeouts(port_handle, &timeouts))
    {
        CloseHandle(port_handle);
        throw std::runtime_error("Failed to set serial port timeout settings");
    }
    
    if(!SetCommMask(port_handle, EV_RXCHAR /*| EV_TXEMPTY */| EV_ERR))
    {
        CloseHandle(port_handle);
        throw std::runtime_error("Failed to set serial port event mask");
    }
}

serial_win32::~serial_win32()
{
    CloseHandle(port_handle);
}

std::vector<char> serial_win32::read()
{
    OVERLAPPED overlapped { .hEvent = CreateEvent(nullptr, true, false, nullptr) };
    if(!overlapped.hEvent)
    {
        throw std::runtime_error("Failed to create event for reading");
    }
    DWORD event_mask;
    if(!WaitCommEvent(port_handle, &event_mask, &overlapped))
    {
        DWORD error = GetLastError();
        if(error != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            throw std::runtime_error(std::string("Error during waiting serial port comm event. GetLastError()=", error));
        }
    }
    
    if(WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0)
    {
        CloseHandle(overlapped.hEvent);
        throw std::runtime_error(std::string("Error during wating serial port rx tx events. GetLastError()=", GetLastError()));
    }
    
    if(!GetCommMask(port_handle, &event_mask))
    {
        CloseHandle(overlapped.hEvent);
        throw std::runtime_error(std::string("Error during getting serial port comm mask. GetLastError()=", GetLastError()));
    }
    
    if(event_mask & EV_ERR)
    {
        DWORD errors;
        COMSTAT stat;
        if(!ClearCommError(port_handle, &errors, &stat))
        {
            CloseHandle(overlapped.hEvent);
            throw std::runtime_error(std::string("Failed to clear serial port error. GetLastError()=", GetLastError()));
        }
        
        if(errors) // TODO: Why EV_ERR is present, but errors is 0 ?
        {
            std::cout << "\nSERIAL ERROR:";
            if(errors & CE_RXOVER) { std::cout << " RXOVER" << std::endl; }
            if(errors & CE_OVERRUN) { std::cout << " OVERRUN" << std::endl; }
            if(errors & CE_RXPARITY) { std::cout << " RXPARITY" << std::endl; }
            if(errors & CE_FRAME) { std::cout << " FRAME" << std::endl; }
            if(errors & CE_BREAK) { std::cout << " BREAK" << std::endl; }
            if(errors & CE_TXFULL) { std::cout << " TXFULL" << std::endl; }
            if(errors & CE_PTO) { std::cout << " PTO" << std::endl; }
            if(errors & CE_IOE) { std::cout << " IOE" << std::endl; }
            if(errors & CE_DNS) { std::cout << " DNS" << std::endl; }
            if(errors & CE_OOP) { std::cout << " OOP" << std::endl; }
            if(errors & CE_MODE) { std::cout << " MODE" << std::endl; }
        }
    }
    
    std::vector<char> result_buff;
    if(event_mask & EV_RXCHAR)
    {
        char buff[500];
        DWORD bytes_read;
        do
        {
            if(!ReadFile(port_handle, buff, sizeof(buff), &bytes_read, &overlapped))
            {
                DWORD error = GetLastError();
                if(error != ERROR_IO_PENDING) // No data to read for now
                {
                    CloseHandle(overlapped.hEvent);
                    throw std::runtime_error(std::string("Failed to read data from serial port. GetLastError()=", error));
                }
            }
            
            for(size_t i = 0; i < bytes_read; i++)
            {
                result_buff.push_back(buff[i]);
            }
            
        } while(bytes_read > 0);
    }
    if(event_mask & EV_TXEMPTY)
    {
        // Bytes sent
    }
    ResetEvent(overlapped.hEvent);
    CloseHandle(overlapped.hEvent);
    
    return result_buff;
}

void serial_win32::write(const std::vector<char> &data)
{
    OVERLAPPED overlapped { .hEvent = CreateEvent(nullptr, true, false, nullptr) };
    if(!overlapped.hEvent)
    {
        throw std::runtime_error("Failed to create event for reading");
    }
    
    DWORD bytes_written = 0;
    if(!WriteFile(port_handle, data.data(), data.size(), &bytes_written, &overlapped))
    {
        DWORD error = GetLastError();
        if(error != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            throw std::runtime_error(std::string("Failed to write data to the serial port. GetLastError()=", error));
        }
        WaitForSingleObject(overlapped.hEvent, INFINITE);
    }
    CloseHandle(overlapped.hEvent);
}

void serial_win32::write(const char *data)
{
    OVERLAPPED overlapped { .hEvent = CreateEvent(nullptr, true, false, nullptr) };
    if(!overlapped.hEvent)
    {
        throw std::runtime_error("Failed to create event for reading");
    }
    
    DWORD bytes_written = 0;
    if(!WriteFile(port_handle, data, strlen(data), &bytes_written, &overlapped))
    {
        DWORD error = GetLastError();
        if(error != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            throw std::runtime_error(std::string("Failed to write data to the serial port. GetLastError()=", error));
        }
        WaitForSingleObject(overlapped.hEvent, INFINITE);
    }
    CloseHandle(overlapped.hEvent);
}

void serial_win32::write(char data)
{
    OVERLAPPED overlapped { .hEvent = CreateEvent(nullptr, true, false, nullptr) };
    if(!overlapped.hEvent)
    {
        throw std::runtime_error("Failed to create event for reading");
    }
    
    DWORD bytes_written = 0;
    if(!WriteFile(port_handle, &data, sizeof(data), &bytes_written, &overlapped))
    {
        DWORD error = GetLastError();
        if(error != ERROR_IO_PENDING)
        {
            CloseHandle(overlapped.hEvent);
            throw std::runtime_error(std::string("Failed to write data to the serial port. GetLastError()=", error));
        }
        WaitForSingleObject(overlapped.hEvent, INFINITE);
    }
    CloseHandle(overlapped.hEvent);
}
