#include "serial_boost.hpp"
#include <iostream>

serial_boost::serial_boost(const std::string &port_name, size_t baudrate,
    size_t databits, parity parity, stopbits stopbits, flowctrl flowctrl) :
    serial(io, port_name)
{
    serial.set_option(boost::asio::serial_port::baud_rate(baudrate));
    serial.set_option(boost::asio::serial_port::character_size(databits));
    serial.set_option(boost::asio::serial_port::parity(
        static_cast<boost::asio::serial_port::parity::type>(parity)));
    serial.set_option(boost::asio::serial_port::stop_bits(
        static_cast<boost::asio::serial_port::stop_bits::type>(stopbits)));
    serial.set_option(boost::asio::serial_port::flow_control(
        static_cast<boost::asio::serial_port::flow_control::type>(flowctrl)));
}

std::vector<char> serial_boost::read()
{
    char byte = 0;
    boost::asio::read(serial, boost::asio::buffer(&byte, sizeof(byte)));
    
    std::vector<char> data;
    data.push_back(byte);
    
    return data;
}

void serial_boost::write(const std::vector<char> &data)
{
    boost::asio::write(serial, boost::asio::buffer(data));
}

void serial_boost::write(const char *data)
{
    boost::asio::write(serial, boost::asio::buffer(data, strlen(data)));
}

void serial_boost::write(char data)
{
    boost::asio::write(serial, boost::asio::buffer(&data, sizeof(data)));
}
