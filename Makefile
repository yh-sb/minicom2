# This makefile is used to shorten build commands with various toolchains.
# You can build the project without it, just calling the CMake manually.

BUILD_DIR ?= build
BUILD_TYPE ?= Debug

.PHONY: gcc msvc clean

all: gcc # Default toolchain

gcc:
	$(eval export CC = gcc)
	$(eval export CXX = g++)
	cmake . -B$(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -j

msvc:
	"$(shell ${ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere -latest -property installationPath)\VC\Auxiliary\Build\vcvarsall.bat" x64 && \
	cmake . -B$(BUILD_DIR) -G Ninja && \
	cmake --build $(BUILD_DIR) -j --config $(BUILD_TYPE)

clean:
	@cmake -E rm -rf $(BUILD_DIR)
