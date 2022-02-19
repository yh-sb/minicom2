BUILD_DIR ?= build
BUILD_TYPE ?= Debug
CMAKE_GENERATOR ?= Ninja

ifneq ($(OS),Windows_NT)
NUMBER_OF_PROCESSORS = $(shell grep -c ^processor /proc/cpuinfo)
endif

# Set CMake toolchain
export CC = gcc
export CXX = g++

all:
	cmake . -B$(BUILD_DIR) -G $(CMAKE_GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $(BUILD_DIR) -j $(NUMBER_OF_PROCESSORS)

clean:
	@cmake -E remove_directory $(BUILD_DIR)
