# minicom2

[![Build](https://github.com/yh-sb/minicom2/actions/workflows/build.yml/badge.svg)](https://github.com/yh-sb/minicom2/actions/workflows/build.yml)

Modern crossplatform serial terminal emulator

## TODO
- [ ] Fix text output on Raspberry Pi when executing `journalctl`
- [ ] Add posix implementation for [terminal window interface](https://github.com/yhsb2k/minicom2/blob/master/terminal/terminal_posix.cpp) :penguin:
- [ ] Re-architect the application. Maybe approach with coordinator thread is not good?
- [ ] Integrate cli pseudographic interface (to be minicom-like)
- [ ] Fix issue when Shift\Ctrl press inserts `^@` symbol
- [ ] Implement Ctrl+V feature

## How to build and launch
```powershell
xmake -y
xmake run
```

Since pseudo-graphical UI has not yet been integrated, it is possible to configure the serial port using command line options:
```powershell
build/minicom2 --help

Usage:
        minicom2 <device> [options]
Options:
        -h, --help        Print help information
        -b, --baudrate    Baudrate (900 - 3000000). Default is 115200 bps
        -d, --databits    Number of data bits (5-8). Default is 8
        -p, --parity      Parity control (none, odd, even). Default is none
        -s, --stopbits    Number of stopbits (1, 1.5|1,5, 2). Default is 1
        -f, --flowctrl    Flow control type (none, hardware|hw, software|sw). Default is none
```

## Requirements
* [MinGW-w64](https://winlibs.com) or [MSVC](https://visualstudio.microsoft.com/free-developer-offers) or Linux GCC
* [Xmake](https://xmake.io/#/guide/installation)
* For Linux:
    ```bash
    sudo apt update
    sudo apt install libfuse2
    ```
