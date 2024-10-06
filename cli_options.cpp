#include <iostream>
#include "cli_options.hpp"
#include <boost/program_options.hpp>

namespace
{
void print_usage()
{
    std::cout << "Usage:\tminicom2 <device> [options]" << std::endl;
}

void print_options()
{
    std::cout << "Options:\n"\
    "\t-h, --help        Print help information\n"\
    "\t-b, --baudrate    Baudrate (900 - 3000000). Default is 115200 bps\n"\
    "\t-d, --databits    Number of data bits (5-8). Default is 8\n"\
    "\t-p, --parity      Parity control (none, odd, even). Default is none\n"\
    "\t-s, --stopbits    Number of stopbits (1, 1.5|1,5, 2). Default is 1\n"\
    "\t-f, --flowctrl    Flow control type (none, hardware|hw, software|sw). Default is none" << std::endl;
}
} // namespace

// NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays)
cli_options::cli_options(int argc, char *argv[]) :
    baudrate(0), databits(0), parity(serial::parity::NONE),
    stopbits(serial::stopbits::ONE), flowctrl(serial::flowctrl::NONE)
{
    std::string parity_str, stopbits_str, flowctrl_str;
    
    boost::program_options::options_description desc;
    desc.add_options() ("device", boost::program_options::value<std::string>())
        ("help,h", "")
        ("baudrate,b", boost::program_options::value<size_t>(&baudrate)->default_value(115200))
        ("databits,d", boost::program_options::value<size_t>(&databits)->default_value(8))
        ("parity,p", boost::program_options::value<std::string>(&parity_str)->default_value("none"))
        ("stopbits,s", boost::program_options::value<std::string>(&stopbits_str)->default_value("1"))
        ("flowctrl,f", boost::program_options::value<std::string>(&flowctrl_str)->default_value("none"));
    
    boost::program_options::positional_options_description pos_desc;
    pos_desc.add("device", 1);
    
    boost::program_options::variables_map opts;
    
    boost::program_options::store(boost::program_options::command_line_parser(
        argc, argv).options(desc).positional(pos_desc).run(), opts);
    boost::program_options::notify(opts);
    
    if(opts.count("help") || argc == 1)
    {
        print_usage();
        print_options();
        std::exit(EXIT_SUCCESS); // NOLINT(concurrency-mt-unsafe)
    }
    
    if(!opts.count("device")) // Mandatory option
    {
        throw std::runtime_error(std::string("Wrong device name: \"\". Must be valid serial device name. "\
            "Example: COM4 or /dev/ttyUSB1"));
    }
    device = opts["device"].as<std::string>();
    
    // Baudrate validation
    if(baudrate < 900 || baudrate > 3'000'000)
    {
        std::cerr << "Wrong baudrate value: \"" << baudrate << "\". "\
            "Must be in range: 900 - 3000000" << std::endl;
        throw std::runtime_error(std::string("Wrong baudrate value: ..."));
    }
    
    // Databits validation
    if(databits < 5 || databits > 8)
    {
        std::cerr << "Wrong databits value: \"" << databits << "\". "\
            "Must be in range: 5 - 8" << std::endl;
        throw std::runtime_error(std::string("Wrong databits value: ..."));
    }
    
    // Parity validation
    std::map<std::string, serial::parity> parity_vals {
        {"none", serial::parity::NONE}, {"odd", serial::parity::ODD},
        {"even", serial::parity::EVEN}};
    if(!parity_vals.contains(parity_str))
    {
        std::cerr << "Wrong parity value: \"" << parity_str << "\". "\
            "Must be one of the following: none, odd, even" << std::endl;
        throw std::runtime_error(std::string("Wrong parity value: ..."));
    }
    parity = parity_vals[parity_str];
    
    // Stopbits validation
    std::map<std::string, serial::stopbits> stopbits_vals {
        {"1", serial::stopbits::ONE}, {"1.5", serial::stopbits::ONE_AND_HALF},
        {"1,5", serial::stopbits::ONE_AND_HALF}, {"2", serial::stopbits::TWO}};
    if(!stopbits_vals.contains(stopbits_str))
    {
        std::cerr << "Wrong stopbits value: \"" << stopbits_str << "\". "\
            "Must be one of the following: 1, 1.5|1,5, 2" << std::endl;
        throw std::runtime_error(std::string("Wrong stopbits value: ..."));
    }
    stopbits = stopbits_vals[stopbits_str];
    
    // Flowctrl validation
    std::map<std::string, serial::flowctrl> flowctrl_vals {
        {"none", serial::flowctrl::NONE}, {"hardware", serial::flowctrl::HARDWARE},
        {"hw", serial::flowctrl::HARDWARE}, {"software", serial::flowctrl::SOFTWARE},
        {"sw", serial::flowctrl::SOFTWARE}};
    if(!flowctrl_vals.contains(flowctrl_str))
    {
        std::cerr << "Wrong flow control value: \"" << flowctrl_str << "\". "\
            "Must be one of the following: none, hardware|hw, software|sw" << std::endl;
        throw std::runtime_error(std::string("Wrong flow control value: ..."));
    }
    flowctrl = flowctrl_vals[flowctrl_str];
}
