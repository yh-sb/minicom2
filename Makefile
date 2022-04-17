# This makefile is used to shorten build commands with various toolchains.
# You can build the project without it, just calling the CMake manually.

BUILD_DIR ?= build
BUILD_TYPE ?= Debug

all: gcc # Default toolchain

gcc:
	$(eval export CC ?= gcc)
	$(eval export CXX ?= g++)
	cmake . -B$(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -j

clang:
	$(eval export CC ?= clang)
	$(eval export CXX ?= clang++)
	cmake . -B$(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -j

msvc:
	cmake . -B$(BUILD_DIR) -G "Visual Studio 17 2022"
	cmake --build $(BUILD_DIR) -j --config $(BUILD_TYPE)

clean:
	@cmake -E rm -rf $(BUILD_DIR)
