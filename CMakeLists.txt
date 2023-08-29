cmake_minimum_required(VERSION 3.15)

project(minicom2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(serial)
add_subdirectory(terminal)

set(BOOST_INCLUDE_LIBRARIES asio program_options)
add_subdirectory(third_party/boost)

add_executable(${CMAKE_PROJECT_NAME} main.cpp cli_options.cpp)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-static-libgcc;-static-libstdc++;$<$<PLATFORM_ID:Windows>:-static>>
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        serial
        terminal
        Boost::program_options
        $<$<PLATFORM_ID:Linux>:pthread>
)
