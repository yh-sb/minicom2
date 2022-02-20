# minicom2

<h3 align="center">Modern crossplatform serial terminal emulator</h3>
<p align="center">
<a href="https://github.com/yhsb2k/minicom2/actions/workflows/windows.yml"><img src="https://github.com/yhsb2k/minicom2/actions/workflows/windows.yml/badge.svg" alt="Windows build badge"></a>
<a href="https://github.com/yhsb2k/minicom2/actions/workflows/linux.yml"><img src="https://github.com/yhsb2k/minicom2/actions/workflows/linux.yml/badge.svg" alt="Linux build badge"></a>
<a href="https://github.com/yhsb2k/minicom2/blob/master/LICENSE"><img src="https://img.shields.io/github/license/yhsb2k/minicom2?color=blue" alt="License"></a>
</p>

## TODO
- [ ] Add posix implementation for [serial port interface](https://github.com/yhsb2k/minicom2/blob/master/serial/serial_posix.cpp) :penguin:
- [ ] Add posix implementation for [terminal window interface](https://github.com/yhsb2k/minicom2/blob/master/terminal/terminal_posix.cpp) :penguin:
- [ ] Re-architect the application. Maybe approach with coordinator thread is not good?
- [ ] Integrate cli pseudographic interface (minicom-like)
- [ ] Fix strange behaviour when Shift is pressid in vi editor opened on raspberry pi. Somehow Shift inserts `^@` symbol (press ':', then Shift)

## How to build and launch
```
git clone --recursive https://github.com/yhsb2k/minicom2.git
cd minicom2
make

build/minicom2 --help
```

## Requirements
* [GCC, Make](https://winlibs.com)
* [CMake](https://cmake.org/download)
* [Ninja](https://ninja-build.org)
* For Linux (apt): `apt install cmake ninja-build`
