# minicom2

<h3 align="center">Modern crossplatform serial terminal emulator</h3>
<p align="center">
<a href="https://github.com/yhsb2k/minicom2/actions?workflow=build"><img src="https://github.com/yhsb2k/minicom2/workflows/build/badge.svg" alt="Build badge"></a>
<a href="https://github.com/yhsb2k/minicom2/blob/master/LICENSE"><img src="https://img.shields.io/github/license/yhsb2k/minicom2?color=blue" alt="License"></a>
</p>

## TODO
- [ ] Add posix implementation for [terminal window interface](https://github.com/yhsb2k/minicom2/blob/master/terminal/terminal_posix.cpp) :penguin:
- [ ] Re-architect the application. Maybe approach with coordinator thread is not good?
- [ ] Integrate cli pseudographic interface (to be minicom-like)
- [ ] Fix issue when Shift\Ctrl press inserts `^@` symbol
- [ ] Implement Ctrl+V feature

## How to build and launch
```powershell
git clone --recursive https://github.com/yhsb2k/minicom2.git
cd minicom2
make
# or make msvc
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
* [CMake](https://cmake.org/download)
* [Ninja](https://ninja-build.org)
* For Linux:
    ```bash
    sudo apt update
    sudo apt install cmake g++ ninja-build libfuse2
    # Install linuxdeploy
    mkdir -p ~/bin && cd $_
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20230713-1/linuxdeploy-x86_64.AppImage -O linuxdeploy.AppImage
    chmod +x linuxdeploy.AppImage
    export PATH=~/bin:$PATH
    ```
