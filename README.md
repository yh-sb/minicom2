# minicom2

<h3 align="center">Modern crossplatform serial terminal emulator</h3>
<p align="center">
<a href="https://github.com/yhsb2k/minicom2/actions/workflows/windows.yml"><img src="https://github.com/yhsb2k/minicom2/actions/workflows/windows.yml/badge.svg" alt="Windows build badge"></a>
<a href="https://github.com/yhsb2k/minicom2/actions/workflows/linux.yml"><img src="https://github.com/yhsb2k/minicom2/actions/workflows/linux.yml/badge.svg" alt="Linux build badge"></a>
<a href="https://github.com/yhsb2k/minicom2/blob/master/LICENSE"><img src="https://img.shields.io/github/license/yhsb2k/minicom2?color=blue" alt="License"></a>
</p>

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
