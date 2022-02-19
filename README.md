# minicom2

[![windows](https://github.com/yhsb2k/minicom2/actions/workflows/build-windows.yml/badge.svg)](https://github.com/yhsb2k/minicom2/actions/workflows/build-windows.yml)
[![linux](https://github.com/yhsb2k/minicom2/actions/workflows/build-linux.yml/badge.svg)](https://github.com/yhsb2k/minicom2/actions/workflows/build-linux.yml)
[![license](https://img.shields.io/github/license/yhsb2k/minicom2?color=blue)](https://github.com/yhsb2k/minicom2/blob/master/LICENSE)

Modern crossplatform serial terminal emulator

## How to build and launch
```
git clone --recursive https://github.com/yhsb2k/minicom2.git
cd minicom2
make

build/minicom2
```

## Requirements
* [GCC, Make](https://winlibs.com)
* [CMake](https://cmake.org/download)
* [Ninja](https://ninja-build.org)
* For Linux (apt): `apt install cmake ninja-build`
