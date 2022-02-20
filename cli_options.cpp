#include <iostream>
#include "cli_options.hpp"
#include "argh.h"

void print_help(const char *program_name)
{
    std::cout << "Usage:\n"\
    "\t" << program_name << " <device> [options]\n"\
    "Options:\n"\
    "\t-h, --help        Print help information\n"\
    "\t-b, --baudrate    Baudrate. Default is 115200 bps\n"\
    "\t-d, --databits    Number of data bits (4-8). Default is 8\n"\
    "\t-p, --parity      Parity control (none, odd, even, mark, space). Default is none\n"\
    "\t-s, --stopbits    Number of stopbits (1, 1.5, 2). Default is 1\n"\
    "\tTODO: handle flow control options" << std::endl;
}

cli_options::cli_options(int argc, char *argv[]) :
    dtr(serial::dtr::DISABLE),
    rts(serial::rts::DISABLE)
{
    argh::parser cmdl;
    cmdl.add_params({"-h", "--help", "-b", "--baudrate", "-d", "--databits",
        "-p", "--parity", "-s", "--stopbits"});
    cmdl.parse(argc, argv);
    
    if(argc == 1 || cmdl[{"-h", "--help"}])
    {
        print_help(argv[0]);
        exit(0);
    }
    
    // <device>
    if(cmdl[1].empty())
    {
        std::cerr << "Wrong device name: \"" << argv[1] << "\". "\
            "Must be valid serial device name. Example: COM4 or /dev/ttyUSB1" << std::endl;
        exit(1);
    }
    device = cmdl[1];
    
    // -b, --baudrate
    if(!(cmdl({"-b", "--baudrate"}, 115200) >> baudrate) || baudrate < 900 || baudrate > 3'000'000)
    {
        std::cerr << "Wrong baudrate value: \"" << cmdl({"-b", "--baudrate"}).str() <<
            "\". Must be in range: 900 - 3000000" << std::endl;
        exit(1);
    }
    
    // -d, --databits
    if(!(cmdl({"-d", "--databits"}, 8) >> databits) || databits < 4 || databits > 8)
    {
        std::cerr << "Wrong databits value: \"" << cmdl({"-d", "--databits"}).str() <<
            "\". Must be in range: 4 - 8" << std::endl;
        exit(1);
    }
    
    // -p, --parity
    std::map<std::string, serial::parity> parity_vals {
        {"none", serial::parity::NONE}, {"odd", serial::parity::ODD},
        {"even", serial::parity::EVEN}, {"mark", serial::parity::MARK},
        {"space", serial::parity::SPACE}};
    std::string parity_str;
    if(!(cmdl({"-p", "--parity"}, "none") >> parity_str) || !parity_vals.count(parity_str))
    {
        std::cerr << "Wrong parity value: \"" << cmdl({"-p", "--parity"}).str() <<
            "\". Must be one of the following: none, odd, even, mark, space" << std::endl;
        exit(1);
    }
    parity = parity_vals[parity_str];
    
    // -s, --stopbits
    std::map<std::string, serial::stopbits> stopbits_vals {
        {"1", serial::stopbits::ONE}, {"1.5", serial::stopbits::ONE_AND_HALF},
        {"2", serial::stopbits::TWO}};
    std::string stopbits_str;
    if(!(cmdl({"-s", "--stopbits"}, "1") >> stopbits_str) || !stopbits_vals.count(stopbits_str))
    {
        std::cerr << "Wrong stopbits value: \"" << cmdl({"-s", "--stopbits"}).str() <<
            "\". Must be one of the following: 1, 1.5, 2" << std::endl;
        exit(1);
    }
    stopbits = stopbits_vals[stopbits_str];
    
    // TODO: handle flow control options
    
    // Check for unknown options
    if(cmdl.flags().size())
    {
        std::cerr << "Unknown options: \n";
        for(auto& flag : cmdl.flags())
            std::cerr << '\"' << flag << "\"\n";
        exit(1);
    }
}
